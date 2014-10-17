/** 
  *   @file TransFile.h
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
typedef std::list<TransFile*> FileList;


class TransFile {

  public:

    TransFile ( const std::string & filename, int type );
    ~TransFile();

    bool          operator<     ( const TransFile & file ) const;
    bool          operator==    ( const TransFile & file ) const;

    const
    std::string&  getFileName() const;

    bool          save();
    bool          readTags();
    void          setTags       ( TagLib::PropertyMap map );
    static bool   ReadFiles     ( const std::string & path,
                                  FileList          & files,
                                  bool                notag );
    static int    GetEncoding   ( const std::string & name );
  private: 

    void          setType();

  private:

    std::string          _fileName;

    TagLib::PropertyMap  _pmap;
    encoding_t           _type;

};


} // namespace

#endif //  _TRANSAM_TRANSFILE_H_

