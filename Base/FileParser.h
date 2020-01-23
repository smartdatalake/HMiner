#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <unordered_map>
#include <vector>
#include "../libs/json-parser/single_include/nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

class FileParser {

    public:

        static int getMatrixDimensions(string metapath, string nodes_dir, vector<int> *dimensions);
        static json readConfig(string config_file);
};

#endif