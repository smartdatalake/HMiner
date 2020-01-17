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

void Config::setNodesDir(const string &nodes_dir) {
    _nodes_dir = nodes_dir;
}

void Config::setRelationsFile(const string &relations_file) {
    _relations_file = relations_file;
}

void Config::printArgs() {
    std::cout
        << "Nodes dir: " << _nodes_dir << endl
        << "Relations dir: " << _relations_file << endl
        << "Constraints file: " << _constraints_file << endl << endl;
}

void Config::setConstraintsFile(const string &constraints_file) {
    _constraints_file = constraints_file;
}


void Config::setAlgorithm(algorithm_type algo) {
    _algo = algo;
}

void Config::setOutputFile(string output_file) {
    _output_file = output_file;
}

string Config::getAlgorithm() const {
    switch (Config::_algo) {
        case algorithm_type::Seq:   return "Seq";
        case algorithm_type::DynP:   return "DynP";
        default:      return "Unknown";
    }
}

void Config::setRelationsDir(const string &relations_dir) {
    this->_relations_dir = relations_dir;
}

const string & Config::getRelationsDir() const {
    return this->_relations_dir;
}

