/** @file transam_main.cpp
  * 
  * Copyright (c) 2010-2021 Timothy Charlton Arland <tcarland@gmail.com>
  *
  * This file is part of TransAm.
  * 
  * TransAm is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * TransAm is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with TransAm.  If not, see <https://www.gnu.org/licenses/>.
  * 
 **/
#define _TRANSAM_TRANSAM_CPP_

extern "C" {
# include <unistd.h>
# include <getopt.h>
# include <string.h>
# include <errno.h>
}

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <list>

#include "transam.h"
#include "TransFile.h"
#include "Decoder.h"
#include "Encoder.h"
using namespace transam;

#include "util/StringUtils.h"
#include "util/FileUtils.h"
using namespace tcanetpp;


static
const char Process[] = "transam";


void version()
{
    std::cout << Process << " : "
              << TRANSAM_VERSION << std::endl
              << TRANSAM_LICENSE  << std::endl;
    exit(0);
}

void usage()
{
    std::cout << "Usage: " << Process << " [-dEhnvVW] [-t type] [-o outfile] <infile|path>\n"
              << "     -A | --apply-only     :  Apply tags to the infiles, no decoding.\n"
              << "     -a | --anytags        :  Disable tag name checking when apply tag names. \n"
              << "     -b | --bitrate        :  Bitrate for encoding (default=384). For flac encoding.\n"
              << "                              Using '16' or '24' (24/96khz) requires raw input (-r).\n"
              << "     -d | --decode-only    :  Decode only to a .wav file (default is to encode).\n"
              << "     -E | --no-erase       :  Do NOT erase source WAV files after decode/encode.\n"
              << "     -F | --no-ffmpeg      :  Do not use the 'ffmpeg' app for aac (requires NeroAAC).\n"
              << "     -h | --help           :  Display help information and exit.\n"
              << "     -l | --list           :  List the ID3/4 tags for the given target.\n"
              << "     -L | --listtags       :  List the ID3/4 tags in the --tags format for a file. \n"
              << "     -n | --dryrun         :  Enable the 'dryrun' option, no changes are made.\n"
              << "     -o | --outfile <file> :  Name of the target output file, when <infile> is given.\n"
              << "     -P | --outpath <path> :  Alternate output path to place generated files.\n"
              << "     -r | --raw            :  Decode files to raw PCM data. \n"
              << "     -R | --renum          :  Sets 'apply-only' and offers to re-number tracks.\n"
              << "     -t | --type <name>    :  The encoding type by extension (if applicable).\n"
              << "                           :  supported types: flac, mp3, mp4, ogg, shn, wav\n"
              << "     -T | --tags=\"KEY:val\" :  Set ID3/4 tags on the given file(s). Useful Keys:\n"
              << "                              ARTIST, ALBUM, TRACKNUMBER, TITLE, DISCNUMBER, GENRE \n"
              << "                              Using the format: \"KEY:val|KEY:val|KEY:val|...\"\n"
              << "     -S | --skiptags       :  Do not copy metadata tags to new encoding.\n"
              << "     -W | --clobber        :  Allow the overwriting of files that already exist.\n"
              << "     -v | --verbose        :  Enable verbose output.\n"
              << "     -V | --version        :  Display version info and exit.\n\n"
              << " This application makes use of external binaries for encoding and decoding.\n"
              << " The various apps are: 'lame' for mp3, 'neroAacEnc/Dec' for m4a, 'flac', \n"
              << " 'oggenc/dec' for vorbis, 'ffmpeg' for aac, and 'shorten' for decoding shn files." << std::endl;
    exit(1);
}


void sigHandler ( int signal )
{
    return;
}


encoding_t setEncodingType ( const std::string & typestr )
{
    encoding_t  type;
    EncoderMap::iterator fIter;

    if ( (fIter = Encoder::Encoders.find(typestr)) == Encoder::Encoders.end() )
        return AUDIO_UNK;

    type = fIter->second;

    return type;
}


int main ( int argc, char **argv )
{
    char  optChar;
    char *outfile  = NULL;
    char *outpath  = NULL;
    char *type     = NULL;
    char *tagstr   = NULL;
    char *br       = NULL;
    bool  apply    = false;
    bool  verbose  = false;
    bool  dryrun   = false;
    bool  decode   = false;
    bool  noerase  = false;
    bool  notags   = false;
    bool  renum    = false;
    bool  clobber  = false;
    bool  showtags = false;
    bool  shtagfmt = false;
    bool  anytags  = false;
    bool  raw      = false;
    bool  ffmpeg   = true; 
    int   optindx  = 0;

    uint16_t rate = TRANSAM_DEFAULT_BITRATE;

    static struct option l_opts[] = { {"apply-only", no_argument, 0, 'A'},
                                      {"anytags",    no_argument, 0, 'a'},
                                      {"bitrate",    required_argument, 0, 'b'},
                                      {"decode-only", no_argument, 0, 'd'},
                                      {"no-erase",   no_argument, 0, 'E'},
                                      {"no-ffmpeg",  no_argument, 0, 'F'},
                                      {"dryrun",     no_argument, 0, 'n'},
                                      {"help",       no_argument, 0, 'h'},
                                      {"list",       no_argument, 0, 'l'},
                                      {"infile",     required_argument, 0, 'i'},
                                      {"outfile",    required_argument, 0, 'o'},
                                      {"outpath",    required_argument, 0, 'P'},
                                      {"type",       required_argument, 0, 't'},
                                      {"tags",       required_argument, 0, 'T'},
                                      {"raw",        no_argument, 0, 'r'},
                                      {"renum",      no_argument, 0, 'R'},
                                      {"skiptags",   no_argument, 0, 's'},
                                      {"verbose",    no_argument, 0, 'v'},
                                      {"version",    no_argument, 0, 'V'},
                                      {"clobber",    no_argument, 0, 'W'},
                                      {0,0,0,0}
                                    };

    while ( (optChar = ::getopt_long(argc, argv, "Aab:dEFhi:lLo:P:nrRst:T:vVW", l_opts, &optindx)) != EOF )
    {
        switch ( optChar ) {
            case 'A':
              apply   = true;
              break;
            case 'a':
              anytags = true;
              break;
            case 'b':
              br      = ::strdup(optarg);
              break;
            case 'd':
              decode  = true;
              break;
            case 'E':
              noerase = true;
              break;
            case 'F':
              ffmpeg = false;
              break;
            case 'h':
              usage();
              break;
            case 'L':
              shtagfmt = true;
            case 'l':
              showtags = true;
              break;
            case 'n':
              dryrun  = true;
              break;
            case 'o':
              outfile = ::strdup(optarg);
              break;
            case 'P':
            case 'p':
              outpath = ::strdup(optarg);
              break;
            case 'r':
              raw     = true;
              break;
            case 'R':
              apply   = true;
              renum   = true;
            case 'S':
              notags  = true;
              break;
            case 't':
              type    = ::strdup(optarg);
              break;
            case 'T':
              tagstr  = ::strdup(optarg);
              break;
            case 'v':
              verbose = true;
              break;
            case 'V':
              version();
              break;
            case 'W':
              clobber = true;
              break;
            default:
              // path
              break;
        }
    }

    std::string inf, outf, outp, path, tags;
    encoding_t  enctype = AUDIO_UNK;

    if ( optind < argc )
        path = argv[optind];
    
    if ( path.empty() ) {
        if ( showtags )
            path=".";
        else
            usage();
    }

    if ( tagstr != NULL ) {
        tags.assign(tagstr);
        ::free(tagstr);
    }

    if ( anytags )
        TransFile::AllowAnyTag(anytags);

    if ( apply ) 
    {
        if ( ! tags.empty() ) {
            TransFile::SetTags(tags, path);
        }
        if ( renum ) {
            bool ask = true;
            if ( clobber )
                ask = false;
            TransFile::SetTrackNo(path, ask);
        }
        return 0;
    }

    if ( ! FileUtils::IsDirectory(path) ) {
        inf.assign(path);
        path = "";
    }

    if ( outfile != NULL ) {
        outf.assign(outfile);
        ::free(outfile);
    }
    if ( outpath != NULL ) {
        outp.assign(outpath);
        ::free(outpath);
    }
    if ( type != NULL ) {
        enctype = setEncodingType(type);
        ::free(type);
    }
    if ( br != NULL ) {
        rate = StringUtils::FromString<uint16_t>(br);
        ::free(br);
    }

    // list only
    if ( showtags )
    {
        if ( ! inf.empty() ) { 
            TransFile tf(inf, TransFile::GetEncoding(inf));
            tf.printTags(shtagfmt);
            return 0;
        }

        TransFile::ListTags(path, verbose, shtagfmt);

        return 0;
    }

    if ( enctype < 2 && inf.empty() && ! decode ) {
        if ( ! tags.empty() )
            return 0;
        std::cout << "Error: Encoding type not provided or detected." << std::endl;
        usage();
    }

    if ( ! outp.empty() && ! FileUtils::IsDirectory(outp) )
    {
        if ( FileUtils::IsReadable(outp) ) {
            std::cout << "Error: Output path is invalid (not a directory)." << std::endl;
            return -1;
        }

        if ( (::mkdir(outp.c_str(), 0755)) != 0 ) {
            std::cout << "Error: Failed to create output directory! ";
            if ( errno == EACCES || errno == EPERM )
                std::cout << "No permissions";
            std::cout << std::endl;
            return -1;
        }
    }

    Decoder decoder;
    decoder.debug(verbose);
    decoder.dryrun(dryrun);
    decoder.notags(notags);
    decoder.clobber(clobber);
    decoder.ffmpeg(ffmpeg);
    decoder.raw(raw);

    Encoder encoder(enctype, rate);
    encoder.debug(verbose);
    encoder.dryrun(dryrun);
    encoder.clobber(clobber);
    encoder.ffmpeg(ffmpeg);
    encoder.erase(!noerase);

    if ( ! path.empty() )
    {
        TransFileList  wavs, outfiles;
        TransFileList::iterator fIter;

        if ( ! decoder.decodePath(wavs, path, outp) ) {
            std::cout << "Error decoding files: " << decoder.getErrorStr() << std::endl;
            return -1;
        }
        
        if ( ! decode ) 
        {
            if ( ! encoder.encodeFiles(wavs, outfiles, outp) ) {
                std::cout << "Error encoding files: " << encoder.getErrorStr() << std::endl;
                return -1;
            }
            if ( ! tags.empty() ) {
                if ( outp.empty() )
                    outp.assign(".");
                TransFile::SetTags(tags, outp);
            }
        }
    }
    else
    {
        if ( inf.empty() ) {
            std::cout << "Error: Input file not provided." << std::endl;
            usage();
        }

        TransFile  intf, outtf, wav;

        intf = TransFile(inf, TransFile::GetEncoding(inf));

        if ( intf.type() == AUDIO_RAW )
            decoder.raw(true);

        wav  = TransFile(decoder.getOutputName(inf), intf.type());

        intf.readTags();

        if ( outf.empty() ) {
            if ( enctype == AUDIO_UNK ) {
                std::cout << "Error: Encoding type is required. Output file is not defined."
                          << std::endl;
                return -1;
            }
            outf = Encoder::GetOutputName(wav, enctype);
        }

        if ( enctype == AUDIO_UNK ) {
            int indx = StringUtils::LastIndexOf(outf, ".");
            std::string ext = outf.substr(indx+1);
            enctype = setEncodingType(ext);
        }

        outtf = TransFile(outf, enctype);

        if ( intf.type() <= AUDIO_WAV ) {
            std::cout << "Input file is already raw pcm/wav." << std::endl;
        } else if ( ! decoder.decode(intf, wav) ) {
            std::cout << "Error in decoding: " << decoder.getErrorStr() << std::endl;
            return -1;
        }

        if ( ! decode ) {
            if ( ! encoder.encode(wav, outtf) ) {
                std::cout << "Error encoding files: " + encoder.getErrorStr() << std::endl;
                return -1;
            }
            if ( ! tags.empty() )
                TransFile::SetTags(tags, outtf.getFileName());
        }
    }

    std::cout << "Finished." << std::endl;

    return 0;
}
