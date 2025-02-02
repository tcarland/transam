/** @file TransFile.h
  * 
  * Copyright (c) 2010-2025 Timothy Charlton Arland <tcarland@gmail.com>
  *
  * This file is part of TransAM.
  * 
  * TransAM is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * TransAM is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with TransAM.  If not, see <https://www.gnu.org/licenses/>.
  * 
 **/
#ifndef _TRANSAM_TRANSFILE_H_
#define _TRANSAM_TRANSFILE_H_

#include <list>
#include <map>

// taglib
#include "fileref.h"
#include "tpropertymap.h"

#include "transam.h"

#include "util/FileUtils.h"
using namespace tcanetpp;


namespace transam {


class TransFile;

typedef std::list<TransFile>    TransFileList;
typedef std::list<std::string>  StringList;
typedef TagLib::PropertyMap     TagMap;

typedef std::map<std::string, std::string> TagNameMap;


class TransFile {

  public:

    TransFile();
    TransFile ( const std::string & filename, encoding_t type );
    ~TransFile();

    bool          operator<    ( const TransFile & file ) const;
    bool          operator==   ( const TransFile & file ) const;

    const
    std::string&  getFileName() const;

    encoding_t    getEncodingType() const { return this->type(); }
    encoding_t    getType() const { return this->type(); }
    encoding_t    type() const;


    void          setTags      ( const TagMap      & tags );
    bool          setTag       ( const std::string & key,
                                 const std::string & val );
    std::string   getTag       ( const std::string & key );
    const TagMap& getTags() const;

    bool          haveTags();
    bool          hasTags()    { return this->haveTags(); }
    bool          readTags();
    bool          saveTags();
    void          printTags    ( bool usetagfmt = false ) const;
    void          printAllTags() const;


    static bool   ReadPath     ( const std::string & path,
                                 TransFileList     & files,
                                 bool                recursive,
                                 bool                notag = false );

    static enc_t  GetEncoding  ( const std::string & name );

    static void   ListTags     ( const std::string & path,
                                 bool                allTags,
                                 bool                tagfmt = false );

    static bool   SetTags      ( const std::string & tags,
                                 const std::string & target,
                                 bool                recursive = false );

    static bool   SetTag       ( TransFile         & file,
                                 StringList        & taglist );

    static bool   SetTrackNo   ( const std::string & path, bool ask = true );

    static void   AllowAnyTag  ( bool any );

  protected:

    void          setType();

  public:

    static TagNameMap  TagIndex;
  
  private:

    std::string   _fileName;
    TagMap        _pmap;
    encoding_t    _type;

    static bool   _AnyTags;
    
};


} // namespace

#endif //  _TRANSAM_TRANSFILE_H_