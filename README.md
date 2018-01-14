transam - Transcoding for Audio Media
=====================================

A tool for performing transcoding between supported audio formats. This makes 
use of external tools for the encoding/decoding of audio files, but allows for 
the setting of tags and/or keeping the metadata/tags intact.

#####Build Requirements:

  * taglib  (https://github.com/taglib/taglib)
  * tcanetpp (https://github.com/tcarland/tcanetpp.git)

#####External Tools: 

Transam simply wraps the usage of common audio tools that already decode and 
encode the various formats we need. For this we choose the following tools
available in all major linux distributions.

  * LAME    - for mp3 files (http://lame.sf.net)
  * NeroAAC - for mp4/aac
  * Shorten - optional for shn file support
  * FLAC    - Free Lossless Audio Compression
  * Vorbis  - for ogg

These tools should be available via the PATH environment variable.
This has been tested with the following versions:

 * LAME 64bits version 3.99.5 
 * NeroAac v1.5.1
 * Flac 1.3.1
 * Vorbis Tools 1.4.0

