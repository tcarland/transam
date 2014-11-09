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
TransFile::save()
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
TransFile::getType() const
{
    return _type;
}

void
TransFile::setTags ( TagLib::PropertyMap map )
{
    _pmap = map;
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
TransFile::ReadFiles ( const std::string & path, FileList & files, bool notag )
{
    DIR*            dirp;
    struct dirent * dire;
    struct stat     fsb;
    std::string     name;
    encoding_t      type;
    TransFile *     file = NULL;

    if ( (dirp = ::opendir(path.c_str())) == NULL ) {
        std::cout << "TransFile::ReadFiles failed to read directory."
            << std::endl;
        return false;
    }

    std::cout << "ReadFiles path " << path << std::endl;

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

        file = new TransFile(name, type);

        if ( ! notag )
            file->readTags();

        files.push_back(file);
    }

    ::closedir(dirp);

    return true;
}


int
TransFile::GetEncoding ( const std::string & name )
{
    std::string ext;
    int         indx;

    indx = StringUtils::lastIndexOf(name, ".");

    if ( indx <= 0 )
        return 0;

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
