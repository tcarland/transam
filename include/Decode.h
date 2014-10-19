/**
  * @file include/Decode.h 
  * @author tcarland@gmail.com
  * 
 **/
#ifndef _TRANSAM_DECODE_H_
#define _TRANSAM_DECODE_H_

#include "TransFile.h"



namespace transam {



class Decode {

  public:

    Decode();
    ~Decode();


    bool   decode    ( TransFile & tf, const std::string & outfile );


    void   setDryrun ( bool dryrun );
    void   setDebug  ( bool debug );

  public:

    static bool        DecodeFiles    ( FileList & files, FileList & wavs );

    static std::string GetOutputName  ( const std::string & infile );
    static std::string GetDecoderExec ( const std::string & infile,
                                        const std::string & outfile,
                                        encoding_t          type );
  
  private:

    bool        _dryrun;
    bool        _debug;
    
};


}  // namespace

#endif  // _TRANSAM_DECODE_H_

