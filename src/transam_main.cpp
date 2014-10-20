/**
  *   @file transam_main.cpp
  *   @author tcarland@gmail.com
  *   Copyright (c) 2011 Timothy Charlton Arland
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
#include "Decode.h"
#include "Encode.h"
using namespace transam;

#include "StringUtils.h"
using namespace tcanetpp;



static
const char Process[] = "transam";
static
const char Version[] = "0.1.2";



void version()
{
    printf("%s version: v%s\n", Process, Version);
    exit(0);
}

void usage()
{
    printf("Usage: %s [-dEhnvVW] [-t type] [-i infile] [-o outfile] <path>\n", Process);
    printf("     -b | --bitrate        :  bitrate for the encoding process (default is 384)\n");
    printf("     -d | --decode         :  decode only to a .wav file (default is to encode)\n");
    printf("     -E | --erase          :  erase wav files after decoding\n");
    printf("     -h | --help           :  display help information and exit");
    printf("     -i | --infile <file>  :  name of the file to transcode\n");
    printf("     -o | --outfile <file> :  name of the target output file\n");
    printf("     -n | --dryrun         :  enable the 'dryrun' option, no changes are made\n");
    printf("     -t | --type  <name>   :  The encoding type by extension (if applicable)\n");
    printf("     -T | --notags         :  Disable converting metadata tags to new format\n");
    printf("     -W | --overwrite      :  Allow the overwriting of files that already exit\n");
    printf("     -v | --verbose        :  enable verbose output\n");
    printf("     -V | --version        :  display version info and exit\n");
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


int main ( int argc, char **argv )
{
    char optChar;
    char *infile  = NULL;
    char *outfile = NULL;
    char *type    = NULL;
    char *br      = NULL;
    bool verbose  = false;
    bool dryrun   = false;
    bool decode   = false;
    bool erase    = false;
    bool notags   = false;
    bool clobber  = false;
    int  optindx  = 0;
    int  cd       = 0;

    uint16_t rate = TRANSAM_DEFAULT_BITRATE;

    static struct option l_opts[] = { {"bitrate", required_argument, 0, 'b'},
                                      {"decode",  no_argument, 0, 'd'},
                                      {"dryrun",  no_argument, 0, 'n'},
									  {"erase",   no_argument, 0, 'E'},
                                      {"help",    no_argument, 0, 'h'},
                                      {"infile",  required_argument, 0, 'i'},
                                      {"outfile", required_argument, 0, 'o'},
                                      {"type",    required_argument, 0, 't'},
                                      {"notags",  no_argument, 0, 'T'},
                                      {"verbose", no_argument, 0, 'v'},
                                      {"version", no_argument, 0, 'V'},
                                      {"overwrite", no_argument, 0, 'W'},
                                      {0,0,0,0}
                                    };


    while ( (optChar = ::getopt_long(argc, argv, "dEhi:o:nt:vV", l_opts, &optindx)) != EOF ) 
    {
        switch ( optChar ) {
            case 'b':
              br = strdup(optarg);
              break;
            case 'd':
              decode = true;
              break;
            case 'E':
              erase = true;
              break;
            case 'h':
              usage();
              break;
            case 'i':
              infile = strdup(optarg);
              break;
            case 'o':
              outfile = strdup(optarg);
              break;
            case 'n':
              dryrun = true;
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

    std::string inf, outf, path;
    encoding_t  enctype = AUDIO_UNK;

    if ( optind == argc && infile == NULL ) {
        std::cout << "No target defined." << std::endl;
        usage();
    } else if ( infile == NULL ) {
        path = argv[optind];
        if ( path.empty() )
            usage();
/*
        cd   = ::chdir(path.c_str());
        if ( cd < 0 ) 
        {
            if ( errno == EACCES ) {
                std::cout << "No permission for " << path << std::endl;
                return -1;
            } else {
                std::cout << "Error with target: " << path << ": "
                    << std::string(strerror(errno)) << std::endl;
                return -1;
            }
        }
*/
    }
    // else set outfile?

    Encode::InitEncoders();

    if ( infile != NULL ) {
        inf.assign(infile);
        ::free(infile);
    }
    if ( outfile != NULL ) {
        outf.assign(outfile);
        ::free(outfile);
    }
    if ( type != NULL ) {
    	enctype = setEncodingType(type);
        ::free(type);
    }
    if ( br != NULL ) {
        rate = StringUtils::fromString<uint16_t>(br);
        ::free(br);
    }

    if ( enctype < 2 && inf.empty() && ! decode ) {
        std::cout << "Error encoding type not provided or detected" << std::endl;
        usage();
    }

    if ( ! path.empty() )
    {
        Decode  decoder;

        decoder.debug(verbose);
        decoder.dryrun(dryrun);
        decoder.notags(notags);
        decoder.clobber(clobber);

        FileList   wavs;
        FileList::iterator fIter;

        if ( ! decoder.decodePath(path, wavs) ) {
            std::cout << "Error reading files" << std::endl;
            return -1;
        }

        if ( ! decode )
        {
        	Encode encoder(enctype, rate);

        	encoder.debug(verbose);
        	encoder.dryrun(dryrun);

        }

    }
    else
    {
    }


    std::cout << "Finished." << std::endl;

    return 0;
}

