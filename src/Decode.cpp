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



Decode::Decode() 
    : _notags(false),
      _dryrun(false),
      _clobber(false),
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

    cmd = this->getDecoder(infile, outfile.getFileName());

    if ( cmd.empty() ) {
        std::cout << "Decoder has no exec for '" << infile.getFileName()
            << "'" << std::endl;
        return false;
    }

    if ( this->_dryrun ) {
        std::cout << "exec: `" << cmd << "'" << std::endl;
        return true;
    }
 
    CmdBuffer  cmdbuf;

    if ( ! cmdbuf.Open(cmd) ) {
        std::cout << "Error in cmd exec: " << cmd << std::endl;
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
        std::string outfile = Decode::GetOutputName(intf.getFileName(), outpath);

        if ( intf.type() < AUDIO_MP3 )
        {
            std::cout << "Decode::decodePath() input file " << intf.getFileName()
                      << " is already a wav/pcm file." << std::endl;
            wavs.push_back(TransFile(outfile, AUDIO_WAV));
        }
        else 
        {
            if ( ! this->notags() )
            {
                if ( ! intf.readTags() )
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

std::string
Decode::getDecoder ( const TransFile   & infile,
                     const std::string & outfile )
{
    std::string  cmd;

    switch ( infile.getType() )
    {
        case AUDIO_MP3:
            cmd = MP3_DECODER;
            cmd.append(MP3D_OPTS).append(outfile);
            cmd.append(" ").append(infile.getFileName());
            break;
        case AUDIO_MP4:
            cmd = MP4_DECODER;
            cmd.append(MP4_IF).append(infile.getFileName());
            cmd.append(MP4_OF).append(outfile);
            break;
        case AUDIO_FLAC:
            cmd = FLAC_DECODER;
            cmd.append(FLACD_OPTS).append(outfile);
            cmd.append(" ").append(infile.getFileName());
            break;
        case AUDIO_SHN:
            cmd = SHN_DECODER;
            cmd.append(SHN_OPTS).append(infile.getFileName());
            cmd.append(" ").append(outfile);
            break;
        case AUDIO_UNK:
        case AUDIO_WAV:
        default:
            std::cout << "Decode::getDecoder() Unsupported format." << std::endl;
            break;
    }

    return cmd;
}

//-------------------------------------------------------------------------

std::string
Decode::GetOutputName ( const std::string & infile, const std::string & outpath )
{
    std::string  outf;
    int  indx;

    indx = StringUtils::lastIndexOf(infile, ".");
    outf = infile.substr(0, indx);

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

