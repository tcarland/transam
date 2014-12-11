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

    tIter = _pmap.find("ARTIST");
    if ( tIter != _pmap.end() )
        std::cout << tIter->second;
    tIter = _pmap.find("ALBUM");
    if ( tIter != _pmap.end() )
        std::cout << " - " << tIter->second;
    tIter = _pmap.find("TRACKNUMBER");
    if ( tIter != _pmap.end() )
        std::cout << " - t" << tIter->second;
    tIter = _pmap.find("TITLE");
    if ( tIter != _pmap.end() )
        std::cout << " - " << tIter->second;

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
	if ( _pmap.isEmpty() )
		return false;
	return true;
}


bool
TransFile::ReadPath ( const std::string & path, TransFileList & files, bool notag )
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

    //std::cout << "ReadFiles path: " << path << std::endl;

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

        if ( S_ISLNK(fsb.st_mode) || S_ISDIR(fsb.st_mode) ) {
            std::cout << " Skipping " << name << std::endl;
            continue;
        }

        if ( type == AUDIO_UNK ) {
        	std::cout << " Skipping " << name << " unknown extension." << std::endl;
        	continue;
        }

        file = TransFile(name, type);

        if ( ! notag )
            file.readTags();

        files.push_back(file);
    }

    ::closedir(dirp);

    return true;
}


enc_t
TransFile::GetEncoding ( const std::string & name )
{
    std::string ext;
    int         indx;

    indx = StringUtils::lastIndexOf(name, ".");

    if ( indx <= 0 )
        return AUDIO_UNK;

    ext = name.substr(indx+1);

    if ( ext.compare("wav") == 0 || ext.compare("pcm") == 0 )
        return AUDIO_WAV;
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


} // namespace 

// _TRANSAM_TRANSFILE_CPP_
