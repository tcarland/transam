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
   
    // send tf?
    cmd = this->getDecoder(infile, outfile.getFileName());

    if ( cmd.empty() ) {
        std::cout << "Decoder has no exec for '" << infile.getFileName()
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

    if ( ! this->notags() )
        outfile.setTags(infile.getTags());

    return true;
}

//-------------------------------------------------------------------------

bool
Decode::decodePath ( FileList & wavs, const std::string & path )
{
    FileList  files;
    FileList::iterator  fIter;

    if ( ! TransFile::ReadFiles(path, files, _notags) ) {
        std::cout << "Reading files from path '" << path
            << "' failed." << std::endl;
        return false;
    }
    
    for ( fIter = files.begin(); fIter != files.end(); ++fIter )
    {
        TransFile & tf      = (TransFile&) *fIter;
        std::string outfile = Decode::GetOutputName(tf.getFileName());

        if ( tf.type() < AUDIO_MP3 )
        {
            std::cout << "DecodePath() input file is already a wav/pcm file." << std::endl;
            wavs.push_back(TransFile(outfile, AUDIO_WAV));
        }
        else 
        {
        	if ( ! this->notags() ) {
        		if ( ! tf.readTags() )
        			std::cout << "Error reading metadata tags" << std::endl;
			}

            if ( outfile.empty() ) {
                std::cout << "Error generating output filename" << std::endl;
                return false;
            }
            if ( FileUtils::IsReadable(outfile) && ! this->clobber() ) {
                std::cout << "Decode::decodePath() output file exists: " << outfile 
                    << std::endl << "    Set --clobber option to overwrite" << std::endl;
            } else {
            	TransFile outtf(outfile, AUDIO_WAV);
            	if ( this->decode(tf, outtf) )
                    wavs.push_back(outtf);
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
Decode::GetOutputName ( const std::string & infile )
{
    std::string  outf;
    int  indx;

    indx = StringUtils::lastIndexOf(infile, ".");
    outf = infile.substr(0, indx);

    outf.append(".wav");

    return outf;
}

//-------------------------------------------------------------------------

}  // namespace

#endif  // _TRANSAM_DECODE_CPP_

