paper_ids = {}

with open("../DBLP_sample_dense/nodes/P.csv", "r") as ins:
    for line in ins:
        tokens = line.split("\t")
        paper_ids[tokens[1]] = tokens[0]

# print(paper_ids)
with open("/home/serafeim/Desktop/refs.csv", "r") as fp:

    line = fp.readline()

    while (line):
        line = line.rstrip()
        tokens = line.split("\t")
        if ( (tokens[0] in paper_ids) and (tokens[1] in paper_ids) ):
            print(paper_ids[tokens[0]] + "\t" + paper_ids[tokens[1]])

        line = fp.readline()