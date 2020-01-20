#!/bin/bash

query_file="data/DBLP_sample_dense/queries.txt"
nodes_dir="data/DBLP_full/nodes/"
relations_dir="data/DBLP_full/relations/"
names_file="data/DBLP_full/nodes/A.csv"
algorithm="DynP"
hin_file="data/out/HIN.csv"
ranking_file="data/out/PR.csv"
final_result="data/out/PR_final.csv"
alpha="0.5"
tol="0.00000000001"

make all
./run -qf "$query_file" -indir "$nodes_dir" -irdir "$relations_dir" -algo "$algorithm" -out "$hin_file"
python3 scripts/pagerank.py "$outfile" "$alpha" "$tol" "$ranking_file"
python3 scripts/add_names.py "$names_file" "$ranking_file" "$final_result"