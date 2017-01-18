transam - Transcoding for Audio Media
=====================================

  A tool for performing transcoding between supported audio formats. This makes use of external tools for encoding/decoding of audio files, but additionally attempts to keep audio metadata tags.

 Build Requirements:

  * taglib
  * tcanetpp 

 External Tool Requirements:

  * lame  (for mp3 files)
  * NeroAAC (for mp4 files)
  * shorten (optional for shn files)
  * flac
  * vorbis tools

These tools should be available via the PATH environment variable.
This has been tested with the following versions:

 * LAME 64bits version 3.99.5 (http://lame.sf.net)
 * NeroAac v1.5.1
 * Flac 1.3.1
 * Vorbis Tools 1.4.0

