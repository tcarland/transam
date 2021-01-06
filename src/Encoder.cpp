/** @file  Encoder.cpp
  *
  * Copyright (c) 2010-2021 Timothy Charlton Arland <tcarland@gmail.com>
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
#define _TRANSAM_ENCODER_CPP_

#include <unistd.h>

#include "Encoder.h"

#include "util/CmdBuffer.h"
#include "util/StringUtils.h"
#include "util/FileUtils.h"
using namespace tcanetpp;


namespace transam {


EncoderMap Encoder::Encoders = Encoder::InitEncoders();


Encoder::Encoder ( encoding_t type, int rate )
    : _type(type),
      _bitrate(rate),
      _notags(false),
      _dryrun(false),
      _erase(true),
      _clobber(false),
      _debug(false)
{}

Encoder::~Encoder() {}

//-------------------------------------------------------------------------

bool
Encoder::encode ( TransFile & infile, TransFile & outfile )
{
    std::string  cmd;

    if ( FileUtils::IsReadable(outfile.getFileName()) && ! this->clobber() ) {
        _errstr = "Encoder::encode() output file exists, use '--clobber' to overwrite";
        return false;
    }

    if ( infile.type() < AUDIO_RAW || infile.type() > AUDIO_WAV )
    {
        _errstr ="Encoder::encode() Input format unknown or unsupported";
        return false;
    }

    cmd = this->getEncoderExec(infile.getFileName(), outfile.getFileName());

    if ( cmd.empty() ) {
        _errstr = "Encoder::encode() error determining encoder type.";
        return false;
    }

    std::cout << "Encode exec: '" << cmd << "'" << std::endl;

    if ( this->dryrun() )
        return true;

    CmdBuffer  cmdbuf;

    if ( ! cmdbuf.open(cmd) ) {
        _errstr = "Encoder::encode() Error in command exec";
        return false;
    }

    StringBuffer  lines;
    StringBuffer::iterator sIter;

    cmdbuf.getAllLines(lines);

    for ( sIter = lines.begin(); sIter != lines.end(); ++sIter )
        std::cout << " '" << *sIter << std::endl;

    if ( ! this->notags() && infile.haveTags() ) {
        outfile.setTags(infile.getTags());
        outfile.saveTags();
    }
    cmdbuf.close();

    return true;
}

//-------------------------------------------------------------------------

bool
Encoder::encodeFiles ( TransFileList & infiles, TransFileList & outfiles,
                      const std::string & outpath )
{
    TransFileList::iterator fIter;

    for ( fIter = infiles.begin(); fIter != infiles.end(); ++fIter )
    {
        TransFile  & intf    = (TransFile&) *fIter;
        std::string  outfile = Encoder::GetOutputName(intf, _type, outpath);

        if ( outfile.empty() ) {
           _errstr = "Encode Error generating output filename";
            return false;
        }
        if ( _debug )
            std::cout << "\n  outfile: " << outfile << std::endl;

        TransFile  outtf(outfile, this->_type);

        if ( this->encode(intf, outtf) )
        {
            if ( ! FileUtils::IsReadable(outfile) && ! _dryrun )
            {
                _errstr = "encodeFiles() output not readable, problem with encoder?";
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
            return false;
        }
    }

    std::cout << std::endl;

    return true;
}

//-------------------------------------------------------------------------

int
Encoder::bitrate() const
{
    return _bitrate;
}

void
Encoder::bitrate ( int rate )
{
    _bitrate = rate;
}

//-------------------------------------------------------------------------

void
Encoder::notags ( bool notags )
{
    _notags = notags;
}

bool
Encoder::notags() const
{
    return _notags;
}

//-------------------------------------------------------------------------

void
Encoder::dryrun ( bool dryrun )
{
    _dryrun = dryrun;
}

bool
Encoder::dryrun() const
{
    return _dryrun;
}

//-------------------------------------------------------------------------

void
Encoder::clobber ( bool clobber )
{
    _clobber = clobber;
}

bool
Encoder::clobber() const
{
    return _clobber;
}

//-------------------------------------------------------------------------

void
Encoder::erase ( bool erase )
{
    _erase = erase;
}

bool
Encoder::erase() const
{
    return _erase;
}

//-------------------------------------------------------------------------

void
Encoder::debug ( bool debug )
{
    _debug = debug;
}

bool
Encoder::debug() const
{
    return _debug;
}

//-------------------------------------------------------------------------

std::string
Encoder::GetOutputName ( const TransFile   & tf, encoding_t type,
                        const std::string & outpath )
{
    std::string outf, ext;
    int         indx;

    indx = StringUtils::LastIndexOf(tf.getFileName(), ".");
    outf = tf.getFileName().substr(0, indx);
    ext  = Encoder::GetExtension(type);
    outf.append(ext);

    if ( ! outpath.empty() )
    {
        indx = StringUtils::LastIndexOf(outf, "/");
        if ( indx >= 0 )
            outf = outf.substr(indx+1);
        outf = outpath + "/" + outf;
    }

    return outf;
}

//-------------------------------------------------------------------------

std::string
Encoder::GetExtension ( encoding_t type )
{
    std::string  ext = ".unk";

    switch ( type ) {
        case AUDIO_MP3:
            ext = ".mp3";
            break;
        case AUDIO_MP4:
            ext = ".m4a";
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
Encoder::getEncoderExec ( const std::string & infile,
                         const std::string & outfile )
{
    std::string  cmd;
    std::string  br  = StringUtils::ToString(this->bitrate());

    switch ( this->_type )
    {
        case AUDIO_MP3:
            cmd = MP3_ENCODER;
            cmd.append(MP3E_OPTS).append(br);
            cmd.append(" \"").append(infile).append("\"");
            cmd.append(" \"").append(outfile).append("\"");
            break;
        case AUDIO_MP4:
            cmd = MP4_ENCODER;
            cmd.append(MP4E_OPTS).append(br);
            cmd.append(MP4_IF);
            cmd.append("\"").append(infile).append("\"");
            cmd.append(MP4_OF);
            cmd.append("\"").append(outfile).append("\"");

            // ffmpeg
            //cmd = AAC_ENCODER;
            //cmd.append(AAC_IF);
            //cmd.append("\"").append(infile).append("\"");
            //cmd.append(AAC_OPTS);
            //cmd.append("\"").append(outfile).append("\"")'

            break;
        case AUDIO_FLAC:
            cmd = FLAC_ENCODER;
            if ( this->bitrate() == 24 )
                cmd.append(FLACE_2496);
            else if (this->bitrate() == 16 )
                cmd.append(FLACE_1648);
            cmd.append(FLACE_OPTS);
            cmd.append("\"").append(outfile).append("\" ");
            cmd.append("\"").append(infile).append("\" ");
            break;
        case AUDIO_OGG:
            break;
        default:
            break;
    }

    return cmd;
}

//-------------------------------------------------------------------------

std::string&
Encoder::getErrorStr() 
{
    return _errstr;
}

//-------------------------------------------------------------------------

}  // namespace

// _TRANSAM_ENCODER_CPP_