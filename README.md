TransAM - Transcoding Audio Media
=====================================

*TransAM* is a tool for transcoding between a variety audio formats 
while tracking or manipulating the audio metadata, ie. id3 tags. 
Namely the *mp3*, *flac*, *aac*, and Vorbis *ogg* formats are supported.
It makes use of existing (*FOSS*) audio tools for the decoding and/or 
encoding of files.

<br>

<notice>
*
* Copyright (c) 2010-2026 Timothy Charlton Arland <tcarland@gmail.com>
*
* This file is part of TransAM.
* 
* TransAM is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* TransAM is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with TransAM.  If not, see <https://www.gnu.org/licenses/>.
* 
</notice>

---

## Overview

*TransAM* wraps various audio tools with sane defaults as well as keeping
tags/metadata intact across the transcoding process. It will keep tags 
intact while transcoding, or manipulate tags as defined by the user via 
[taglib](https://github.com/taglib/taglib). 
The tool is intended to operate on directories of audio files and will
decode, transcode (decode *and* encode), or edit tags.

## Audio Tools

The audio formats are supported via existing opensource tools:

- **LAME** for *mp3* files (http://lame.sf.net)
- **FLAC** or Free Lossless Audio Compression (https://github.com/xiph/flac)
- **libvorbis** for *ogg* support. (https://xiph.org/vorbis/)
- **ffmpeg** for *aac* support. (https://ffmpeg.org/)
- **shorten** is a legacy lossless format that is optionally supported 
  for decoding only.

Additionally, support for the *Nero AAC encoder/decoder* is provided for 
legacy purposes as AAC support and quality has historically varied in the 
underlying *libfaac* or *libavcodec* libraries used by *ffmpeg*, while the 
Nero implementation still proves to be stable and efficient in providing 
quality AAC files. Of note, the ffmpeg aac encoder has improved since the 
project was originally created and is now the default encoder for mp4/aac 
files. To prefer the use of the Nero Encoder, pass the `--no-ffmpeg` option.

## Build Requirements

*TransAM* makes use of the *tcanetpp* library for tree data-structure 
and various String and File utilities. Both *TransAM* and *tcanetpp* make 
use of the *tcamake* project for providing the build environment. 
  
- tcamake (https://github.com/tcarland/tcamake.git)  
- tcanetpp (https://github.com/tcarland/tcanetpp.git)  

The *taglib* library can installed via the systems package manager as 
detailed below.

<br>

---

## External Codec support / dependencies

The various tools are expected to be available via the system PATH.
Nearly all tools (save NeroAAC) are available via distribution package 
managers. These packages should be installed accordingly.

RHEL (dnf/yum) distributions, package names:
- flac
- lame
- vorbis-tools
- libvorbis-devel
- ffmpeg
- taglib-devel

Debian/Ubuntu (apt) distributions, package names:
- lame
- flac
- vorbis-tools
- libvorbis-dev
- ffmpeg
- libtag1-dev


## NeroAAC

NeroAAC is a proprietary codec that is no longer maintained, but 
remains a stable and efficient option as an AAC encoder/decoder (and 
possibly still better than libfaac or ffmpeg). It can still be acquired separately 
from the following locations:
- http://wiki.hydrogenaud.io/index.php?title=Nero_AAC
- https://web.archive.org/web/20160923100008/http://ftp6.nero.com/tools/NeroAACCodec-1.5.1.zip


### Nero vs FFMPEG

To prefer **NeroAAC** over **ffmpeg**,  use the `-F` or `--no-ffmpeg` option.
*ffmpeg* is the default encoder/decoder for *AAC* files. Note that Nero
will not encode raw pcm audio.


## Codec Versions

*TransAm* has been tested with the following versions:

- LAME 64 >=3.100
- Flac 1.3.3
- Vorbis Tools 1.4.0
- FFmpeg 4.4.2
- NeroAAC v1.5.1


## Examples

Transcode the files in the current directory to mp3 with a constant bitrate of 256k,
and keepting tags intact.
```sh
transam -b 256 -t mp3 .
```

Set the Artist and Album name for tracks in the current directory only.
```sh
transam -A -T "ARTIST:Grateful Dead|ALBUM:Aoxomoxoa" .
```

Decode the input files to wav pcm and do not re-encode.
```sh
transam -d -r .
```

Transcode the input files to AAC, but leave the decoded WAV files intact. Overwrite 
any existing files.
```sh
transam -t m4a --no-erase --clobber .
```

Renumber the tracks in a given directory to be sequential (based on sorted filenames).
```sh
transam -L
transam -R .
transam -L
```

## Tagging by filename

The script *resources/transetTagByName.sh* will attempt to tag the media title from 
the filename. Useful when dealing with raw recordings such as those pulled in by 
Ardour or other multi-track recording apps. This requires a file-naming convention of 
*name-trackno-title.ext*. For example, *HeavenlyHana-s1t01-HanaChant.wav*, would 
result in the TITLE tag being set to `Hana Chant`.
