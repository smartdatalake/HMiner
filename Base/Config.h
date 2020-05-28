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
    string _constraints;
    long double _cache_size;
    long double _cache_threshold = 0.8;
    cache_type _cache_policy;
    int _threshold = -1;
    optimizer_type _dyn_optimizer_type;

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

    void setConstraint(string constraint) {
        this->_constraints = constraint;
    }

    string getConstraint() const {
        return this->_constraints;
    }

    void setCacheSize(float size) {
        this->_cache_size = size;
    }

    long double getCacheSize() {
        return this->_cache_size;
    }

    long double getCacheThreshold() {
        return this->_cache_size * this->_cache_threshold;
    }

    void setCachePolicy(cache_type ct) {
        this->_cache_policy = ct;
    }

    cache_type getCachePolicy() {
        return this->_cache_policy;
    }

    int getThreshold() {
        return this->_threshold;
    }

    void setTreshold(int t) {
        this->_threshold = t;
    }

    void setDynOptimizerType(optimizer_type type) {
        this->_dyn_optimizer_type = type;
    }

    optimizer_type getDynOptimizerType() const {
        return this->_dyn_optimizer_type;
    }
};


#endif //HRANK_CONFIG_H
