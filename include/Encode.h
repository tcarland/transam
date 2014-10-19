/**
  * @file include/Encode.h 
  * @author tcarland@gmail.com
  * 
 **/
#ifndef _TRANSAM_ENCODE_H_
#define _TRANSAM_ENCODE_H_

#include "transam.h"
#include "TransFile.h"


namespace transam {



class Encode {

  public:

    Encode     ( encoding_t type, int bitrate = TRANSAM_DEFAULT_BITRATE );
    ~Encode();


    bool         encode    ( TransFile         & tf, 
                             const std::string & outfile );


    void         bitrate   ( int bitrate );
    int          bitrate() const;

    void         dryrun    ( bool dryrun );
    bool         dryrun() const;

    void         debug     ( bool debug );
    bool         debug() const;


    std::string  getEncoderExec ( const std::string & infile,
                                  const std::string & outfile );
 

    static bool  EncodeFiles    ( FileList & files, encoding_t type, int rate );

    static std::string GetOutputName ( TransFile & tf );


  private:

    encoding_t  _type;
    int         _bitrate;
    bool        _tags;
    bool        _dryrun;
    bool        _debug;
};


}  // namespace

#endif  // _TRANSAM_ENCODE_H_

