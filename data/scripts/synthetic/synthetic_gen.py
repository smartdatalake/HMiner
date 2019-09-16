import random

nodes = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J']
print (nodes)

for i in range(10):
	i_cardinality = (int(i / 4) + 1) * 1500
	
	# write node files
	# fi = open("nodes/" + nodes[i] + ".csv", "w")
	# fi.write("id\n")
	# for k in range(i_cardinality):
	# 	fi.write(str(k) + "\n")
	# fi.close()
	
	# write relation files


	for j in range(10):
		if (i == j):
			break

		fij = open("relations_dense/" + nodes[i] + nodes[j] + ".crs", "w")
		fji = open("relations_dense/" + nodes[j] + nodes[i] + ".crs", "w")
		

		print(nodes[i] + nodes[j])
		j_cardinality = (int(j / 4) + 1) * 1500

		values_num = i_cardinality * j_cardinality * 0.1
		print (values_num)
		for k in range(int(values_num)):

			i_value = str(random.randrange(0, i_cardinality))
			j_value = str(random.randrange(0, j_cardinality))

			fij.write(i_value + "\t" + j_value + "\n")
			fji.write(j_value + "\t" + i_value + "\n")

		fij.close()
		fji.close()