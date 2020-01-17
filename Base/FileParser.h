#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <unordered_map>
#include <vector>
#include "../libs/json-parser/single_include/nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

class FileParser {

    public:
        // static void getMetapathAndConstraints(string query_line, string &metapath, unordered_map<string, string> &constraints);
        static void getMatrixDimensions(string metapath, string nodes_dir, vector<int> *dimensions);

        static json readQuery(string query_file);
};

#endif