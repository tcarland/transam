/**
  *  @file    transam.h
  *  @author  Timothy C. Arland <tcarland@gmail.com>
  *
  *  Copyright (c) 2010-2021 Timothy C. Arland <tcarland@gmail.com>
  *
  *    TRANS-AM  Transcoding for Audio Media
  *  Allows for single-step transcoding from one audio format to another
  *  such as flac->mp3 or flac->mp4 while keeping metadata intact.
 **/
#ifndef _TRANSAM_TRANSAM_H_
#define _TRANSAM_TRANSAM_H_

#include <inttypes.h>
#include <string>
#include <map>


namespace transam {


#define TRANSAM_VERSION  "v21.01"
#define TRANSAM_LICENSE  "Copyright (c)2010-2021 Timothy C. Arland <tcarland@gmail.com>"

#define TRANSAM_DEFAULT_BITRATE 384


// supported encoding types
typedef enum encoding_types {
    AUDIO_UNK     = 0,
    AUDIO_RAW     = 1,
    AUDIO_WAV     = 2,
    AUDIO_MP3     = 3,
    AUDIO_MP4     = 4,
    AUDIO_FLAC    = 5,
    AUDIO_SHN     = 6,
    AUDIO_OGG     = 7
} encoding_t;

typedef encoding_t enc_t;


// Command and options for each encoding type
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

// as libfaac improves, consider supporting this option over nero
static std::string  AAC_ENCODER  = "ffmpeg";
static std::string  AAC_OPTS     = " -w -c:a aac -b:a 256k";

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