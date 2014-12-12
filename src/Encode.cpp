/**
  *   @file   Encode.cpp 
  *   @author tcarland@gmail.com
  *
  *   Copyright (c) 2011 Timothy Charlton Arland <tcarland@gmail.com>
  * 
 **/
#ifndef _TRANSAM_ENCODE_CPP_
#define _TRANSAM_ENCODE_CPP_

#include <unistd.h>

#include "Encode.h"

#include "CmdBuffer.h"
#include "StringUtils.h"
#include "FileUtils.h"
using namespace tcanetpp;


namespace transam {



EncoderMap Encode::Encoders = Encode::InitEncoders();


Encode::Encode ( encoding_t type, int rate ) 
    : _type(type),
      _bitrate(rate),
      _notags(false),
      _dryrun(false),
      _erase(true),
      _clobber(false),
      _debug(false)
{}

Encode::~Encode() {}

//-------------------------------------------------------------------------

bool
Encode:: encode ( TransFile & infile, TransFile & outfile )
{
    std::string  cmd;

    if ( FileUtils::IsReadable(outfile.getFileName()) && ! this->clobber() ) {
        std::cout << "encode() output file exists: "
        		  << outfile.getFileName() << std::endl
                  << "  Set --clobber option to overwrite." << std::endl;
        return false;
    }

    if ( infile.type() != AUDIO_WAV )
    {
        std::cout << "encode() Error input format invalid: "
                  << infile.type() << std::endl;
        return false;
    }

    cmd = this->getEncoderExec(infile.getFileName(), outfile.getFileName());

    if ( cmd.empty() ) {
        std::cout << "encode() Error determining encoder." << std::endl;
        return false;
    }

    std::cout << " exec: '" << cmd << "'" << std::endl;

    if ( this->dryrun() )
        return true;

    CmdBuffer  cmdbuf;

    if ( ! cmdbuf.Open(cmd) ) {
        std::cout << "encode() Error in cmd open." << std::endl;
        return false;
    }

    StringBuffer  lines;
    StringBuffer::iterator sIter;
    
    cmdbuf.getAllLines(lines);

    for ( sIter = lines.begin(); sIter != lines.end(); ++sIter )
        std::cout << " '" << *sIter << std::endl;

    if ( ! this->notags() ) {
        outfile.setTags(infile.getTags());
        outfile.saveTags();
    }

    return true;
}

//-------------------------------------------------------------------------

bool
Encode::encodeFiles ( TransFileList & infiles, TransFileList & outfiles,
                      const std::string & outpath )
{
    TransFileList::iterator fIter;

    for ( fIter = infiles.begin(); fIter != infiles.end(); ++fIter )
    {
        TransFile  & intf    = (TransFile&) *fIter;
        std::string  outfile = Encode::GetOutputName(intf, _type, outpath);

        if ( outfile.empty() ) {
            std::cout << "Error generating output filename" << std::endl;
            return false;
        }
        if ( _debug )
            std::cout << "\noutfile: " << outfile << std::endl;

        TransFile  outtf(outfile, this->_type);

        if ( this->encode(intf, outtf) )
        {
            if ( ! FileUtils::IsReadable(outfile) && ! _dryrun )
            {
                std::cout << "ERROR! Outfile not readable, problem with encoder exec?"
                    << std::endl;
                return false;
            }

            std::cout << " . ";
            if ( _debug )
                std::cout << intf.getFileName() << " >> " << outfile << std::endl;

            if ( this->erase() && ! this->dryrun() )
            {
                ::unlink(intf.getFileName().c_str());
                if ( _debug )
                    std::cout << "  DELETE: " << intf.getFileName() << std::endl;
            }
        }
        else
        {
            std::cout << "ERROR! in decode()" << std::endl;
            return false;
        }
    }

    std::cout << std::endl;

    return true;
}

//-------------------------------------------------------------------------

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

//-------------------------------------------------------------------------

void
Encode::notags ( bool notags )
{
    _notags = notags;
}

bool
Encode::notags() const
{
    return _notags;
}

//-------------------------------------------------------------------------

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

//-------------------------------------------------------------------------

void
Encode::clobber ( bool clobber )
{
    _clobber = clobber;
}

bool
Encode::clobber() const
{
    return _clobber;
}

//-------------------------------------------------------------------------

void
Encode::erase ( bool erase )
{
    _erase = erase;
}

bool
Encode::erase() const
{
    return _erase;
}

//-------------------------------------------------------------------------

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

//-------------------------------------------------------------------------

std::string
Encode::GetOutputName ( const TransFile   & tf, encoding_t type, 
                        const std::string & outpath )
{
    std::string outf, ext;
    int         indx;

    indx = StringUtils::lastIndexOf(tf.getFileName(), ".");
    outf = tf.getFileName().substr(0, indx);
    ext  = Encode::GetExtension(type);
    outf.append(ext);

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

std::string
Encode::GetExtension ( encoding_t type )
{
    std::string  ext = ".unk";

    switch ( type ) {
        case AUDIO_MP3:
            ext = ".mp3";
            break;
        case AUDIO_MP4:
            ext = ".mp4";
            break;
        case AUDIO_FLAC:
            ext = ".flac";
            break;
        case AUDIO_OGG:
            ext = ".ogg";
            break;
        default:
            break;
    }

    return ext;
}

//-------------------------------------------------------------------------

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

//-------------------------------------------------------------------------

}  // namespace


#endif  // _TRANSAM_ENCODE_CPP_
