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
    string metapath_;
    string nodes_dir_;
    string relations_dir_;
    string constraints_file_;
    string query_file_;
    algorithm_type algo_;
    bool adaptive_ { false };
    int max_memory_ { 0 };
    string output_dir_;
    algorithm_type otree_expansion_;

    void setQueryFile(const string &query_file);

    void setNodesDir(const string &nodes_dir);

    void setRelationsDir(const string &relations_dir);

    void setConstraintsFile(const string &constraints_file);

    void setAlgorithm(algorithm_type algo);

    void setAdaptive(bool adaptive);

    void setMaxMemory(int max_memory);

    void setOTreeExpansion(algorithm_type expansion);

    void printArgs();

    void printOutputDir(string output_dir);

    void setMetapath(const string &metapath);

    const string &getMetapath() const;

    string getAlgorithm() const;

    const string &getQueryFile() const;

    algorithm_type getOtreeExpansion() const;
};


#endif //HRANK_CONFIG_H
