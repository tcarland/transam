#ifndef _TRANSAM_TRANSAM_H_
#define _TRANSAM_TRANSAM_H_

#include <inttypes.h>


namespace transam {


#define TRANSAM_DEFAULT_BITRATE 384


typedef enum encoding_types {
    AUDIO_UNK     = 0,
    AUDIO_WAV     = 1,
    AUDIO_MP3     = 2,
    AUDIO_MP4     = 3,
    AUDIO_FLAC    = 4,
    AUDIO_SHN     = 5,
    AUDIO_OGG     = 6
} encoding_t;


} // namespace


#endif // _TRANSAM_TRANSAM_H_

