paper_ids = {}

with open("../DBLP_full/nodes/cc.csv", "r") as ins:
    for line in ins:
        tokens = line.split("\t")
        paper_ids[tokens[0]] = tokens[1]

# print(paper_ids)
with open("../DBLP_full/nodes/P.csv", "r") as fp:

    line = fp.readline()

    while (line):
        line = line.rstrip()
        tokens = line.split("\t")
        if ( (tokens[1] in paper_ids) ):
            print(tokens[0] + "\t" + tokens[1] + "\t" + tokens[3])
            # print(line + "\t" + paper_ids[tokens[1]])
            # print(paper_ids[tokens[0]] + "\t" + paper_ids[tokens[1]])

        line = fp.readline()