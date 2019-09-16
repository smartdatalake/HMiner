
filename = "relations/AP.csv"
i = 0

with open(filename, 'r') as f:
	for line in f:
		i += 1
		if (i == 1): 
			continue
		i += 1

		# print(line)
		tokens = line.rstrip().split('\t')
		if (int(tokens[1]) < 20 and int(tokens[0]) < 22185):
			print(line.rstrip())
		# print(tokens)