import sys
import numpy as np
from scipy import sparse
# from fast_pagerank import pagerank
from fast_pagerank import pagerank_power
import csv

if len(sys.argv) != 5:
    print("Usage: python3 pagerank.py <input_file> <alpha> <tolerance> <output_file>")
    sys.exit(-1)

inputfile = sys.argv[1]
alpha = float(sys.argv[2])
tol = float(sys.argv[3])
outfile = sys.argv[4]

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
