TransAm - Transcoding Audio Media
=====================================

*TransAm* is a tool for transcoding between a variety audio formats 
while tracking or manipulating the audio metadata (eg. id3 tags). 
Namely the *mp3*, *flac*, *aac*, and Vorbis *ogg* formats are supported.
It makes use of existing (*FOSS*) audio tools for the decoding and/or encoding 
of files.

<br>
---
## Overview

 *TransAm* wraps the various audio tools with sane defaults as well as keeping
tags/metadata intact across the transcoding process. It will keep tags intact 
while transcoding or manipulate tags as defined by the user using
 [taglib](https://github.com/taglib/taglib)
The tool is intended to operate on directories of audio files and will
decode only, transcode (decode/encode), or edit tags.

## Audio Tools

The audio formats are supported via various opensource tools:

- **LAME** for *mp3* files (http://lame.sf.net)
- **FLAC** or Free Lossless Audio Compression (https://github.com/xiph/flac)
- **libvorbis** for *ogg* support. (https://xiph.org/vorbis/)
- **ffmpeg** for *aac* support. (https://ffmpeg.org/)
- **shorten** is a legacy lossless format that is supported for decode only.

Additionally, support for the Nero AAC encoder/decoder is provided for 
legacy purposes as AAC support and quality has traditionally varied in the 
underlying *libfaac* or *libavcodec* libraries used by *ffmpeg*, while the 
Nero implementation still proves to be stable and efficient in providing 
quality AAC files. Of note, the ffmpeg aac encoder has improved since.


## Build Requirements

  TransAm makes use of the *tcanetpp* library for tree data-structure 
and various String and File utilities. Both TransAm and tcanetpp make 
use of *tcamake* for a build environment. 
  
  * tcanetpp (https://github.com/tcarland/tcanetpp.git)
  * tcamake (https://github.com/tcarland/tcamake.git)

  The *taglib* library can installed via the systems package manager.

<br>

---

## External Encoder support

The various tools are expected to be available via the system PATH.
Nearly all tools (save NeroAAC) are available via distribution package 
managers. These packages should be installed accordingly.

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

### NeroAAC

NeroAAC is a proprietary codec that is no longer maintained, but 
remains a stable and efficient option for an AAC encoder/decoder. It can 
still be acquired separately from the following location(s):
 - http://wiki.hydrogenaud.io/index.php?title=Nero_AAC
 - https://web.archive.org/web/20160923100008/http://ftp6.nero.com/tools/NeroAACCodec-1.5.1.zip

### Nero vs FFMPEG

To use **NeroAAC**, use a filename extension of *.mp4*. 
The extensions of *.m4a* or *.aac* will utilize **ffmpeg**.

## Codec Versions

*TransAm* has been tested with the following versions:

 * LAME 64 >=3.99.5
 * Flac 1.3.2
 * Vorbis Tools 1.4.0
 * FFmpeg 4.2.x
 * NeroAAC v1.5.1
