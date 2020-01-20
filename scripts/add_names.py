import sys
import csv

if len(sys.argv) != 4: 
    print("Usage: python3 add_names.py <input_file_with_names> <input_file_with_pagerank_results> <output_file>")
    sys.exit(-1)
    
input_idx = sys.argv[1]
input_pr = sys.argv[2]
outfile = sys.argv[3]

# assign names to ids 
names = {}
with open(input_idx) as fp:
    line = fp.readline()

    while line:
        line = line.rstrip()
        parts = line.strip().split("\t")
        names[parts[0]] = parts[1]
        line = fp.readline()


# read pagerank resuls and append names
with open(outfile, 'w', newline='') as csvfile:
    filewriter = csv.writer(csvfile, delimiter='\t', quotechar='"', quoting=csv.QUOTE_MINIMAL)

    with open(input_pr) as fd:
        
        for line in fd:

            line = line.rstrip()
            parts = line.split("\t")

            name = 'Unknown'
            if parts[0] in names: 
                name = names[parts[0]]
            
            filewriter.writerow([parts[0], name, parts[1]])