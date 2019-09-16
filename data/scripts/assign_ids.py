
filename = "/home/serafeim/Desktop/data/small_nodes/P.csv"

ids = {}
with open(filename, 'r') as f:
    for line in f:

        tokens = line.rstrip().split('\t')
        ids[tokens[1].rstrip()] = tokens[0].rstrip()

filename = "/home/serafeim/Desktop/data/small_relations/AP.csv"
# print (ids)
with open(filename, 'r') as f:
    for line in f:
        tokens = line.rstrip().split('\t')
        print(tokens[0].rstrip() + "\t" + ids[tokens[1].rstrip()])
