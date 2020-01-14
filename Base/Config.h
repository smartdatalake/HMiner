//
// Created by serafeim on 27/5/2019.
//

#ifndef HRANK_CONFIG_H
#define HRANK_CONFIG_H

#include <string>
#include <vector>
#include <map>
#include "../Utils.h"

using namespace std;

class Config {
public:
    string _nodes_dir;
    string _relations_file;
    string _constraints_file;
    string _query_file;
    algorithm_type _algo;
    string _output_file;

    void setQueryFile(const string &query_file);

    void setNodesDir(const string &nodes_dir);

    void setRelationsFile(const string &relations_file);

    void setConstraintsFile(const string &constraints_file);

    void setAlgorithm(algorithm_type algo);

    void printArgs();

    void setOutputFile(string output_dir);

    string getAlgorithm() const;

    const string &getQueryFile() const;

    const string &getRelationsFile() const;
};


#endif //HRANK_CONFIG_H
