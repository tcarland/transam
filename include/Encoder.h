/**  @file Encoder.h 
  *
  * Copyright (c) 2010-2021 Timothy Charlton Arland <tcarland@gmail.com>
  *
  * This file is part of TransAM.
  * 
  * TransAM is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * TransAM is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with TransAM.  If not, see <https://www.gnu.org/licenses/>.
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
        emap["m4a"]  = AUDIO_AAC;
        emap["aac"]  = AUDIO_AAC;
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