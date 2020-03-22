import sys
import json
import utils 

with open(sys.argv[2]) as config_file:
    config = json.load(config_file)
    nodes_dir = config["indir"]
    infile = config["hin_out"]
    outfile = config["analysis_out"]
    metapath = config["query"]["metapath"]
    src_field = config["query"]["src_field"]
    dest_field = config["query"]["dest_field"]
    build_index = config["query"]["buildIndex"]
    src_entity = metapath[:1]
    dest_entity = metapath[-1]


if build_index == False:
    sys.exit(0)

src_entity_file = nodes_dir + src_entity + ".csv"
dest_entity_file = nodes_dir + dest_entity + ".csv"

# print(src_entity_file)
# print(dest_entity_file)

# def expand_result(value, times):

src_dict = utils.read_data_file(src_entity_file, src_field)
dest_dict = utils.read_data_file(dest_entity_file, dest_field)

srcId = ''
src = ''
values = ''
# read analysis resuls and append names
with open(outfile, 'w') as fout:

    with open(infile) as fin:
        
        for line in fin:
            line = line.rstrip()
            parts = line.split("\t")

            if (srcId != parts[0]):
               
                srcId = parts[0]

                if (src != ''):
                    fout.write(src + "\t" + values + "\n")
                    values = ''

                # append src field
                src = 'Unknown'
                if parts[0] in src_dict: 
                    src = src_dict[parts[0]]
            
            # print (line)
            value = 'Unknown'
            if parts[1] in dest_dict:
                value = dest_dict[parts[1]]

            if values != '':
                values += ";"
                
            values += ";".join([value]*int(parts[2]))
                
    fout.close()
            #     for i in range(0, int(parts[2])):
            #         dest.append(value)


            # dest = []
            # for dest_rec in parts[1].split(";"):
            #     # print (dest_rec)

            #     desc_rec_parts = dest_rec.split("|")

                
            
            # row_data.append(";".join(dest))
            # # print(row_data)
            
            # filewriter.writerow(row_data)
