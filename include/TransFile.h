/** 
  *   @file   TransFile.h
  *   @author tcarland@gmail.com
  *
  *   Copyright (c) 2011 Timothy Charlton Arland <tcarland@gmail.com>
  *
 **/
#ifndef _TRANSAM_TRANSFILE_H_
#define _TRANSAM_TRANSFILE_H_

#include <list>

// taglib
#include "fileref.h"
#include "tpropertymap.h"


#include "transam.h"


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


    void          setTags      ( const TagMap & tags );
    
    const TagMap& getTags() const;

    bool          readTags();
    bool          saveTags();
    void          printTags();


    static bool   ReadPath     ( const std::string & path,
                                 TransFileList     & files,
                                 bool        notag = false );

    static enc_t  GetEncoding  ( const std::string & name );

  private: 

    void          setType();

  private:

    std::string   _fileName;

    TagMap        _pmap;
    encoding_t    _type;

};


} // namespace

#endif //  _TRANSAM_TRANSFILE_H_
