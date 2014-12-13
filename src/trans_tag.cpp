#define _TRANSAM_TRANSTAG_CPP_

extern "C" {
# include <unistd.h>
# include <getopt.h>
# include <string.h>
# include <errno.h>
}

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "transam.h"
#include "TransFile.h"
#include "Encode.h"
using namespace transam;

#include "FileUtils.h"
#include "StringUtils.h"
using namespace tcanetpp;

static const char Process[] = "transtag";
static const char Version[] = "0.3.1";



void version()
{
    printf("%s version: v%s\n", Process, Version);
    exit(0);
}


void usage()
{
    std::cout << "Usage: " << Process << " [-hnvV] [-t type] [-T tagname:value|...] <file|path>" << std::endl
              << "  Supports setting the standard ID3/4 tags, such as: " << std::endl
              << "  ARTIST, ALBUMARTIST, TRACKNUMBER, TITLE, DISCNUMBER, COMMENT, DATE, GENRE" << std::endl
			  << "  (eg. -T \"ARTIST:The Animals|ALBUM:Best Of|TRACKNUMBER:14|TITLE:It's My Life\"" << std::endl;
    exit(0);
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

bool setTags ( TransFile & tf, StringList & tags )
{

    std::string key, val;
    int         indx;

    StringList::iterator  sIter;

    for ( sIter = tags.begin(); sIter != tags.end(); ++sIter )
    {
        std::string & tag = *sIter;
        indx = StringUtils::indexOf(tag, ":");

        if ( indx < 0 ) {
            std::cout << " Tag format invalid. Should be 'key:val'" << std::endl;
            return false;
        }

        key  = tag.substr(0, indx);
        val  = tag.substr(indx+1);

        std::cout << "  Setting " << key << " : " << val << std::endl;

        if ( ! tf.setTag(key, val) ) {
            std::cout << "Error setting tag for " << tf.getFileName() << std::endl;
            return false;
        }
    }

    tf.saveTags();

    return true;
}


int main ( int argc, char **argv )
{
    char optChar;
    char *tagstr    = NULL;
    char *typestr   = NULL;
    int  optindx    = 0;
    
    encoding_t type = AUDIO_UNK;

    static struct option l_opts[] = { {"help",    no_argument, 0, 'h'},
                                      {"type",    required_argument, 0, 't'},
                                      {"tag",     required_argument, 0, 'T'},
                                      {"version", no_argument, 0, 'V'},
                                      {0,0,0,0}
                                    };


    while ( (optChar = ::getopt_long(argc, argv, "ht:T:V", l_opts, &optindx)) != EOF )
    {
        switch ( optChar ) {
            case 'h':
              usage();
              break;
            case 't':
              typestr = strdup(optarg);
              break;
            case 'T':
              tagstr = strdup(optarg);
              break;
            case 'V':
              version();
              break;
            default:
              // path
              break;
        }
    }

    std::string    tags, target;
	StringList     taglist;
    TransFileList  files;

    TransFileList::iterator  fIter;
    
    if ( optind == argc ) {
        std::cout << "No target defined." << std::endl;
        usage();
    }

    target = argv[optind];
    if ( target.empty() ) {
        std::cout << "No target defined." << std::endl;
        usage();
    }

    if ( tagstr != NULL ) {
        tags.assign(tagstr);
        ::free(tagstr);
    }

    if ( typestr != NULL ) {
        type = setEncodingType(typestr);
        ::free(typestr);
    }

    if ( tags.empty() ) {
        listTags(target, AUDIO_UNK);
        return 0;
    }

	StringUtils::split(tags, '|', back_inserter(taglist));

    // operate tags on all files in directory (of the given encoding type if applicable)
    if ( FileUtils::IsDirectory(target) )
    {
        if ( ! TransFile::ReadPath(target, files) ) {
            std::cout << "ERROR reading files from path '" << target << "'" << std::endl;
            return -1;
        }

        files.sort();
    }
    else if ( FileUtils::IsReadable(target) )
    {
    	TransFile ta(target, TransFile::GetEncoding(target));
    	ta.readTags();
    	files.push_back(ta);
    }

    for ( fIter = files.begin(); fIter != files.end(); ++fIter )
    {
        TransFile & tf = (TransFile&) *fIter;

        std::cout << "File: " << tf.getFileName() << std::endl
        		  << "  Current Tag: ";
        tf.printTags();

        if ( type > AUDIO_UNK && type == tf.type() )
            setTags(tf, taglist);
        else if ( type == AUDIO_UNK )
            setTags(tf, taglist);

        std::cout << "  Updated Tag: ";
        tf.printTags();
    }

    return 0;
}


