transam - Transcoding Audio Media
=====================================

A tool for performing transcoding between various audio formats. This makes
use of external tools for the encoding/decoding of audio files and also uses
taglib for setting tags and/or keeping the metadata/tags intact while transcoding.

#### Build Requirements

  * taglib  (https://github.com/taglib/taglib)
  * tcanetpp (https://github.com/tcarland/tcanetpp.git)

#### External Tools

TransAm wraps the use of common audio tools that already exist to decode and
encode various audio formats. For this purpose, we choose the following tools
available in all major Linux distributions.

  * LAME    - for mp3 files (http://lame.sf.net)
  * FLAC    - Free Lossless Audio Compression
  * Vorbis  - for ogg support
  * NeroAAC - for mp4/aac (better quality than libfaac)
  * Shorten - optional for shn file support

These tools should be available via the system PATH environment variable.
TransAm has been tested with the following versions:

 * LAME 64bits version >=3.99.5
 * NeroAac v1.5.1
 * Flac 1.3.2
 * Vorbis Tools 1.4.0
