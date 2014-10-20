/**
  * @file include/Encode.h 
  * @author tcarland@gmail.com
  * 
 **/
#ifndef _TRANSAM_ENCODE_H_
#define _TRANSAM_ENCODE_H_

#include <map>

#include "transam.h"
#include "TransFile.h"


namespace transam {


typedef std::map<std::string, encoding_t> EncoderMap;


class Encode {

  public:

    Encode     ( encoding_t type, int bitrate = TRANSAM_DEFAULT_BITRATE );
    ~Encode();


    bool         encode    ( TransFile         & tf, 
                             const std::string & outfile );


    void         bitrate   ( int bitrate );
    int          bitrate() const;

    void         clobber   ( bool clobber );
    bool         clobber() const;

    void         dryrun    ( bool dryrun );
    bool         dryrun() const;

    void         erase     ( bool erase );
    bool         erase() const;

    void         debug     ( bool debug );
    bool         debug() const;


    std::string  getEncoderExec ( const std::string & infile,
                                  const std::string & outfile );
 

    static bool  EncodeFiles    ( FileList & files, encoding_t type, int rate );

    static std::string GetOutputName ( TransFile & tf );


  public:

    static EncoderMap  Encoders;
    static EncoderMap  InitEncoders()
    {
    	EncoderMap   emap;
    	emap["mp3"]  = AUDIO_MP3;
        emap["mp4"]  = AUDIO_MP4;
        emap["flac"] = AUDIO_FLAC;
        emap["shn"]  = AUDIO_SHN;
        emap["ogg"]  = AUDIO_OGG;
        return emap;
    }

  private:

    encoding_t  _type;
    int         _bitrate;
    bool        _notags;
    bool        _dryrun;
    bool        _erase;
    bool        _clobber;
    bool        _debug;
};


}  // namespace

#endif  // _TRANSAM_ENCODE_H_

