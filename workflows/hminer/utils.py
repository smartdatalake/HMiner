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