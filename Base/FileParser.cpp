#include <iostream>
#include <fstream>

#include "FileParser.h"
#include "../Utils.h"

json FileParser::readQuery(string query_file) {
    
    // read whole file into string
    ifstream fd(query_file);
    stringstream stream;
    stream << fd.rdbuf();
    fd.close();

    // parse query file content into json object
    return json::parse(stream.str());
}

void FileParser::getMatrixDimensions(string metapath, string nodes_dir, vector<int> *dimensions) {

    // get max id values && read constraint matrices
    for(unsigned int i=0; i<metapath.size(); i++) {

        // find dimension of the constraint table
        string node_data_file = nodes_dir + metapath.substr(i, 1) + ".csv";
        int id_idx = Utils::get_column_idx(node_data_file, "id_n");
        int dim = Utils::get_max_column_value(node_data_file, id_idx) + 1;

        dimensions->push_back(dim);
    }
}