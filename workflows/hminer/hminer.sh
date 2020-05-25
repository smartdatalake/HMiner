#!/bin/bash
cd "$(dirname "$0")"

config="$1"

if ! python3 check.py -c "$config"; then
        echo "Output files already exist"
        exit 0
fi

if ! ./run -c "$config"; then
        echo "Error: Association Mining"
        exit 1
fi

if ! python3 add_names.py -c "$config"; then 
        echo "Error: Finding node names"
        exit 2
fi

if ! python3 pagerank.py -c "$config"; then
        echo "Error: Ranking"
        exit 3
fi