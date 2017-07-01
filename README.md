transam - Transcoding for Audio Media
=====================================

A tool for performing transcoding between supported audio formats. This makes use of external tools 
for the encoding/decoding of audio files, but allows for the setting of tags and/or keeping the metadata/tags intact.

 Build Requirements:

  * taglib  (https://github.com/taglib/taglib)
  * tcanetpp (https://github.com/tcarland/tcanetpp.git)

 External Tool Requirements: 
   All tools, other than Nero, have packages available in all major linux distributions.

  * lame - for mp3 files (http://lame.sf.net)
  * NeroAAC - for mp4 files
  * shorten - optional for shn files
  * flac - Free Lossless Audio Compression
  * vorbis tools - for ogg

These tools should be available via the PATH environment variable.
This has been tested with the following versions:

 * LAME 64bits version 3.99.5 
 * NeroAac v1.5.1
 * Flac 1.3.1
 * Vorbis Tools 1.4.0

