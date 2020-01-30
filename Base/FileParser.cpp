#include <iostream>
#include <fstream>

#include "FileParser.h"
#include "../Utils.h"
#include "../libs/csv-filter/dsv_filter.hpp"

json FileParser::readConfig(string config_file) {
    
    // read whole file into string
    ifstream fd(config_file);
    stringstream stream;
    stream << fd.rdbuf();
    fd.close();

    // parse query file content into json object
    return json::parse(stream.str());
}

int FileParser::getMatrixDimensions(string metapath, string nodes_dir, vector<int> *dimensions) {
    dsv_filter filter;
    filter.set_input_delimiter("\t");

    // get max id values && read constraint matrices
    for(unsigned int i=0; i<metapath.size(); i++) {

        // find dimension of the constraint table
        string node_data_file = nodes_dir + metapath.substr(i, 1) + ".csv";
        
        int id_idx = Utils::get_column_idx(node_data_file, "id_n");
        int dim = Utils::get_max_column_value(node_data_file, 0) + 1;

        // if (!filter.load(node_data_file)) {
        //     return -1;
        // }

        // int dim = filter.row_count();
        dimensions->push_back(dim);
    }
    return 0;
}