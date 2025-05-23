/**
  * @file  transam.h
  *
  * TransAM -  Transcoding for Audio Media
  *
  * Allows for single-step transcoding from one audio format to another
  * such as flac->mp3 or flac->mp4 while keeping metadata intact.
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
#ifndef _TRANSAM_TRANSAM_H_
#define _TRANSAM_TRANSAM_H_

#include <inttypes.h>
#include <string>
#include <map>


namespace transam {

#define TRANSAM_VERSION  "v25.05.18"
#define TRANSAM_LICENSE  "Copyright (c)2010-2025 Timothy C. Arland <tcarland@gmail.com>"

#define TRANSAM_DEFAULT_BITRATE 256

// supported encoding types
typedef enum encoding_types {
    AUDIO_UNK     = 0, // unknown|unsupported
    AUDIO_RAW     = 1, // raw|pcm
    AUDIO_WAV     = 2, // wav
    AUDIO_MP3     = 3, // lame
    AUDIO_MP4     = 4, // nero
    AUDIO_AAC     = 5, // ffmpeg
    AUDIO_FLAC    = 6, // flac
    AUDIO_OGG     = 7, // vorbis
    AUDIO_SHN     = 8  // shorten
} encoding_t;

typedef encoding_t enc_t;


// Command and options for each encoding type
// LAME is the preferred mp3 encoder/decoder
static std::string  MP3_ENCODER  = "lame";
static std::string  MP3_DECODER  = "lame";
static std::string  MP3E_OPTS    = " -h -b ";
static std::string  MP3D_OPTS    = " --decode ";
static std::string  MP3D_RAW     = " --decode -t ";

// neroAacEnc does not support encoding raw pcm
static std::string  MP4_ENCODER  = "neroAacEnc";
static std::string  MP4_DECODER  = "neroAacDec";
static std::string  MP4E_OPTS    = " -br ";
static std::string  MP4_IF       = " -if ";
static std::string  MP4_OF       = " -of ";

// consider switching to ffmpeg as libfaac improves.
// ffmpeg [options] [[infile options] -i infile]... {[outfile options] outfile}...
static std::string  AAC_ENCODER  = "ffmpeg";
static std::string  AAC_DECODER  = "ffmpeg";
static std::string  AAC_FDKOPTS  = " -c:a libfdk_aac "; // fraunhaufer lib
static std::string  AAC_AACOPTS  = " -c:a aac ";
static std::string  AAC_CBR      = "-b:a 128k ";
static std::string  AAC_VBR      = "-vbr 5 ";
static std::string  AAC_IF       = " -i ";
static std::string  AAC_FORMAT   = "-f adts";  // AAC-LE

// FLAC
static std::string  FLAC_ENCODER = "flac";
static std::string  FLAC_DECODER = "flac";
static std::string  FLACD_OPTS   = " -d --output-name=";
static std::string  FLACD_RAW    = " --force-raw-format";
static std::string  FLACD_FORCE  = " -f";
static std::string  FLACE_OPTS   = " -8 -V -f -o ";
static std::string  FLACE_1648   = " --endian=little --sign=signed --channels=2 --bps=16 --sample-rate=48000";
static std::string  FLACE_2496   = " --endian=little --sign=signed --channels=2 --bps=24 --sample-rate=96000";

static std::string  OGG_ENCODER  = "oggenc";
static std::string  OGG_DECODER  = "oggdec";
static std::string  OGGD_OPTS    = " --output=";
static std::string  OGGD_RAW     = " --bits=16 --raw ";

static std::string  SHN_DECODER  = "shorten";
static std::string  SHN_OPTS     = " -x ";
static std::string  SHN_RAW      = " -t s16";

} // namespace

#endif // _TRANSAM_TRANSAM_H_
