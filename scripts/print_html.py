import sys

filepath = sys.argv[1]
pagerank_output = sys.argv[2]

dict = {}

with open(filepath) as fp:
    line = fp.readline()

    while line:
        line = line.rstrip()
        parts = line.strip().split("\t")
        dict[parts[0]] = parts[1]

        line = fp.readline()


    # print (dict)

with open(pagerank_output) as fd:
    line = fd.readline()

    # print ("<table>")
    while line:

        line = line.rstrip()
        parts = line.split("\t")
        print (dict[parts[0]] + "\t" + parts[1])
        # print("<tr>")
        # print("<td>" + dict[parts[0]] + "</td>")
        # print("<td>" + parts[1] + "</td>")
        # print("</tr>")
        line = fd.readline()



    # print("</table>")