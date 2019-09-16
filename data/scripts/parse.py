paper_ids = {}

with open("nodes/papers.csv", "r") as ins:
    for line in ins:
        tokens = line.split("\t")
        paper_ids[tokens[1]] = tokens[0]

# print(paper_ids)
with open("relations/PP.csv", "r") as ins:
    for line in ins:
        tokens = line.split("\t")
        # paper_ids[tokens[1]] = tokens[0]
        print(paper_ids[tokens[0].rstrip()] + "\t" + paper_ids[tokens[1].rstrip()])
