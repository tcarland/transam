/**  @file   Decode.h 
  * 
  * Copyright (c) 2011-2020 Timothy Charlton Arland <tcarland@gmail.com>
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

    void        raw      ( bool raw );
    bool        raw() const;

    void        debug    ( bool debug );
    bool        debug() const;

    std::string getDecoderExec( const TransFile   & infile,
                                const std::string & outfile );

    std::string getOutputName ( const std::string & infile,
                                const std::string & outpath = "" );
  
  private:

    bool        _notags;
    bool        _dryrun;
    bool        _clobber;
    bool        _raw;
    bool        _debug;
};


}  // namespace

#endif  // _TRANSAM_DECODE_H_