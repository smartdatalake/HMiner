#!/bin/bash

query_file="data/DBLP_sample_dense/queries.txt"
nodes_dir="data/DBLP_sample_dense/nodes/"
relations_file="data/DBLP_sample_dense/relations/relations.crs"
algorithm="DynP"
outfile="data/out/"

make all
echo "./run -qf $query_file -indir $nodes_dir -irdir $relations_file -c data/constraints.txt -algo $algorithm -out $outfile"
./run -qf $query_file -indir $nodes_dir -irdir $relations_file -c data/constraints.txt -algo $algorithm -out $outfile
#python3 scripts/format_output.py data/out/APPA_DynP.csv > PaperRanking/PageRank/pagerank_input.csv
#cd ./PaperRanking/PageRank/
#python2 pagerank.py pagerank_input.csv 0.5 0.00000000001
#cd ../..
#bash scripts/sort_ranks.sh "./PaperRanking/PageRank/final_pagerank.txt" "./data/out/" 1 2 4
#python3 scripts/print_html.py ./data/DBLP_sample_dense/nodes/A.csv ./data/out/final_pagerank.txt > /data/www/HMiner/data/results.csv
