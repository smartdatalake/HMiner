#!/bin/bash
cd "$(dirname "$0")"

config="$1"

if ! ./run -c "$config"; then
        echo "Error: Association Mining"
        exit 1
fi

python3 pagerank.py -c "$config"
retVal=$?
if [ $retVal -eq 4 ]; then
        echo "No results found"
        exit 4
elif [ $retVal -ne 0 ]; then
        echo "Error: Ranking"
        exit 2
fi

if ! python3 add_names.py -c "$config"; then 
        echo "Error: Finding node names"
        exit 3
fi
