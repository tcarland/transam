/**
  *   @file transam_main.cpp
  *   @author tcarland@gmail.com
  *   Copyright (c) 2011 Timothy Charlton Arland
  *
  *   TODO: show taginfo, alternate output path
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
#include "Decode.h"
#include "Encode.h"
using namespace transam;

#include "StringUtils.h"
using namespace tcanetpp;



static
const char Process[] = "transam";
static
const char Version[] = "0.2.3";




void version()
{
    printf("%s version: v%s\n", Process, Version);
    exit(0);
}

void usage()
{
    printf("Usage: %s [-dEhnvVW] [-t type] [-i infile] [-o outfile] <path>\n", Process);
    printf("     -b | --bitrate        :  bitrate for the encoding process (default is 384).\n");
    printf("     -d | --decode         :  decode only to a .wav file (default is to encode).\n");
    printf("     -E | --noerase        :  do NOT erase wav files after encoding.\n");
    printf("     -h | --help           :  display help information and exit.\n");
    printf("     -L | --list           :  list only the id3/4 tags for each file.\n");
    printf("     -n | --dryrun         :  enable the 'dryrun' option, no changes are made.\n");
    printf("     -i | --infile <file>  :  name of the file to transcode.\n");
    printf("     -o | --outfile <file> :  name of the target output file.\n");
    printf("     -p | --outpath <path> :  alternate output path to place generated files.\n");
    printf("     -t | --type <name>    :  The encoding type by extension (if applicable).\n");
    printf("                           :  supported types are: flac, mp3, mp4, ogg, shn, wav");
    printf("     -T | --notags         :  Disable converting metadata tags to new format.\n");
    printf("     -W | --clobber        :  Allow the overwriting of files that already exist.\n");
    printf("     -v | --verbose        :  enable verbose output.\n");
    printf("     -V | --version        :  display version info and exit.\n");
    exit(0);
}


void sigHandler ( int signal )
{
    return;
}


encoding_t setEncodingType ( const std::string & typestr )
{
    encoding_t type;

    EncoderMap::iterator fIter;

    if ( (fIter = Encode::Encoders.find(typestr)) == Encode::Encoders.end() )
        return AUDIO_UNK;

    type = fIter->second;

    return type;
}


void listTags ( const std::string & path, encoding_t type )
{
    TransFileList  files;
    TransFileList::iterator  fIter;

    if ( ! TransFile::ReadPath(path, files) )
    {
        std::cout << "ERROR reading files from path '" << path
            << "'" << std::endl;
        return;
    }

    files.sort();

    for ( fIter = files.begin(); fIter != files.end(); ++fIter )
    {
        TransFile & tf = (TransFile&) *fIter;
        if ( ! tf.haveTags() ) {
        	std::cout << "NOTAGS: " << tf.getFileName() << std::endl;
        	continue;
        }
        if ( type > AUDIO_UNK && type == tf.type() )
            tf.printTags();
        else if ( type == AUDIO_UNK )
        	tf.printTags();
    }
    return;
}



int main ( int argc, char **argv )
{
    char optChar;
    char *infile  = NULL;
    char *outfile = NULL;
    char *outpath = NULL;
    char *type    = NULL;
    char *br      = NULL;
    bool verbose  = false;
    bool dryrun   = false;
    bool decode   = false;
    bool noerase  = false;
    bool notags   = false;
    bool clobber  = false;
    bool showtags = false;
    int  optindx  = 0;

    uint16_t rate = TRANSAM_DEFAULT_BITRATE;

    static struct option l_opts[] = { {"bitrate", required_argument, 0, 'b'},
                                      {"decode",  no_argument, 0, 'd'},
                                      {"dryrun",  no_argument, 0, 'n'},
                                      {"noerase", no_argument, 0, 'E'},
                                      {"help",    no_argument, 0, 'h'},
                                      {"list",    no_argument, 0, 'L'},
                                      {"infile",  required_argument, 0, 'i'},
                                      {"outfile", required_argument, 0, 'o'},
									  {"outpath", required_argument, 0, 'p'},
                                      {"type",    required_argument, 0, 't'},
                                      {"notags",  no_argument, 0, 'T'},
                                      {"verbose", no_argument, 0, 'v'},
                                      {"version", no_argument, 0, 'V'},
                                      {"clobber", no_argument, 0, 'W'},
                                      {0,0,0,0}
                                    };


    while ( (optChar = ::getopt_long(argc, argv, "dEhi:Lo:p:nt:vVW", l_opts, &optindx)) != EOF )
    {
        switch ( optChar ) {
            case 'b':
              br = strdup(optarg);
              break;
            case 'd':
              decode = true;
              break;
            case 'E':
              noerase = true;
              break;
            case 'h':
              usage();
              break;
            case 'i':
              infile = strdup(optarg);
              break;
            case 'L':
              showtags = true;
              break;
            case 'n':
              dryrun = true;
              break;
            case 'o':
              outfile = strdup(optarg);
              break;
            case 'p':
              outpath = strdup(optarg);
              break;
            case 't':
              type  = strdup(optarg);
              break;
            case 'T':
              notags = true;
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

    std::string inf, outf, outp, path;

    encoding_t  enctype = AUDIO_UNK;

    if ( optind == argc && infile == NULL ) {
        std::cout << "No target defined." << std::endl;
        usage();
    } else if ( infile == NULL ) {
        path = argv[optind];
        if ( path.empty() )
            usage();
    }
    // else set outfile?

    if ( infile != NULL ) {
        inf.assign(infile);
        ::free(infile);
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
        rate = StringUtils::fromString<uint16_t>(br);
        ::free(br);
    }

    // list only
    if ( showtags )
    {
        if ( path.empty() ) {
            std::cout << "Path required with the --list option" << std::endl;
            return -1;
        }

        listTags(path, enctype);

        return 0;
    }

    if ( enctype < 2 && inf.empty() && ! decode ) {
        std::cout << "Error! Encoding type not provided or detected." << std::endl;
        usage();
    }

    Decode  decoder;
    decoder.debug(verbose);
    decoder.dryrun(dryrun);
    decoder.notags(notags);
    decoder.clobber(clobber);

    Encode  encoder(enctype, rate);
    encoder.debug(verbose);
    encoder.dryrun(dryrun);
    encoder.clobber(clobber);
    encoder.erase(!noerase);

    if ( ! path.empty() )
    {
        TransFileList  wavs, outfiles;
        TransFileList::iterator fIter;

        if ( ! decoder.decodePath(wavs, path, outp) ) {
            std::cout << "Error decoding files" << std::endl;
            return -1;
        }
        if ( ! decode ) {
            if ( ! encoder.encodeFiles(wavs, outfiles, outp) ) {
                std::cout << "Error encoding files" << std::endl;
                return -1;
            }
        }
    }
    else
    {
        if ( inf.empty() ) {
            std::cout << "Error! Input file not provided." << std::endl;
            usage();
        }

        TransFile intf(inf, TransFile::GetEncoding(inf));
        TransFile wav(Decode::GetOutputName(inf), AUDIO_WAV);
        TransFile outf(Encode::GetOutputName(wav, enctype), enctype);

        if ( ! decoder.decode(intf, wav) ) {
            std::cout << "Error decoding. " << std::endl;
            return -1;
        }
        if ( ! decode ) {
            if ( ! encoder.encode(wav, outf) ) {
                std::cout << "Error encoding files." << std::endl;
                return -1;
            }
        }
    }

    std::cout << "Finished." << std::endl;

    return 0;
}

