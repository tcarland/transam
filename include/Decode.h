/**
  * @file include/Decode.h 
  * @author tcarland@gmail.com
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


    bool        decode     ( TransFile & tf, const std::string & outfile );
    bool        decodePath ( const std::string & path, FileList & wavs );


    void        notags  ( bool notags );
    bool        notags() const;

    void        dryrun  ( bool dryrun );
    bool        dryrun() const;

    void		clobber ( bool clobber );
    bool        clobber() const;

    void        debug   ( bool debug );
    bool        debug() const;

    std::string getDecoder ( const std::string & infile,
                             const std::string & outfile,
                             encoding_t          type );

  public:

    static std::string GetOutputName  ( const std::string & infile );
  

  private:

    bool        _notags;
    bool        _dryrun;
    bool        _clobber;
    bool        _debug;
    
};


}  // namespace

#endif  // _TRANSAM_DECODE_H_

