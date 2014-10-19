/**
  * @file src/Decode.cpp 
  * @author tcarland@gmail.com
  * 
 **/
#ifndef _TRANSAM_DECODE_CPP_
#define _TRANSAM_DECODE_CPP_

#include "Decode.h"

#include "StringUtils.h"
#include "CmdBuffer.h"
using namespace tcanetpp;

namespace transam {



Decode::Decode() 
    : _notags(false),
      _dryrun(false),
      _debug(false)
{}


Decode::~Decode() {}

//-------------------------------------------------------------------------

bool
Decode::decode ( TransFile & tf, const std::string & outfile )
{
    if ( tf.type() == AUDIO_WAV )
        return true;

    std::string cmd;
   
    // send tf?
    cmd = this->getDecoder(tf.getFileName(), outfile, tf.getType());

    if ( cmd.empty() ) {
        std::cout << "Decoder has no exec for '" << tf.getFileName() 
            << "'" << std::endl;
        return false;
    }

    if ( this->_dryrun )
    {
        std::cout << "exec: `" << cmd << "'" << std::endl;
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

//-------------------------------------------------------------------------

void
Decode::dryrun ( bool dryrun )
{
    _dryrun = dryrun;
}

bool
Decode::dryrun() const
{
    return _dryrun;
}

//-------------------------------------------------------------------------

void
Decode::debug ( bool debug )
{
    _debug = debug;
}

bool
Decode::debug() const
{
    return _debug;
}

//-------------------------------------------------------------------------

void
Decode::notags ( bool notags )
{
    _notags = notags;
}

//-------------------------------------------------------------------------

bool
Decode::decodePath ( const std::string & path, FileList & wavs )
{
    FileList  files;
    FileList::iterator  fIter;

    if ( ! TransFile::ReadFiles(path, files, _notags) )
        return false;
    
    for ( fIter = files.begin(); fIter != files.end(); ++fIter )
    {
        TransFile * tf      = (TransFile*) *fIter;
        std::string outfile = Decode::GetOutputName(tf->getFileName());

        if ( outfile.empty() ) {
            std::cout << "Error generating output filename" << std::endl;
            return false;
        }

        if ( this->decode(*tf, outfile) )
            wavs.push_back(new TransFile(outfile, AUDIO_WAV));
    }

    return true;
}

//-------------------------------------------------------------------------

std::string
Decode::GetOutputName ( const std::string & infile )
{
    std::string  outf;
    int  indx;

    indx = StringUtils::lastIndexOf(infile, ".");
    outf = infile.substr(0, indx);

    outf.append("wav");

    std::cout << "Decode::GetOutputName() infile = " << infile
              << " outfile = " << outf << std::endl;

    return outf;
}


std::string
Decode::getDecoder ( const std::string & infile, 
                     const std::string & outfile,
                     encoding_t          type )
{
    std::string  cmd;

    switch ( type ) 
    {
        case AUDIO_MP3:
            cmd = MP3_DECODER;
            cmd.append(MP3D_OPTS).append(outfile);
            cmd.append(" ").append(infile);
            break;
        case AUDIO_MP4:
            cmd = MP4_DECODER;
            cmd.append(MP4_IF).append(infile);
            cmd.append(MP4_OF).append(outfile);
            break;
        case AUDIO_FLAC:
            cmd = FLAC_DECODER;
            cmd.append(FLACD_OPTS).append(outfile);
            cmd.append(" ").append(infile);
            break;
        case AUDIO_SHN:
            cmd = SHN_DECODER;
            cmd.append(SHN_OPTS).append(infile);
            cmd.append(" ").append(outfile);
            break;
        case AUDIO_UNK:
        case AUDIO_WAV:
        default:
            std::cout << "Decode::getDecoder() Unsupported format" << std::endl;
            break;
    }

    return cmd;
}


}  // namespace

#endif  // _TRANSAM_DECODE_CPP_

