#define _TRANSAM_SHOWTAGS_CPP_

#include <cstdio>

#include "transam.h"
#include "TransFile.h"
using namespace transam;


static const char Process[] = "showtags";

void usage()
{
    printf("Usage: %s <path>\n", Process);
    exit(0);
}

int main ( int argc, char **argv )
{
    if ( argc == 1 )
        usage();

    std::string path = argv[1];

    if ( path.empty() ) 
        usage();

    TransFileList  files;
    TransFileList::iterator  fIter;

    if ( ! TransFile::ReadPath(path, files, false) )
        std::cout << "Error reading path." << std::endl;

    for ( fIter = files.begin(); fIter != files.end(); ++fIter )
        fIter->printTags();

    return 0;
}


