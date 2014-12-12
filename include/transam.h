#ifndef _TRANSAM_TRANSAM_H_
#define _TRANSAM_TRANSAM_H_

#include <inttypes.h>
#include <string>
#include <map>


namespace transam {


#define TRANSAM_VERSION        "v0.2.8"
#define TRANSAM_LICENSE        "Copyright (c)2010-2013 Timothy C. Arland <tcarland@gmail.com>"
#define TRANSAM_DEFAULT_BITRATE 384

// supported encoding types
typedef enum encoding_types {
    AUDIO_UNK     = 0,
    AUDIO_WAV     = 1,
    AUDIO_MP3     = 2,
    AUDIO_MP4     = 3,
    AUDIO_FLAC    = 4,
    AUDIO_SHN     = 5,
    AUDIO_OGG     = 6
} encoding_t;

typedef encoding_t enc_t;

// Command and options for each encoding type
static std::string   MP3_ENCODER  = "lame";
static std::string   MP3E_OPTS    = " -h -b ";
static std::string   MP3_DECODER  = "mpg123";
static std::string   MP3D_OPTS    = " -w ";

static std::string   MP4_ENCODER  = "neroAacEnc";
static std::string   MP4_DECODER  = "neroAacDec";
static std::string   MP4E_OPTS    = " -br ";
static std::string   MP4_IF       = " -if ";
static std::string   MP4_OF       = " -of ";

static std::string   FLAC_ENCODER = "flac";
static std::string   FLAC_DECODER = "flac";
static std::string   FLACE_OPTS   = " -8 -V -f -o ";
static std::string   FLACD_OPTS   = " -d --output-name=";
static std::string   FLACE_2496   = " --bps=24 --sample-rate=96000";

static std::string   OGG_ENCODER  = "oggenc";
static std::string   OGG_DECODER  = "oggdec";

static std::string   SHN_DECODER  = "shorten";
static std::string   SHN_OPTS     = " -x ";


} // namespace

#endif // _TRANSAM_TRANSAM_H_

