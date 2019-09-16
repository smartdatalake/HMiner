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
    query_file_ = query_file;
}

const string &Config::getQueryFile() const {
    return query_file_;
}

void Config::setNodesDir(const string &nodes_dir) {
    nodes_dir_ = nodes_dir;
}

void Config::setRelationsDir(const string &relations_dir) {
    relations_dir_ = relations_dir;
}

void Config::printArgs() {
    std::cout
        << "Metapath: " << metapath_ << endl
        << "Nodes dir: " << nodes_dir_ << endl
        << "Relations dir: " << relations_dir_ << endl
        << "Constraints file: " << constraints_file_ << endl << endl;
}

void Config::setConstraintsFile(const string &constraints_file) {
    constraints_file_ = constraints_file;
}


void Config::setAlgorithm(algorithm_type algo) {
    algo_ = algo;
}

void Config::setOTreeExpansion(algorithm_type expansion) {
    otree_expansion_ = expansion;

}

void Config::setAdaptive(bool adaptive) {
    adaptive_ = adaptive;
}

void Config::setMaxMemory(int max_memory) {
    max_memory_ = max_memory;
}

void Config::printOutputDir(string output_dir) {
    output_dir_ = output_dir;
}

const string &Config::getMetapath() const {
    return metapath_;
}

string Config::getAlgorithm() const {
    switch (Config::algo_) {
        case algorithm_type::Seq:   return "Seq";
        case algorithm_type::DynP:   return "DynP";
        default:      return "Unknown";
    }
}

void Config::setMetapath(const string &metapath) {
    metapath_ = metapath;
}

algorithm_type Config::getOtreeExpansion() const {
    return otree_expansion_;
}
