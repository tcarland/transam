#!/usr/bin/env bash
#
#  Automate the setting of  metadata 'TITLE' idTag of audio files 
#  by file name convention.
#
PNAME=${0##*\/}
VERSION="v23.11"

files=()
path=
exts=("mp3" "flac" "wav")
dryrun=0
yes=0

# -----------------------------------------------------------

usage="
$PNAME $VERSION
Adds song title metadata tags to audio files based on the filename.
This expects the filename in the format of 'str-name.ext' where 
'str' is some string such as trackno. and 'name' is the song title.
This is useful for first setting individual title names resulting 
directly from raw media file importing. 

Synopsis:
  $PNAME [options] <path>

Options:
  -h, --help
    Print this help message and exit.
  -n, --dry-run
    Print the commands that would be executed, but do not execute them.
  -y, --yes
    Do not prompt before executing commands.
  -V, --version
    Print the version number and exit.

  <path>
    Path to the file(s) to tag. 
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


# -----------------------------------------------------------

# Main
rt=0

while [ $# -gt 0 ]; do
    case "$1" in
        -h|--help)
            echo "$usage"
            exit 0
            ;;
        -n|--dryrun|--dry-run)
            echo " > DRYRUN enabled"
            dryrun=1
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

for f in ${files[@]}; do
    title=$(get_title_string "$f")

    if [ -z "$title" ]; then
        echo "$PNAME Error getting title from '$f'" >&2
        rt=1
        break
    fi

    if [ $yes -eq 0 ]; then
        echo ""
        ask "Tag '$f' with 'TITLE:$title'?" "Y" || continue
    fi
    
    echo " > transam -A -T 'TITLE:$title' $path/$f"
    if [ $dryrun -eq 1 ]; then
        continue
    fi
    
    transam -A -T "TITLE:$title" "$path/$f"

    rt=$?
    if [ $rt -ne 0 ]; then
        echo "$PNAME Error tagging '$f'" >&2
        break
    fi
done

exit $rt
