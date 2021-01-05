/**  @file Decoder.cpp
  * 
  *  Copyright (c) 2011-2021 Timothy Charlton Arland <tcarland@gmail.com>
  *
 **/
#ifndef _TRANSAM_DECODER_CPP_
#define _TRANSAM_DECODER_CPP_

#include "Decoder.h"

#include "util/StringUtils.h"
#include "util/FileUtils.h"
#include "util/CmdBuffer.h"
using namespace tcanetpp;


namespace transam {


//-------------------------------------------------------------------------

Decoder::Decoder()
    : _notags(false),
      _dryrun(false),
      _clobber(false),
      _raw(false),
      _debug(false)
{}


Decoder::~Decoder()
{}

//-------------------------------------------------------------------------

bool
Decoder::decode ( const TransFile & infile, TransFile & outfile )
{
    std::string cmd;

    if ( FileUtils::IsReadable(outfile.getFileName()) && ! this->clobber() )
    {
        _errstr = "Decode target output file already exists, set --clobber option to overwrite.";
        return false;
    }

    cmd = this->getDecoderExec(infile, outfile.getFileName());

    if ( cmd.empty() ) {
        _errstr = "Decoder has no exec for file " + infile.getFileName();
        return false;
    }

    std::cout << "Decode exec: `" << cmd << "'" << std::endl;

    if ( this->_dryrun )
        return true;

    CmdBuffer  cmdbuf;

    if ( ! cmdbuf.open(cmd) ) {
        _errstr = "Decoder error in command";
        return false;
    }

    StringBuffer  lines;
    StringBuffer::iterator sIter;

    cmdbuf.getAllLines(lines);

    for ( sIter = lines.begin(); sIter != lines.end(); ++sIter )
        std::cout << " '" << *sIter << std::endl;

    if ( ! this->notags() )
        outfile.setTags(infile.getTags());
    cmdbuf.close();

    return true;
}

//-------------------------------------------------------------------------

bool
Decoder::decodePath ( TransFileList & wavs, const std::string & path,
                     const std::string & outpath )
{
    TransFileList  files;
    TransFileList::iterator  fIter;

    if ( ! TransFile::ReadPath(path, files, _notags) ) {
        _errstr = "decodePath() failed to read files from path: " + path;
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
                _errstr = "decodePath() ERROR generating output filename.";
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
Decoder::dryrun ( bool dryrun )
{
    _dryrun = dryrun;
}

bool
Decoder::dryrun() const
{
    return _dryrun;
}

//-------------------------------------------------------------------------

void
Decoder::debug ( bool debug )
{
    _debug = debug;
}

bool
Decoder::debug() const
{
    return _debug;
}

//-------------------------------------------------------------------------

void
Decoder::notags ( bool notags )
{
    _notags = notags;
}

bool
Decoder::notags() const
{
    return _notags;
}

//-------------------------------------------------------------------------

void
Decoder::clobber ( bool clobber )
{
    _clobber = clobber;
}

bool
Decoder::clobber() const
{
    return _clobber;
}

//-------------------------------------------------------------------------

void
Decoder::raw ( bool raw )
{
    _raw = raw;
}

bool
Decoder::raw() const
{
    return _raw;
}

//-------------------------------------------------------------------------

std::string
Decoder::getDecoderExec ( const TransFile & infile, const std::string & outfile )
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
            std::cout << "Decoder::getDecoderExec() Skipping raw/pcm/wav file" << std::endl;
            break;
        default:
            std::cout << "Decoder::getDecoderExec() Unsupported format." << std::endl;
            break;
    }

    return cmd;
}

//-------------------------------------------------------------------------

std::string
Decoder::getOutputName ( const std::string & infile, const std::string & outpath )
{
    std::string  outf;
    int  indx;

    indx = StringUtils::LastIndexOf(infile, ".");
    outf = infile.substr(0, indx);

    if ( this->raw() )
        outf.append(".raw");
    else
        outf.append(".wav");

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

std::string&
Decoder::getErrorStr()
{
    return _errstr;
}

//-------------------------------------------------------------------------

}  // namespace

#endif  // _TRANSAM_DECODE_CPP_