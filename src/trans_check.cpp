#define _TRANSAM_TRANSCHECK_CPP_

extern "C" {
# include <unistd.h>
# include <getopt.h>
# include <string.h>
# include <errno.h>
}

#include <cstdio>
#include <iostream>

#include "transam.h"
using namespace transam;

#include "CmdBuffer.h"
#include "StringUtils.h"
using namespace tcanetpp;


static const char Process[] = "transcheck";


bool isValid ( std::string & result )
{
    if ( StringUtils::startsWith(result, "/") )
        return true;
    return false; 
}

void check ( std::string & encoder )
{
    CmdBuffer    cmdbuf(4096);
    std::string  cmd, line;

    cmd  = "which ";
    cmd.append(encoder);

    if ( ! cmdbuf.Open(cmd) ) {
        std::cout << "Error in cmdbuf.open()" << std::endl;
        return;
    }

    line = cmdbuf.getLine();

    if ( isValid(line) )
        std::cout << "   [OK]" << std::endl;
    else
        std::cout << " MISSING!" << std::endl;
}


int main ( int argc, char **argv )
{
    std::cout << "Checking for the MP3 Encoder: " << MP3_ENCODER;
    check(MP3_ENCODER);

    std::cout << "Checking for the MP4 Encoder: " << MP4_ENCODER;
    check(MP4_ENCODER);
    
    std::cout << "Checking for the FLAC Encoder: " << FLAC_ENCODER;
    check(FLAC_ENCODER);

    std::cout << "Checking for the OGG Encoder: " << OGG_ENCODER;
    check(OGG_ENCODER);
    
    std::cout << "Checking for the SHN Decoder: " << SHN_DECODER;
    check(SHN_DECODER);
    
    return 0;
}




