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
print("1\tLoading input adjacency matrix")
A = np.loadtxt(inputfile, delimiter='\t', usecols={0, 1}, dtype=np.int32)

# calculate max dimension
print("2\tTransforming adjacency matrix")
N = max(np.amax(A[:,0]), np.amax(A[:,1])) + 1

# initialize weights
W = np.full(len(A), 1 / len(A))

# transform to sparse matrix representation
G = sparse.csr_matrix((W, (A[:,0], A[:,1])), shape=(N, N))

# run pagerank
print("3\tExecuting PageRank")
PR = pagerank_power(G, p=alpha, tol=tol)

# sorting output
print("4\tSorting results")
sorted_indices = np.argsort(PR)[::-1][:len(PR)]

# write results to output file
print("5\tWriting results to file")
with open(outfile, 'w', newline='') as csvfile:
    filewriter = csv.writer(csvfile, delimiter='\t', quotechar='"', quoting=csv.QUOTE_MINIMAL)
    for i in sorted_indices:
        filewriter.writerow([i, PR[i]])
        # print(str(i) + "\t" + str(PR[i]))
