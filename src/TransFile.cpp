/**  @file TransFile.cpp
  *
  * Copyright (c) 2010-2021 Timothy Charlton Arland <tcarland@gmail.com>
  *
  * This file is part of TransAm.
  * 
  * TransAm is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * TransAm is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with TransAm.  If not, see <https://www.gnu.org/licenses/>.
  * 
 **/
#define _TRANSAM_TRANSFILE_CPP_

#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include <iomanip>

#include "tstringlist.h" // taglib

#include "TransFile.h"

#include "util/StringUtils.h"
using namespace tcanetpp;


namespace transam {


TagNameMap TransFile::TagIndex = { 
    {"TITLE", "TITLE"},
    {"TRACKNAME", "TITLE"},
    {"TRACK", "TITLE"},
    {"ALBUM", "ALBUM"},
    {"ALBUMNAME", "ALBUM"},
    {"ALBUMTITLE", "ALBUM"},
    {"ARTIST", "ARTIST"},
    {"ARTISTNAME", "ARTIST"},
    {"TRACKNUMBER", "TRACKNUMBER"},
    {"TRACKNO", "TRACKNUMBER"},
    {"DISCNUMBER", "DISCNUMBER"},
    {"DISCNO", "DISCNUMBER"},
    {"DISKNUMBER", "DISCNUMBER"},
    {"DATE", "DATE"},
    {"ORIGINALDATE", "ORIGINALDATE"},
    {"GENRE", "GENRE"},
    {"COMMENT", "COMMENT"}
};

bool TransFile::_AnyTags = false;


//-------------------------------------------------------------------------

TransFile::TransFile()
    : _type(AUDIO_UNK)
{}

TransFile::TransFile ( const std::string & filename, encoding_t type )
    : _fileName(filename),
      _type(type)
{
    this->readTags();
}

TransFile::~TransFile()
{}

//-------------------------------------------------------------------------

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

//-------------------------------------------------------------------------

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

//-------------------------------------------------------------------------

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

//-------------------------------------------------------------------------

void
TransFile::setTags ( const TagMap & map )
{
    _pmap = map;
}

bool
TransFile::setTag ( const std::string & key, const std::string & val )
{
    std::string tag;
    TagNameMap::iterator tIter = TagIndex.find(key);

    if ( ! TransFile::_AnyTags ) {
        if ( tIter == TagIndex.end() )
            return false;
        else
            tag = tIter->second;
    }

    TagLib::StringList vals;
    vals.append(val);

    return _pmap.replace(tag, vals);
}

std::string
TransFile::getTag ( const std::string & key )
{
    TagMap::ConstIterator tIter;
    std::string val = "";

    tIter = _pmap.find(key);

    if ( tIter != _pmap.end() )
        val = tIter->second.front().to8Bit();

    return val;
}

const TagMap&
TransFile::getTags() const
{
    return _pmap;
}

//-------------------------------------------------------------------------

void
TransFile::printTags ( bool usetagfmt ) const
{
    TagMap::ConstIterator tIter;

    if ( ! usetagfmt )
        std::cout << "'" << this->getFileName() << "': ";

    if ( _pmap.isEmpty() )
        return;

    tIter = _pmap.find("ARTIST");
    if ( tIter != _pmap.end() ) {
        if ( usetagfmt ) 
            std::cout << "ARTIST:'";
        std::cout << tIter->second.front().to8Bit();
        if ( usetagfmt ) 
            std::cout << "'";
    }

    tIter = _pmap.find("ALBUM");
    if ( tIter != _pmap.end() ) {
        if ( usetagfmt )
            std::cout << "|ALBUM:'";
        else
            std::cout << " - ";
        std::cout << tIter->second.front().to8Bit();
        if ( usetagfmt )
            std::cout << "'";
    }

    tIter = _pmap.find("DISCNUMBER");
    if ( tIter != _pmap.end() ) {
        if ( usetagfmt)
            std::cout << "|DISCNUMBER:'";
        else 
            std::cout << " - d";
        std::cout << tIter->second.front().to8Bit();
        if ( usetagfmt)
            std::cout << "'";
    }

    tIter = _pmap.find ("GENRE");
    if ( tIter != _pmap.end() ) {
        if ( usetagfmt )
            std::cout <<"|GENRE:'";
        else
            std::cout << " - ";
        std::cout << tIter->second.front().to8Bit();
        if ( usetagfmt )
            std::cout << "'";
    }

    tIter = _pmap.find("TRACKNUMBER");
    if ( tIter != _pmap.end() ) {
        if ( usetagfmt )
            std::cout << "|TRACKNUMBER:'";
        else
            std::cout << " - t";
        std::cout << tIter->second.front().to8Bit();
        if ( usetagfmt )
            std::cout << "'";
    }

    tIter = _pmap.find("TITLE");
    if ( tIter != _pmap.end() ) {
        if ( usetagfmt )
            std::cout << "|TITLE:'";
        else
            std::cout << " - ";
        std::cout << tIter->second.front().to8Bit();
        if ( usetagfmt )
            std::cout << "'";
    }

    std::cout << std::endl;

    return;
}

void
TransFile::printAllTags() const
{
    TagMap::ConstIterator tIter;
    TagLib::StringList::ConstIterator sIter;

    std::cout << this->getFileName() << ":" << std::endl;

    for ( tIter = _pmap.begin(); tIter != _pmap.end(); ++tIter )
    {
        std::cout << "  " << tIter->first;
        TagLib::StringList & strlist = (TagLib::StringList&) tIter->second;
        for ( sIter = strlist.begin(); sIter != strlist.end(); ++sIter )
        {
            TagLib::String & str = (TagLib::String&) *sIter;
            std::cout << " : " << str;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

//-------------------------------------------------------------------------

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

//-------------------------------------------------------------------------

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

//-------------------------------------------------------------------------

/** Returns the encoding id for a given encoding type. */
enc_t
TransFile::GetEncoding ( const std::string & name )
{
    std::string ext;
    int         indx;

    indx = StringUtils::LastIndexOf(name, ".");

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
    else if ( ext.compare("m4a") == 0 || ext.compare("aac") == 0 )
        return AUDIO_AAC;
    else if ( ext.compare("flac") == 0 )
        return AUDIO_FLAC;
    else if ( ext.compare("shn") == 0 )
        return AUDIO_SHN;
    else if ( ext.compare("ogg") == 0 )
        return AUDIO_OGG;

    return AUDIO_UNK;
}

//-------------------------------------------------------------------------

void
TransFile::ListTags ( const std::string & path, bool allTags, bool recursive )
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

        if ( allTags )
            tf.printAllTags();
        else
            tf.printTags();
    }

    return;
}

//-------------------------------------------------------------------------

bool
TransFile::SetTags ( const std::string & tags, const std::string & target, bool recursive )
{
    TransFileList files;
    StringList    taglist;
    bool          res = true;

    StringUtils::Split(tags, '|', back_inserter(taglist));

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

        if ( res ) {
            std::cout << "  Updated Tag: ";
            if (  tf.hasTags() )
                tf.printTags();
            else
                std::cout << "  SetTag Failed" << std::endl;
        }
    }

    return res;
}

//-------------------------------------------------------------------------

bool
TransFile::SetTag ( TransFile & tf, StringList & taglist )
{

    std::string key, val;
    int         indx;

    StringList::iterator  sIter;

    for ( sIter = taglist.begin(); sIter != taglist.end(); ++sIter )
    {
        std::string & tag = *sIter;
        indx = StringUtils::IndexOf(tag, ":");

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

//-------------------------------------------------------------------------

bool
TransFile::SetTrackNo ( const std::string & path, bool ask )
{
    TransFileList files;
    TransFileList::iterator fIter;

    if ( ! TransFile::ReadPath(path, files, false) ) {
        std::cout << "ERROR reading files from path '" << path << "'" << std::endl;
        return false;
    }

    files.sort();

    int trackno = 1;
    for ( fIter = files.begin(); fIter != files.end(); ++fIter )
    {
        TransFile & tf = (TransFile&) *fIter;

        std::cout << std::setw(48) << std::setiosflags(std::ios_base::left)
                  << tf.getFileName() << std::setw(18)
                  << " => TRACKNUMBER:" << trackno << std::endl;
        ++trackno;
    }

    char y = 'n';
    if ( ask )
    {
        std::cout << "Renumber the tracks? (y/n) ";
        std::cin >> y;

        if ( y == 'y' || y == 'Y' )
            std::cout << "Renumbering tracks.." << std::endl;
        else
            return false;
    }

    trackno = 1;
    for ( fIter = files.begin(); fIter != files.end(); ++fIter )
    {
        TransFile & tf = (TransFile&) *fIter;
        tf.setTag("TRACKNUMBER", StringUtils::ToString(trackno));
        tf.saveTags();
        ++trackno;
    }

    TransFile::ListTags(path, AUDIO_UNK);

    return true;
}

//-------------------------------------------------------------------------

void
TransFile::AllowAnyTag ( bool any )
{
    _AnyTags = any;
}

//-------------------------------------------------------------------------

} // namespace

// _TRANSAM_TRANSFILE_CPP_