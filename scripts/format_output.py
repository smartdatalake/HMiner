import sys

filepath = sys.argv[1]

dict = {}

for i in range(0, 10000):
   dict[str(i)] = []

with open(filepath) as fp:
    line = fp.readline()
    cnt = 1
    while line:
        # print("Line {}: {}".format(cnt, line.strip()))
        parts = line.strip().split("\t")
        if (parts[0] != parts[1]):

            # if (parts[0] not in dict):
            #     dict[parts[0]] = []
            dict[parts[0]].append(parts[1])

        line = fp.readline()

# print(dict)

initial_score = 1 / len(dict)


for key in dict:
    length = len(dict[key])
    values = ",".join(dict[key])
    if length == 0:
        values = "0"

    print(key + "\t" + values + "|" + str(length) + "|" + str(initial_score) + "\t0\t0")