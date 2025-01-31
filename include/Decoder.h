/** @file Decoder.h 
  * 
  * Copyright (c) 2010-2025 Timothy Charlton Arland <tcarland@gmail.com>
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
#ifndef _TRANSAM_DECODER_H_
#define _TRANSAM_DECODER_H_

#include "transam.h"
#include "TransFile.h"

namespace transam {

class Decoder {

  public:

    Decoder();
    ~Decoder();

    bool         decode      ( const TransFile   & infile,
                               TransFile         & outfile );

    bool         decodePath  ( TransFileList     & wavs,
                               const std::string & path,
                               const std::string & outpath = "" );

    void         notags   ( bool notags );
    bool         notags() const;

    void         dryrun   ( bool dryrun );
    bool         dryrun() const;

    void         clobber  ( bool clobber );
    bool         clobber() const;

    void         raw      ( bool raw );
    bool         raw() const;

    void         ffmpeg   ( bool f );
    bool         ffmpeg() const;

    void         debug    ( bool debug );
    bool         debug() const;

    std::string  getDecoderExec( const TransFile   & infile,
                                 const std::string & outfile );

    std::string  getOutputName ( const std::string & infile,
                                 const std::string & outpath = "" );

    std::string& getErrorStr();
  
  private:

    bool        _notags;
    bool        _dryrun;
    bool        _clobber;
    bool        _raw;
    bool        _ffmpeg;
    bool        _debug;
    std::string _errstr;
};

}  // namespace

#endif  // _TRANSAM_DECODER_H_