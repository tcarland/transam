/**
  * @file src/Encode.cpp 
  * @author 
  * 
 **/
#ifndef _TRANSAM_ENCODE_CPP_
#define _TRANSAM_ENCODE_CPP_

#include "Encode.h"

#include "StringUtils.h"
using namespace tcanetpp;


namespace transam {


Encode::Encode ( encoding_t type, int rate ) 
    : _type(type),
      _bitrate(rate)
{}

Encode::~Encode() {}


bool
Encode:: encode ( TransFile & tf, const std::string & outfile )
{

    return true;
}

int
Encode::bitrate() const
{
    return _bitrate;
}

void
Encode::bitrate ( int rate )
{
    _bitrate = rate;
}

void
Encode::setDryrun ( bool dryrun )
{
    _dryrun = dryrun;
}

void
Encode::setDebug ( bool debug )
{
    _debug = debug;
}

bool
Encode::EncodeFiles ( FileList & files, encoding_t type )
{
    FileList::iterator fIter;

    for ( fIter = files.begin(); fIter != files.end(); ++fIter )
    {

    }


    return true;
}

std::string
Encode::getEncoderExec ( const std::string & infile,
                         const std::string & outfile )
{
    std::string  cmd;
    std::string  br  = StringUtils::toString(this->bitrate());

    switch ( this->_type ) 
    {
        case AUDIO_MP3:
            cmd = MP3_ENCODER;
            cmd.append(MP3E_OPTS).append(br);
            cmd.append(" ").append(infile);
            cmd.append(" ").append(outfile);
            ;;
        case AUDIO_MP4:
            cmd = MP4_ENCODER;
            cmd.append(MP4E_OPTS).append(br);
            cmd.append(MP4_IF).append(infile);
            cmd.append(MP4_OF).append(outfile);
            ;;
        case AUDIO_FLAC:
            cmd = FLAC_ENCODER;
            cmd.append(FLACE_OPTS).append(outfile);
            cmd.append(" ").append(infile);
            ;;
        case AUDIO_OGG:
            ;;
        default:
            ;;
    }

    return cmd;
}


}  // namespace


#endif  // _TRANSAM_ENCODE_CPP_
