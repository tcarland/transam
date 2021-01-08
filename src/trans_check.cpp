#define _TRANSAM_TRANSCHECK_CPP_

#include <iostream>
#include <iomanip>

#include "transam.h"
using namespace transam;

#include "util/CmdBuffer.h"
#include "util/StringUtils.h"
using namespace tcanetpp;



bool isValid ( std::string & result )
{
    if ( StringUtils::StartsWith(result, "/") )
        return true;
    return false; 
}


bool check ( tcanetpp::CmdBuffer & cmdbuf, const std::string & exec )
{
    std::string  cmd, line;
    bool         result = false;

    cmd  = "which ";
    cmd.append(exec);

    if ( ! cmdbuf.open(cmd) ) {
        std::cout << "Error in cmdbuf.open()" << std::endl;
        return false;
    }

    line = cmdbuf.getLine();

    if ( (result = isValid(line)) )
        std::cout << "   [OK]" << std::endl;
    else
        std::cout << " MISSING!" << std::endl;

    cmdbuf.close();

    return result;
}


int main ( int argc, char **argv )
{
    tcanetpp::CmdBuffer  cmdbuf;
    bool  r, f = true;

    std::cout << std::endl;
    std::cout << std::setw(32)<< std::setiosflags(std::ios_base::left)
              << "Checking for the MP3 Encoder: " << std::setw(12) << MP3_ENCODER;
    r = check(cmdbuf, MP3_ENCODER);
    if ( ! r ) f = r;

    std::cout << std::setw(32) << "Checking for the MP4 Encoder: "
              << std::setw(12) << MP4_ENCODER;
    r = check(cmdbuf, MP4_ENCODER);
    if ( ! r ) f = r;

    std::cout << std::setw(32) << "Checking for the AAC Encoder: "
              << std::setw(12) << AAC_ENCODER;
    r = check(cmdbuf, AAC_ENCODER);
    
    std::cout << std::setw(32) << "Checking for the FLAC Encoder: "
              << std::setw(12) << FLAC_ENCODER;
    r = check(cmdbuf, FLAC_ENCODER);
    if ( ! r ) f = r;

    std::cout << std::setw(32) << "Checking for the OGG Encoder: "
              << std::setw(12) << OGG_ENCODER;
    r = check(cmdbuf, OGG_ENCODER);
    if ( ! r ) f = r;
    
    std::cout << std::setw(32) << "Checking for the SHN Decoder: "
              << std::setw(12) << SHN_DECODER;
    r = check(cmdbuf, SHN_DECODER);
    if ( ! r ) f = r;

    if ( ! f )
        std::cout << "Some apps are missing, ensure the appropriate packages are "  << std::endl
                  << "installed and in your system path if you plan to work with " << std::endl
                  << "that given format." << std::endl;
    else
        std::cout << "All Apps found!" << std::endl;
    
    return 0;
}
