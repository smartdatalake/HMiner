//
// Created by serafeim on 27/5/2019.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Config.h"
#include "../Utils.h"

void Config::setQueryFile(const string &query_file) {
    _query_file = query_file;
}

const string &Config::getQueryFile() const {
    return _query_file;
}

const string &Config::getRelationsFile() const {
    return this->_relations_file;
}

const string &Config::getConfigFile() const {
    return this->_config_file;
}

void Config::setNodesDir(const string &nodes_dir) {
    _nodes_dir = nodes_dir;
}

void Config::setRelationsFile(const string &relations_file) {
    _relations_file = relations_file;
}

void Config::setConstraintsFile(const string &constraints_file) {
    _constraints_file = constraints_file;
}


void Config::setAlgorithm(algorithm_type algo) {
    _algo = algo;
}

void Config::setOutputDir(string output_dir) {
    _output_dir = output_dir;
}

void Config::setOutputType(const string &output_type) {
    this->_output_type = output_type;
}

void Config::setConfigFile(const string config_file) {
    this->_config_file = config_file;
}

string Config::getAlgorithm() const {
    switch (Config::_algo) {
        case algorithm_type::HRank:   return "HRank";
        case algorithm_type::BS1:   return "BS1";
        case algorithm_type::BS2:   return "BS2";
        default:      return "Unknown";
    }
}

void Config::setRelationsDir(const string &relations_dir) {
    this->_relations_dir = relations_dir;
}

const string & Config::getRelationsDir() const {
    return this->_relations_dir;
}

const string & Config::getOutputType() const {
    return this->_output_type;
}

