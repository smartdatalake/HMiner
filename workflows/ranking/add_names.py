import sys
import csv
import json

with open(sys.argv[2]) as config_file:
    config = json.load(config_file)
    metapath = config["query"]["metapath"]
    nodes_dir = config["indir"]
    outfile = config["final_out"]
    input_pr = config["analysis_out"]
    first_entity = metapath[:1]

input_idx = nodes_dir + first_entity + ".csv"

# assign names to ids 
names = {}
with open(input_idx) as fp:
    line = fp.readline()

    while line:
        line = line.rstrip()
        parts = line.strip().split("\t")
        names[parts[0]] = parts[1]
        line = fp.readline()


# read pagerank resuls from stdin and append names
with open(outfile, 'w', newline='') as csvfile:
    filewriter = csv.writer(csvfile, delimiter='\t', quotechar='"', quoting=csv.QUOTE_MINIMAL)

    with open(input_pr) as fd:
        
        for line in fd:
        # for line in sys.stdin:

            line = line.rstrip()
            parts = line.split("\t")

            name = 'Unknown'
            if parts[0] in names: 
                name = names[parts[0]]
            
            filewriter.writerow([parts[0], name, parts[1]])
