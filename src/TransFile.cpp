/**
  *   @file   TransFile.cpp
  *   @author tcarland@gmail.com
  *
  *   Copyright (c) 2011 Timothy Charlton Arland <tcarland@gmail.com>
  *
 **/
#define _TRANSAM_TRANSFILE_CPP_

#include <sys/stat.h>
#include <dirent.h>


#include "TransFile.h"

#include "StringUtils.h"
using namespace tcanetpp;


namespace transam {


TransFile::TransFile()
    : _type(AUDIO_UNK)
{}

TransFile::TransFile ( const std::string & filename, encoding_t type )
    : _fileName(filename),
      _type(type)
{
}

TransFile::~TransFile()
{}


bool
TransFile::operator< ( const TransFile & file ) const
{
    return(_fileName < file.getFileName());
}

bool
TransFile::operator== ( const TransFile & file ) const
{
    if ( this->_fileName.compare(file.getFileName()) == 0 )
        return true;
    return false;
}


bool
TransFile::saveTags()
{
    TagLib::FileRef f(_fileName.c_str());

    if ( f.isNull() )
        return false;

    f.tag()->setProperties(_pmap);
    f.save();

    return true;
}


const std::string&
TransFile::getFileName() const
{
    return _fileName;
}

encoding_t
TransFile::type() const
{
    return _type;
}

void
TransFile::setTags ( const TagMap & map )
{
    _pmap = map;
}

bool
TransFile::setTag ( const std::string & key, const std::string & val )
{
    TagLib::StringList vals;
    vals.append(val);
    return _pmap.replace(key, vals);
}

const TagMap&
TransFile::getTags() const
{
    return _pmap;
}

void
TransFile::printTags()
{
    TagMap::Iterator tIter;

    std::cout << "  ";

    if ( _pmap.isEmpty() )
        return;

    tIter = _pmap.find("ARTIST");
    if ( tIter != _pmap.end() )
        std::cout << tIter->second.front().to8Bit();

    tIter = _pmap.find("ALBUM");
    if ( tIter != _pmap.end() )
        std::cout << " - " << tIter->second.front().to8Bit();

    tIter = _pmap.find("DISCNUMBER");
    if ( tIter != _pmap.end() )
        std::cout << " - d" << tIter->second.front().to8Bit();

    tIter = _pmap.find("TRACKNUMBER");
    if ( tIter != _pmap.end() )
        std::cout << " - t" << tIter->second.front().to8Bit();

    tIter = _pmap.find("TITLE");
    if ( tIter != _pmap.end() )
        std::cout << " - " << tIter->second.front().to8Bit();

    std::cout << std::endl;

    return;
}

bool
TransFile::readTags()
{
    TagLib::FileRef f(_fileName.c_str());

    if ( f.isNull() )
        return false;

    _pmap = f.tag()->properties();

    return true;
}

bool
TransFile::haveTags()
{
    return(!_pmap.isEmpty());
}


bool
TransFile::ReadPath ( const std::string & path, TransFileList & files, bool recursive, bool notag )
{
    DIR*            dirp;
    struct dirent * dire;
    struct stat     fsb;
    std::string     name;
    encoding_t      type;
    TransFile       file;

    if ( (dirp = ::opendir(path.c_str())) == NULL ) {
        std::cout << "TransFile::ReadFiles failed to read directory."
            << std::endl;
        return false;
    }

    while ( (dire = ::readdir(dirp)) != NULL )
    {
        name = dire->d_name;

        if ( name.compare(".") == 0 || name.compare("..") == 0 )
            continue;

        type = (encoding_t) TransFile::GetEncoding(name);
        name = path + "/" + name;

        if ( ::stat(name.c_str(), &fsb) < 0 ) {
            std::cout << "stat failed for " << name << std::endl;
            return false;
        }

        if ( S_ISLNK(fsb.st_mode) )
            continue;

        if ( S_ISDIR(fsb.st_mode) ) {
            if ( ! recursive )
                continue;
            if ( ! TransFile::ReadPath(name, files, recursive, notag) )
                return false;
        }

        if ( type == AUDIO_UNK )
            continue;

        file = TransFile(name, type);

        if ( ! notag )
            file.readTags();

        files.push_back(file);
    }
    ::closedir(dirp);

    return true;
}


/** Returns the encoding id for a given encoding type. */
enc_t
TransFile::GetEncoding ( const std::string & name )
{
    std::string ext;
    int         indx;

    indx = StringUtils::lastIndexOf(name, ".");

    if ( indx <= 0 )
        return AUDIO_UNK;

    ext = name.substr(indx+1);

    if ( ext.compare("wav") == 0 )
        return AUDIO_WAV;
    else if ( ext.compare("pcm") == 0 || ext.compare("raw") == 0 )
        return AUDIO_RAW;
    else if ( ext.compare("mp3") == 0 )
        return AUDIO_MP3;
    else if ( ext.compare("mp4") == 0 )
        return AUDIO_MP4;
    else if ( ext.compare("flac") == 0 )
        return AUDIO_FLAC;
    else if ( ext.compare("shn") == 0 )
        return AUDIO_SHN;
    else if ( ext.compare("ogg") == 0 )
        return AUDIO_OGG;
        
    return AUDIO_UNK;
}


void 
TransFile::ListTags ( const std::string & path, encoding_t type, bool recursive )
{
    TransFileList  files;
    TransFileList::iterator  fIter;

    if ( ! TransFile::ReadPath(path, files, recursive ) )
    {
        std::cout << "ERROR reading files from path '" << path
            << "'" << std::endl;
        return;
    }

    files.sort();

    std::cout << "Listing metadata for files in " << path << std::endl;

    for ( fIter = files.begin(); fIter != files.end(); ++fIter )
    {
        TransFile & tf = (TransFile&) *fIter;
        if ( ! tf.haveTags() ) {
            std::cout << " <NOTAGS> : " << tf.getFileName() << std::endl;
            continue;
        }
        if ( type > AUDIO_UNK && type == tf.type() )
            tf.printTags();
        else if ( type == AUDIO_UNK )
            tf.printTags();
    }

    return;
}

bool
TransFile::SetTags ( const std::string & tags, const std::string & target, bool recursive )
{
    TransFileList files;
    StringList    taglist;
    bool          res = true;

    StringUtils::split(tags, '|', back_inserter(taglist));

    if ( FileUtils::IsDirectory(target) )
    {
        if ( ! TransFile::ReadPath(target, files, recursive) ) {
            std::cout << "ERROR reading files from path '" << target << "'" << std::endl;
            return false;
        }

        files.sort();
    }
    else if ( FileUtils::IsReadable(target) )
    {
        TransFile ta(target, TransFile::GetEncoding(target));
        ta.readTags();
        files.push_back(ta);
    }

    TransFileList::iterator fIter;

    for ( fIter = files.begin(); fIter != files.end(); ++fIter )
    {
        TransFile & tf = (TransFile&) *fIter;

        std::cout << "File: " << tf.getFileName() << std::endl
                  << "  Current Tag: ";

        if (  tf.hasTags() ) 
            tf.printTags();
        else 
            std::cout << "       <none>" << std::endl;

        res = TransFile::SetTag(tf, taglist);

        std::cout << "  Updated Tag: ";
        
        if (  tf.hasTags() )
            tf.printTags();
        else
            std::cout << "       <none>" << std::endl;
    }

    return res;
}


bool 
TransFile::SetTag ( TransFile & tf, StringList & taglist )
{

    std::string key, val;
    int         indx;

    StringList::iterator  sIter;

    for ( sIter = taglist.begin(); sIter != taglist.end(); ++sIter )
    {
        std::string & tag = *sIter;
        indx = StringUtils::indexOf(tag, ":");

        if ( indx < 0 ) {
            std::cout << " Tag format invalid. Should be 'key:val'" << std::endl;
            return false;
        }

        key  = tag.substr(0, indx);
        val  = tag.substr(indx+1);

        if ( ! tf.setTag(key, val) ) {
            std::cout << "Error setting tag for " << tf.getFileName() << std::endl;
            return false;
        }
    }

    tf.saveTags();

    return true;
}



} // namespace 

// _TRANSAM_TRANSFILE_CPP_
