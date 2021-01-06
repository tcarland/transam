TransAm - Transcoding Audio Media
=====================================

*TransAm* is a tool for transcoding between a variety audio formats. 
Namely *mp3*, *flac*, *aac*, and Vorbis *ogg*.  It makes use of 
existing (foss) audio tools for the decoding and/or encoding 
of files. 

 *TransAm* wraps these tools with sane defaults as well as tracking 
and maintaining any *metadata* (id tags) on the files as well as 
manipulating tags using the [taglib](https://github.com/taglib/taglib)
library. Features include support for listing tags/metadata, setting tags, 
setting tags while encoding, decode to `wav/PCM` only.

## Audio Tools

The audio formats are supported via various opensource tools:
- **LAME** for *mp3* files (http://lame.sf.net)
- **FLAC** or Free Lossless Audio Compression (https://github.com/xiph/flac)
- **libvorbis** for *ogg* support. (https://xiph.org/vorbis/)
- **ffmpeg** for *aac* support. (https://ffmpeg.org/)
- **shorten** is a legacy lossless format that is supported for decode only.

Additionally, support for the Nero AAC encoder/decoder is provided for 
legacy purposes as AAC support and quality has varied in the underlying 
*libfaac* or *libavcodec* libraries used by *ffmpeg*.


## Build Requirements

  TransAm makes use of the *tcanetpp* library for tree data-structure 
and various String and File utilities. Both TransAm and tcanetpp make 
use of *tcamake* for a build environment. 
  
  * tcanetpp (https://github.com/tcarland/tcanetpp.git)
  * tcamake (https://github.com/tcarland/tcamake.git)

  The *taglib* library can installed via the systems package manager.


## External Tools

The various tools should be installed and available via the system PATH 
environment variable using the system package manager.

RHEL/CentOS (yum) based distributions:
- flac
- lame
- vorbis-tools
- libvorbis-devel
- ffmpeg
- taglib-devel

Ubuntu (apt) based distributions:
- flac
- lame
- vorbis-tools
- libvorbis-dev
- ffmpeg
- libtag1-dev

## NeroAAC

NeroAAC is proprietary implementation that is no longer maintained, but 
remains a stable and highly efficient AAC encoder/decoder. It can still 
be acquired separately from 
 - http://wiki.hydrogenaud.io/index.php?title=Nero_AAC
 - https://web.archive.org/web/20160923100008/http://ftp6.nero.com/tools/NeroAACCodec-1.5.1.zip

## Versions

TransAm has been tested with the following versions:

 * LAME 64bits version >=3.99.5
 * Flac 1.3.2
 * Vorbis Tools 1.4.0
 * NeroAAC v1.5.1
 * FFmpeg 4.2.x
