import sys
import numpy as np
from scipy import sparse
from fast_pagerank import pagerank_power
import csv
import json

if len(sys.argv) != 3:
    print("Usage: python3 pagerank.py -c <config_file>")
    sys.exit(-1)

with open(sys.argv[2]) as config_file:
    config = json.load(config_file)
    inputfile = config["hin_out"]
    alpha = config["pr_alpha"]
    tol = config["pr_tol"]
    outfile = config["ranking_out"]

# load adjacency matrix from file
print("Ranking\t1\tLoading Adjacency Matrix")
A = np.loadtxt(inputfile, delimiter='\t', usecols={0, 1}, dtype=np.int32)
W = np.loadtxt(inputfile, delimiter='\t', usecols={2}, dtype=np.float64)

if (A.size <= 0):
    # create empty file
    open(outfile, 'a').close()
    sys.exit(0)

# normalize edge weights
total_sum = np.sum(W)
W /= total_sum

# calculate max dimension
print("Ranking\t2\tTransforming Adjacency Matrix") 
N = max(np.amax(A[:,0]), np.amax(A[:,1])) + 1

# transform to sparse matrix representation
G = sparse.csr_matrix((W, (A[:,0], A[:,1])), shape=(N, N))

# run pagerank
print("Ranking\t3\tRunning Ranking")
PR = pagerank_power(G, p=alpha, tol=tol)

# sorting output
print("Ranking\t4\tSorting Results")
sorted_indices = np.argsort(PR)[::-1][:len(PR)]

# filter out indices that are not in A  
indices = np.unique(np.concatenate((A[:,0], A[:,1])))
sorted_indices = sorted_indices[np.isin(sorted_indices, indices)]

# write results to output file
print("Ranking\t5\tWriting Results")
with open(outfile, 'w', newline='') as csvfile:
    filewriter = csv.writer(csvfile, delimiter='\t', quotechar='"', quoting=csv.QUOTE_MINIMAL)
    for i in sorted_indices:
        filewriter.writerow([i, PR[i]])
