/** 
  * @file   TransFile.h
  * @author tcarland@gmail.com
  *
  * Copyright (c) 2010-2019 Timothy Charlton Arland <tcarland@gmail.com>
  *
 **/
#ifndef _TRANSAM_TRANSFILE_H_
#define _TRANSAM_TRANSFILE_H_

#include <list>

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
    void          printTags() const;
    void          printAllTags() const;


    static bool   ReadPath     ( const std::string & path,
                                 TransFileList     & files,
                                 bool                recursive,
                                 bool                notag = false );

    static enc_t  GetEncoding  ( const std::string & name );

    static void   ListTags     ( const std::string & path,
                                 encoding_t          type,
                                 bool                recursive = false );

    static bool   SetTags      ( const std::string & tags,
                                 const std::string & target,
                                 bool                recursive = false );

    static bool   SetTag       ( TransFile         & file,
                                 StringList        & taglist );

    static bool   SetTrackNo   ( const std::string & path, bool ask = true );


  private: 

    void          setType();

  private:

    std::string   _fileName;

    TagMap        _pmap;
    encoding_t    _type;

};


} // namespace

#endif //  _TRANSAM_TRANSFILE_H_

