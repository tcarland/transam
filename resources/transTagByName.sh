#!/usr/bin/env/bash
#
#  Tag a given file(s) Song Title by the file name.
#
PNAME=${0##*\/}
VERSION="v23.11"

files="$@"
exts=("mp3" "flac" "wav")
dryrun=0
yes=0


usage="
Adds song title metadata tags to audio files based on the filename.
This expects the filename in the format of 'str-name.ext' where 
'str' is some string such as trackno. and 'name' is the song title.

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
    title="${title#*-}"
    title=$(insert_spaces "$title")
    echo "$title"
}


# Main
rt=0

while [$# -gt 0 ]; do
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

files=()
for f in $(ls -1 "$path"); do
    if [ -f "$f" ]; then
        ext="${f##*.}"
        if [[ " ${exts[@]} " =~ " ${ext} " ]]; then
            files+=("$f")
        fi
    fi
done


for f in $files; do
    title=$(get_title_string "$f")
    if [ -z "$title" ]; then
        echo "$PNAME Error getting title from '$f'" >&2
        rt=1
        break
    fi
    echo "Tagging $f with 'TITLE:$title'"
    if [ $dryrun -eq 1 ]; then
        echo "transam -A -T 'TITLE:$title' $f"
        continue
    fi
    if [ $yes -eq 0 ]; then
        ask "Tag $f with 'TITLE:$title'?" || continue
    fi

    echo "tag it"
    #transam -A -T "TITLE:$title" "$f"
    #rt=$?
    #if [ $rt -ne 0 ]; then
    #    echo "$PNAME Error tagging '$f'" >&2
    #    break
    #fi
done

exit $rt
