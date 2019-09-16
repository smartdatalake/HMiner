import random

nodes = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J']
print (nodes)
queries = []

for i in range(200):
    length = int(random.randrange(4, 10))

    q = ""
    prev_node = ""

    for l in range(length):

        while(1):

            cur_node = nodes[int(random.randrange(0, 10))]

            if (cur_node != prev_node):
                q += cur_node
                prev_node = cur_node
                break


    print (q)
    queries.append(q)

print ("lala")
print(queries)
print (random.randrange(0, len(queries)))
