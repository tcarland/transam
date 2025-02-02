#!/usr/bin/env bash
#
#  Automate the setting of metadata 'TITLE' idTag of audio files 
#  by file name convention.
#
PNAME=${0##*\/}
VERSION="v25.02.01"

files=()
path=
exts=("mp3" "flac" "wav")
dryrun=0
setall=0
yes=0

artist=
album=
comment=

# -----------------------------------------------------------

usage="
$PNAME $VERSION
Adds media 'TITLE' metadata tags to files based on the filename.
This expects the filename in the format of 'artist-trackno-name.ext' 
where 'name' is the media title (ie. song name).

This is useful for first setting individual title names resulting 
directly from raw media file importing. 

Synopsis:
  $PNAME [options] <path>

Options:
  -A, --artist <artist>    Set the 'ARTIST' metadata tag to <artist>
  -a, --album <album>      Set the 'ALBUM' metadata tag to <album>
  -c, --comment <comment>  Set the 'COMMENT' metadata tag to <comment>
  -h, --help               Print this help message and exit.
  -n, --dry-run            Enables Dry Run, no changes are made.
  -S, --set-all            Additionally set Artist and Album tags 
                           from the filename format. 
  -y, --yes                Do not prompt before executing commands.
  -V, --version            Print the version number and exit.

  <path>                   Path to the file(s) to tag. 
"

# -----------------------------------------------------------

ask()
{
    local prompt="y/n"
    local default=
    local REPLY=

    while true; do
        if [ "${2:-}" = "Y" ]; then
            prompt="Y/n"
            default="Y"
        elif [ "${2:-}" = "N" ]; then
            prompt="y/N"
            default="N"
        fi

        read -p "$1 [$prompt] " REPLY

        if [ -z "$REPLY" ]; then
            REPLY="$default"
        fi

        case "$REPLY" in
            Y*|y*) return 0 ;;
            N*|n*) return 1 ;;
        esac
    done
}


insert_spaces() {
    str=$(echo "$1" | sed 's/\([a-z]\)\([A-Z]\)/\1 \2/g')
    str=$(echo "$str" | sed 's/\([A-Z]\)\([A-Z]\)/\1 \2/g')
    echo "$str"
}

get_title_string() {
    local str="$1"
    title="${str%.*}"
    title="${title##*-}"
    title=$(insert_spaces "$title")
    echo "$title"
}

count_dashes() {
    local str="$1"
    local count=$(echo "$str" | grep -o '-' | wc -l)
    echo $count
}

# -----------------------------------------------------------

# Main
rt=0

while [ $# -gt 0 ]; do
    case "$1" in
        -A|--artist)
            artist="$2"
            shift
            ;;
        -a|--album)
            album="$2"
            shift
            ;;
        -c|--comment)
            comment="$2"
            shift
            ;;
        -h|--help)
            echo "$usage"
            exit 0
            ;;
        -n|--dryrun|--dry-run)
            echo " > DRYRUN enabled"
            dryrun=1
            ;;
        -S|--set*) 
            setall=1
            ;;
        -y|-Y|--y*|--Y*)
            yes=1
            ;;
        -V|--version)
            echo "$VERSION"
            exit 0
            ;;
        -*)
            echo "Unknown option: $1" >&2
            echo "$usage" >&2
            exit 1
            ;;
        *)
            path="$1"
            break
            ;;
    esac
    shift
done

if ! which transam >/dev/null 2>&1; then
    echo "$PNAME Error, 'transam' not found in PATH." >&2
    exit 3
fi

if [ -z "$path" ]; then
    echo "$usage"
    exit 1
fi

if [ ! -d "$path" ]; then
    echo "$PNAME Error, '$path' is not a directory." >&2
    echo ""
    echo "$usage"
    exit 1
fi

# -----------------------------------------------------------

for f in $(ls -1 "$path"); do
    if [ -f $path/$f ]; then
        ext="${f##*.}"
        if [[ " ${exts[@]} " =~ " ${ext} " ]]; then
            files+=("$f")
        fi
    fi
done

if [ ${#files[@]} -eq 0 ]; then
    echo "$PNAME Error, no supported audio files found in '$path'." >&2
    exit 2
fi

settags=0
tagstr=""

if [[ -n "$artist" || -n "$album" || -n "$comment" || $setall -eq 1 ]]; then
    settags=1
fi

for file in ${files[@]}; do
    title=$(get_title_string "$file")

    if [ -z "$title" ]; then
        echo "$PNAME Error getting title from '$file'" >&2
        rt=1
        break
    fi

    tagstr="TITLE:$title"
    cnt=$(count_dashes "$file")

    if [ $settags -eq 1 ]; then
        if [ -n "$artist" ]; then
            tagstr="${tagstr}|ARTIST:${artist}"
        elif [[ $setall -eq 1 && $cnt -eq 3 ]]; then
            artist=$(echo "$file" | cut -d'-' -f1)
            artist=$(insert_spaces "$artist")
            tagstr="${tagstr}|ARTIST:${artist}"
        fi
        if [ -n "$album" ]; then
            tagstr="${tagstr}|ALBUM:${album}"
        elif [[ $setall -eq 1 && $cnt -eq 3 ]]; then
            album=$(echo "$file" | cut -d'-' -f2)
            album=$(insert_spaces "$album")
            tagstr="${tagstr}|ALBUM:${album}"
        fi
        if [ -n "$comment" ]; then
            tagstr="$tagstr|COMMENT:$comment"
        fi
    fi

    if [ $yes -eq 0 ]; then
        echo ""
        ask "Tag '$file' with '$tagstr'?" "Y" || continue
    fi
    
    echo " > transam -A -T '$tagstr' $path/$file"
    if [ $dryrun -eq 1 ]; then
        continue
    fi
    
    transam -A -T "$tagstr" "$path/$file"

    rt=$?
    if [ $rt -ne 0 ]; then
        echo "$PNAME Error tagging '$file'" >&2
        break
    fi
done

exit $rt
