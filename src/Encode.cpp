/**
  * @file src/Encode.cpp 
  * @author 
  * 
 **/
#ifndef _TRANSAM_ENCODE_CPP_
#define _TRANSAM_ENCODE_CPP_

#include "Encode.h"

#include "CmdBuffer.h"
#include "StringUtils.h"
using namespace tcanetpp;


namespace transam {



EncoderMap Encode::Encoders = Encode::InitEncoders();


Encode::Encode ( encoding_t type, int rate ) 
    : _type(type),
      _bitrate(rate),
	  _notags(false),
	  _dryrun(false),
	  _debug(false)
{}

Encode::~Encode() {}


bool
Encode:: encode ( TransFile & tf, const std::string & outfile )
{
    std::string cmd;

    if ( tf.type() != AUDIO_WAV )
    {
        // decode first
        ;
        return false;
    }

    cmd = this->getEncoderExec(tf.getFileName(), outfile);

    if ( this->dryrun() )
    {
        std::cout << "exec: '" << cmd << "'" << std::endl;
        return true;
    }

    CmdBuffer     cmdbuf(cmd);
    StringBuffer  lines;
    StringBuffer::iterator sIter;
    
    cmdbuf.getAllLines(lines);

    if ( lines.empty() ) 
        return false;

    for ( sIter = lines.begin(); sIter != lines.end(); ++sIter )
        std::cout << " '" << *sIter << std::endl;

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
Encode::dryrun ( bool dryrun )
{
    _dryrun = dryrun;
}

bool
Encode::dryrun() const
{
    return _dryrun;
}


void
Encode::debug ( bool debug )
{
    _debug = debug;
}

bool
Encode::debug() const
{
    return _debug;
}

bool
Encode::EncodeFiles ( FileList & files, encoding_t type, int rate )
{
    Encode  encoder(type, rate);

    FileList::iterator fIter;

    for ( fIter = files.begin(); fIter != files.end(); ++fIter )
    {
        TransFile  * tf      = (TransFile*) *fIter;
        std::string  outfile = Encode::GetOutputName(*tf);

        if ( outfile.empty() ) {
            std::cout << "Error generating output filename" << std::endl;
            return false;
        }

        if ( encoder.encode(*tf, outfile) )
            std::cout << "Encoded to " << outfile << std::endl;
        else
            return false;
    }

    return true;
}


std::string
Encode::GetOutputName ( TransFile & tf )
{
    std::string outf, ext;
    int         indx;

    indx = StringUtils::lastIndexOf(tf.getFileName(), ".");
    outf = tf.getFileName().substr(0, indx);

    // set extension
    // outf.append(ext);

    std::cout << "Encode::GetOutputName() outfile = " << outf << std::endl;

    return outf;
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
            break;
        case AUDIO_MP4:
            cmd = MP4_ENCODER;
            cmd.append(MP4E_OPTS).append(br);
            cmd.append(MP4_IF).append(infile);
            cmd.append(MP4_OF).append(outfile);
            break;
        case AUDIO_FLAC:
            cmd = FLAC_ENCODER;
            cmd.append(FLACE_OPTS).append(outfile);
            cmd.append(" ").append(infile);
            break;
        case AUDIO_OGG:
            break;
        default:
            break;
    }

    return cmd;
}


}  // namespace


#endif  // _TRANSAM_ENCODE_CPP_
