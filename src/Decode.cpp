/**
  *   @file   Decode.cpp 
  *   @author tcarland@gmail.com
  *
  *   Copyright (c) 2011 Timothy Charlton Arland <tcarland@gmail.com>
  * 
 **/
#ifndef _TRANSAM_DECODE_CPP_
#define _TRANSAM_DECODE_CPP_

#include "Decode.h"

#include "StringUtils.h"
#include "FileUtils.h"
#include "CmdBuffer.h"
using namespace tcanetpp;


namespace transam {


//-------------------------------------------------------------------------


Decode::Decode() 
    : _notags(false),
      _dryrun(false),
      _clobber(false),
      _raw(false),
      _debug(false)
{}


Decode::~Decode()
{}


//-------------------------------------------------------------------------

bool
Decode::decode ( const TransFile & infile, TransFile & outfile )
{
    std::string cmd;
   
    if ( FileUtils::IsReadable(outfile.getFileName()) && ! this->clobber() )
    {
        std::cout << "Decode::decode() output file exists: "
                  << outfile.getFileName() << std::endl
                  << "    Set --clobber option to overwrite."
                  << std::endl;
        return false;
    }

    cmd = this->getDecoderExec(infile, outfile.getFileName());

    if ( cmd.empty() ) {
        std::cout << "Decoder has no exec for '" << infile.getFileName()
            << "'" << std::endl;
        return false;
    }

    std::cout << "exec: `" << cmd << "'" << std::endl;

    if ( this->_dryrun )
        return true;
 
    CmdBuffer  cmdbuf;

    if ( ! cmdbuf.Open(cmd) ) {
        std::cout << "decode() Error in cmd open." << std::endl;
        return false;
    }

    StringBuffer  lines;
    StringBuffer::iterator sIter;

    cmdbuf.getAllLines(lines);

    for ( sIter = lines.begin(); sIter != lines.end(); ++sIter )
        std::cout << " '" << *sIter << std::endl;

    if ( ! this->notags() )
        outfile.setTags(infile.getTags());

    return true;
}

//-------------------------------------------------------------------------

bool
Decode::decodePath ( TransFileList & wavs, const std::string & path,
                     const std::string & outpath )
{
    TransFileList  files;
    TransFileList::iterator  fIter;

    if ( ! TransFile::ReadPath(path, files, _notags) ) {
        std::cout << "Reading files from path '" << path
            << "' failed." << std::endl;
        return false;
    }
    
    for ( fIter = files.begin(); fIter != files.end(); ++fIter )
    {
        TransFile & intf    = (TransFile&) *fIter;
        std::string outfile = this->getOutputName(intf.getFileName(), outpath);

        if ( intf.type() < AUDIO_MP3 )
        {
            std::cout << "Decode::decodePath() input file " << intf.getFileName()
                      << " is already a wav/pcm file." << std::endl;
            wavs.push_back(TransFile(outfile, intf.type()));
        }
        else 
        {
            if ( ! this->notags() )
            {
                if ( intf.type() != AUDIO_SHN && ! intf.readTags() )
                    std::cout << "decodePath() ERROR reading metadata tags: "
                              << intf.getFileName() << std::endl;
            }

            if ( outfile.empty() ) {
                std::cout << "decodePath() ERROR generating output filename."
                		  << std::endl;
                return false;
            }

            if ( _debug )
            	std::cout << "\noutfile: " << outfile << std::endl;

            TransFile outtf(outfile, AUDIO_WAV);

            if ( this->decode(intf, outtf) ) {
                wavs.push_back(outtf);
            } else { 
                return false;
            }
        }
    }

    if ( this->_debug )
        std::cout << "Decoding finished.\n" << std::endl;

    files.clear();

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

bool
Decode::notags() const
{
    return _notags;
}

//-------------------------------------------------------------------------

void
Decode::clobber ( bool clobber )
{
    _clobber = clobber;
}

bool
Decode::clobber() const
{
    return _clobber;
}

//-------------------------------------------------------------------------

void
Decode::raw ( bool raw )
{
    _raw = raw;
}

bool
Decode::raw() const 
{
    return _raw;
}

//-------------------------------------------------------------------------

std::string
Decode::getDecoderExec ( const TransFile   & infile, const std::string & outfile )
{
    std::string  cmd;

    switch ( infile.getType() )
    {
        case AUDIO_MP3:
            cmd = MP3_DECODER;
            if ( this->raw() )
                cmd.append(MP3D_RAW);
            cmd.append(MP3D_OPTS);
            cmd.append("\"").append(outfile).append("\" ");
            cmd.append("\"").append(infile.getFileName()).append("\"");
            break;
        case AUDIO_MP4:
            cmd = MP4_DECODER;
            cmd.append(MP4_IF);
            cmd.append("\"").append(infile.getFileName()).append("\"");
            cmd.append(MP4_OF);
            cmd.append("\"").append(outfile).append("\"");
            break;
        case AUDIO_FLAC:
            cmd = FLAC_DECODER;
            if ( this->raw() )
                cmd.append(FLACD_RAW);
            cmd.append(FLACD_OPTS);
            cmd.append("\"").append(outfile).append("\" ");
            cmd.append("\"").append(infile.getFileName()).append("\"");
            break;
        case AUDIO_SHN:
            cmd = SHN_DECODER;
            if ( this->raw() )
                cmd.append(SHN_RAW);
            cmd.append(SHN_OPTS);
            cmd.append("\"").append(infile.getFileName()).append("\" ");
            cmd.append("\"").append(outfile).append("\" ");
            break;
        case AUDIO_UNK:
        case AUDIO_WAV:  // TODO: Allow WAV to RAW
        case AUDIO_RAW:
            std::cout << "Decode::getDecoderExec() Skipping raw/pcm/wav file" << std::endl;
            break;
        default:
            std::cout << "Decode::getDecoderExec() Unsupported format." << std::endl;
            break;
    }

    return cmd;
}

//-------------------------------------------------------------------------

std::string
Decode::getOutputName ( const std::string & infile, const std::string & outpath )
{
    std::string  outf;
    int  indx;

    indx = StringUtils::lastIndexOf(infile, ".");
    outf = infile.substr(0, indx);

    if ( this->raw() )
        outf.append(".raw");
    else
        outf.append(".wav");

    if ( ! outpath.empty() )
    {
        indx = StringUtils::lastIndexOf(outf, "/");
        if ( indx >= 0 )
            outf = outf.substr(indx+1);
        outf = outpath + "/" + outf;
    }

    return outf;
}

//-------------------------------------------------------------------------

}  // namespace

#endif  // _TRANSAM_DECODE_CPP_

