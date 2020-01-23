#!/bin/bash
cd "$(dirname "$0")"

config="config.json"
./run -c "$config"
python3 pagerank.py -c "$config" 
python3 add_names.py -c "$config"