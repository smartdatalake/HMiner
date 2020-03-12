import sys
import csv
import json

with open(sys.argv[2]) as config_file:
    config = json.load(config_file)
    nodes_dir = config["indir"]
    infile = config["hin_out"]
    outfile = config["analysis_out"]
    metapath = config["query"]["metapath"]
    src_field = config["query"]["src_field"]
    dest_field = config["query"]["dest_field"]
    src_entity = metapath[:1]
    dest_entity = metapath[-1]

src_entity_file = nodes_dir + src_entity + ".csv"
dest_entity_file = nodes_dir + dest_entity + ".csv"

# print(src_entity_file)
# print(dest_entity_file)

def read_data_file(input_file, selected_field):
    with open(input_file) as fp:
        
        # read header file
        line = fp.readline()

        fields = line.rstrip().split('\t')
        for i in range(len(fields)):

            if selected_field in fields[i]: 
                break

        selected_field_idx = i

        # assign names to ids 
        _dict = {}
        while line:
            line = line.rstrip()
            parts = line.strip().split("\t")
            _dict[parts[0]] = parts[selected_field_idx]
            line = fp.readline()
        
        return _dict

src_dict = read_data_file(src_entity_file, src_field)
dest_dict = read_data_file(dest_entity_file, dest_field)

# read analysis resuls and append names
with open(outfile, 'w', newline='') as csvfile:
    filewriter = csv.writer(csvfile, delimiter='\t', quotechar='"', quoting=csv.QUOTE_MINIMAL)

    with open(infile) as fd:
        
        for line in fd:
            # print(line)

            line = line.rstrip()
            parts = line.split("\t")

            row_data = []
            
            # append src field
            value = 'Unknown'
            if parts[0] in src_dict: 
                row_data.append(src_dict[parts[0]])

            dest = []
            for dest_rec in parts[1].split(";"):
                # print (dest_rec)
            
                value = 'Unknown'
                if dest_rec in dest_dict :
                    dest.append(dest_dict[dest_rec])
            row_data.append(";".join(dest))
            # print(row_data)
           
            filewriter.writerow(row_data)
