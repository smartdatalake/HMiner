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
    string _config_file;
    string _nodes_dir;
    string _relations_file;
    string _relations_dir;
    string _constraints_file;
    string _query_file;
    algorithm_type _algo;
    string _output_dir;
    string _output_type; 

    void setQueryFile(const string &query_file);

    void setNodesDir(const string &nodes_dir);

    void setRelationsFile(const string &relations_file);

    void setConstraintsFile(const string &constraints_file);

    void setAlgorithm(algorithm_type algo);

    void setRelationsDir(const string &relations_dir); 

    void setOutputType(const string &output_type);

    void printArgs();

    void setOutputDir(string output_dir);

    void setConfigFile(string config_file);

    string getAlgorithm() const;

    const string &getQueryFile() const;

    const string &getRelationsFile() const;

    const string &getRelationsDir() const;
    
    const string &getOutputType() const;

    const string &getConfigFile() const;
};


#endif //HRANK_CONFIG_H
