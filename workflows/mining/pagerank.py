import sys
import numpy as np
from scipy import sparse
from fast_pagerank import pagerank_power
import csv
import json
import subprocess
import utils

if len(sys.argv) != 3:
    print("Usage: python3 pagerank.py -c <config_file>")
    sys.exit(-1)

with open(sys.argv[2]) as config_file:

    config = json.load(config_file)
    if "ranking" not in config:
        sys.exit(0)

    inputfile = config["hin_out"]
    alpha = config["ranking"]["pr_alpha"]
    tol = config["ranking"]["pr_tol"]
    outfile = config["ranking"]["ranking_out"]
    metapath = config["query"]["metapath"]
    threshold = config["ranking"]["threshold"]
    nodes_dir = config["indir"]
    src_field = config["query"]["src_field"]
    build_index = config["query"]["buildIndex"]

# Do not execute PR when no symmetric metapath is given
if (metapath[0] != metapath[-1] or build_index == "true"):
    sys.exit(0)

# filter inputfile for a specific threshold
# tmp_file = inputfile + ".tmp"
# command = "awk ' $3 >= " + str(threshold) +" ' " + inputfile + " > " + tmp_file + " && mv " + tmp_file + " " + inputfile
# process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE)
# process.wait()

# load adjacency matrix from file
print("Ranking\t1\tLoading Adjacency Matrix", flush=True)
A = np.loadtxt(inputfile, delimiter='\t', usecols={0, 1}, dtype=np.int32)
W = np.loadtxt(inputfile, delimiter='\t', usecols={2}, dtype=np.float64)

if (A.size <= 2):
    # create empty file
    open(outfile, 'a').close()
    sys.exit(0)

# normalize edge weights
total_sum = np.sum(W)
W /= total_sum

# calculate max dimension
print("Ranking\t2\tTransforming Adjacency Matrix", flush=True) 
N = max(np.amax(A[:,0]), np.amax(A[:,1])) + 1

# transform to sparse matrix representation
G = sparse.csr_matrix((W, (A[:,0], A[:,1])), shape=(N, N))

# run pagerank
print("Ranking\t3\tExecuting Ranking Algorithm", flush=True)
PR = pagerank_power(G, p=alpha, tol=tol)

# sorting output
print("Ranking\t4\tSorting Results", flush=True)
sorted_indices = np.argsort(PR)[::-1][:len(PR)]

# filter out indices that are not in A  
indices = np.unique(np.concatenate((A[:,0], A[:,1])))
sorted_indices = sorted_indices[np.isin(sorted_indices, indices)]

# write results to output file
print("Ranking\t5\tWriting Results", flush=True)
idx_file = nodes_dir + metapath[0] + ".csv"
idx = utils.read_data_file(idx_file, src_field)

with open(outfile, 'w', newline='') as csvfile:
    filewriter = csv.writer(csvfile, delimiter='\t', quotechar='"', quoting=csv.QUOTE_MINIMAL)
    for i in sorted_indices:
        filewriter.writerow([idx[str(i)], PR[i]])
