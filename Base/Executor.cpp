//
// Created by serafeim on 22/8/2019.
//
#include <fstream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include<unordered_map>
#include "Executor.h"
#include "../TransitionMatrix/TransitionMatrix.h"
#include "../HRank/HRankSY.h"

using namespace Eigen;

Executor::Executor(Config* config) : _config(config) {}

void Executor::read_constraints() {
    istream *infile = new ifstream(this->_config->_constraints_file);

    string line;

    while (getline(*infile, line)) {
        vector<string> tokens;
        boost::split(tokens, line, boost::is_any_of("\t"));

        auto it = _constraints.find(tokens[0]);

        // this node has already assigned constraints
        if (it != _constraints.end()) {
            it->second.insert(make_pair(tokens[1], tokens[2]));

            // node was not found, so insert that node with its constraint
        } else {
            map<string, string> filter_map;
            filter_map.insert(make_pair(tokens[1], tokens[2]));
            _constraints.insert(make_pair(tokens[0], filter_map));
        }
    }
    delete infile;
}

map<string, ConstraintMatrix*> Executor::build_constraint_matrices(string metapath, vector<int> *dimensions) {
    map<string, ConstraintMatrix*> constraint_matrices;

    // get max id values && read constraint matrices
    for(unsigned int i=0; i<metapath.size(); i++) {

        // convert char to string
        string node_name(1, metapath[i]);

        // find dimension of the constraint table
        string node_data_file = this->_config->_nodes_dir + node_name + ".csv";
        int id_idx = Utils::get_column_idx(node_data_file, "id");
        int dim = Utils::get_max_column_value(node_data_file, id_idx) + 1;

        dimensions->push_back(dim);

        // if constraint matrix for this node type
        // is already created, then continue
        auto mit = constraint_matrices.find(node_name);
        if (mit != constraint_matrices.end())
            continue;

        // if constraints are given for this node type
        // then create constraint matrix
        auto it = _constraints.find(node_name);

        if (it != _constraints.end()) {

            ConstraintMatrix *matrix = new ConstraintMatrix(node_name, node_data_file, it->second, dim);

            matrix->build();
            //matrix.print_constraints();
            //matrix.print();

            constraint_matrices.insert(make_pair(node_name, matrix));
        }
    }

    return constraint_matrices;
}

void Executor::delete_constraint_matrices(map<string, ConstraintMatrix *> constraint_matrices) {

    auto it = constraint_matrices.begin();

    while (it != constraint_matrices.end()) {
        delete it->second;
        it++;
    }
}

vector<TransitionMatrix*> Executor::build_transition_matrices(string metapath, vector<int> dimensions, map<string, ConstraintMatrix*> constraint_matrices) {

    // create map with empty triplet lists for each matrix
    unordered_map<string, vector<Triplet<int>>*> triplets_map;
    for(unsigned int i=0; i<metapath.size()-1; i++) {
        triplets_map.insert(make_pair(metapath.substr(i, 2), new vector<Triplet<int>>()));
    }

    // open relations file
    ifstream relations_fd(this->_config->getRelationsFile());
    if (!relations_fd.good()) {
        cerr << "Error: No valid input relations file given" << endl;
        exit(EXIT_FAILURE);
    }

    // read relations file and keep triplets for each matrix
    string line;
    while (getline(relations_fd, line)) {
        string rel;
        int i = 0, src = -1, dest = -1;
        std::istringstream iss(line);

        while (getline(iss, line, '\t')) {
            if (i == 0) {
                rel = line.c_str();
            } else if (i == 1){
                src = strtol(line.c_str(), nullptr, 10);
            } else if (i == 2) {
                dest = strtol(line.c_str(), nullptr, 10);
            }
            i++;
        }

        // cout << rel << "\t" << src << "\t" << dest << endl;
        auto it = triplets_map.find(rel);
        if (it != triplets_map.end()) {
            it->second->emplace_back(src, dest, 1);
        }
    }
    // close relations file
    relations_fd.close(); 

    // construct matrices (and apply constraints) 
    vector<TransitionMatrix*> matrices;
    for(unsigned int i=0; i<metapath.size()-1; i++) {
        string src(1, metapath[i]);
        string trg(1, metapath[i+1]);

        string rel = src + trg;

        auto tr = triplets_map.find(rel);
        if (tr == triplets_map.end()) {
            cerr << "Error: Cannot find triplets for relation " << rel << endl;
            exit(EXIT_FAILURE);
        }

        auto *transition_matrix = new TransitionMatrix(rel, dimensions[i], dimensions[i+1]);
        transition_matrix->createFromTriplets(tr->second);

        auto *t = transition_matrix->get_matrix();

        // apply constraints of source node type
        auto it = constraint_matrices.find(src);
        if (it != constraint_matrices.end()) {
            *t = *(it->second->get_matrix()) * (*t);
        }

        // apply constraints of target node type
        it = constraint_matrices.find(trg);
        if (it != constraint_matrices.end()) {
            *t = (*t) * *(it->second->get_matrix());
        }

        //delete transition_matrix;
        matrices.push_back(transition_matrix);
    }

    // delete triplets
    for (auto it = triplets_map.begin(); it != triplets_map.end(); it++ ) {
        delete it->second;
    }

    return matrices;
}

void Executor::write_results_to_file(TransitionMatrix* result, string metapath) {
    
    // write results on disk
    string output_file = this->_config->_output_file + "/" + metapath + "_" + this->_config->getAlgorithm() + ".csv";
    ofstream fd = ofstream(output_file);
    result->write_to_file(fd, result->get_matrix());
    fd.close();
    string sort_cmd = "/usr/bin/sort -k1,1 -k2,2 -n -o" + output_file + " " + output_file;
    system(sort_cmd.c_str());
}

void Executor::batch_run() {

    this->read_constraints();

    HRankSY* hrank = new HRankSY(this->_config);

    istream *infile = new ifstream(this->_config->_query_file);

    string metapath;

    // execute all queries of query file
    while (getline(*infile, metapath)) {
        
        Utils::log("Process metapath: " + metapath + "\n");
        Utils::log("Building constraint matrices.");
clock_t begin = clock();
        vector<int> dimensions;
        map<string, ConstraintMatrix*> constraint_matrices = build_constraint_matrices(metapath, &dimensions);
Utils::logTime(begin);

        Utils::log("Building transition matrices.");
begin = clock();
        vector<unsigned long long int> non_zeros;
        vector<TransitionMatrix*> matrices = this->build_transition_matrices(metapath, dimensions, constraint_matrices);
Utils::logTime(begin);

        delete_constraint_matrices(constraint_matrices);

        Utils::log("Executing matrix multiplication.");
begin = clock();
        algorithm_type algorithm = this->_config->_algo;
        TransitionMatrix* result = nullptr;

        if (algorithm == algorithm_type::Seq || algorithm == algorithm_type::DynP) {
            result = hrank->run(metapath, matrices, dimensions);
        } else {
            cout << "ERROR: Unknown algorithm given" << endl;
            return;
        } 
Utils::logTime(begin);

Utils::log("Writing result to output file.");
begin = clock();
        this->write_results_to_file(result, metapath);
Utils::logTime(begin);

        delete result;
    }

    delete hrank;

    delete infile;
}
