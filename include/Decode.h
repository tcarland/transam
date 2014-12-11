/**
  *   @file   Decode.h 
  *   @author tcarland@gmail.com
  *
  *   Copyright (c) 2011 Timothy Charlton Arland <tcarland@gmail.com>
  * 
 **/
#ifndef _TRANSAM_DECODE_H_
#define _TRANSAM_DECODE_H_

#include "transam.h"
#include "TransFile.h"


namespace transam {


class Decode {

  public:

    Decode();
    ~Decode();


    bool        decode      ( const TransFile   & infile,
                              TransFile         & outfile );

    bool        decodePath  ( TransFileList     & wavs,
                              const std::string & path,
							  const std::string & outpath = "" );

    void        notags   ( bool notags );
    bool        notags() const;

    void        dryrun   ( bool dryrun );
    bool        dryrun() const;

    void        clobber  ( bool clobber );
    bool        clobber() const;

    void        debug    ( bool debug );
    bool        debug() const;

    std::string getDecoder  ( const TransFile   & infile,
                              const std::string & outfile );

  public:

    static std::string GetOutputName ( const std::string & infile,
                                       const std::string & outpath = "" );
  

  private:

    bool        _notags;
    bool        _dryrun;
    bool        _clobber;
    bool        _debug;
    
};


}  // namespace

#endif  // _TRANSAM_DECODE_H_

