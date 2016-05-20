#!/bin/sh
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cat "$1" | python "$DIR"/process.py > "$DIR"/a.py && \
python "$DIR"/a.py <&0
