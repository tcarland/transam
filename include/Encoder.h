/**  @file Encoder.h 
  *
  *  Copyright (c) 2011-2021 Timothy Charlton Arland <tcarland@gmail.com>
  *
 **/
#ifndef _TRANSAM_ENCODER_H_
#define _TRANSAM_ENCODER_H_

#include <map>

#include "transam.h"
#include "TransFile.h"


namespace transam {

    
typedef std::map<std::string, encoding_t> EncoderMap;



class Encoder {

  public:

    Encoder ( encoding_t type, int bitrate = TRANSAM_DEFAULT_BITRATE );
    ~Encoder();


    bool         encode      ( TransFile         & infile,
                               TransFile         & outfile );

    bool         encodeFiles ( TransFileList     & infiles,
                               TransFileList     & outfiles,
                               const std::string & outpath = "");

    void         bitrate   ( int bitrate );
    int          bitrate() const;

    void         notags    ( bool notags );
    bool         notags() const;

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

    std::string& getErrorStr();

  public:

    static std::string GetOutputName ( const TransFile   & tf,
                                       encoding_t          type,
                                       const std::string & outpath = "");

    static std::string GetExtension  ( encoding_t  type );


  public:

    static EncoderMap  Encoders;

    static EncoderMap  InitEncoders()
    {
    	EncoderMap   emap;
    	emap["mp3"]  = AUDIO_MP3;
        emap["mp4"]  = AUDIO_MP4;
        emap["m4a"]  = AUDIO_MP4;
        emap["aac"]  = AUDIO_MP4;
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
    std::string _errstr;
};


}  // namespace

#endif  // _TRANSAM_ENCODER_H_