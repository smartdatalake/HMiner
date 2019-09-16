//
// Created by serafeim on 22/8/2019.
//
#include <fstream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include "Executor.h"
#include "../TransitionMatrix/TransitionMatrix.h"
#include "../HRank/HRankSY.h"

using namespace Eigen;

Executor::Executor(Config* config) : _config(config) {}

void Executor::read_constraints() {
    istream *infile = new ifstream(this->_config->constraints_file_);

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
        string node_data_file = this->_config->nodes_dir_ + node_name + ".csv";
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

    vector<TransitionMatrix*> matrices;
    for(unsigned int i=0; i<metapath.size()-1; i++) {
        string src(1, metapath[i]);
        string trg(1, metapath[i+1]);

        string rel = src + trg;

        //cout << "\t" << rel;
        TransitionMatrix *transition_matrix = new TransitionMatrix(rel, this->_config->relations_dir_, dimensions[i], dimensions[i+1]);
        transition_matrix->read();

        SparseMatrix<int> *t = transition_matrix->get_matrix();

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
    return matrices;
}

vector<TransitionMatrix*> Executor::adaptive_build_transition_matrices(string metapath, vector<int> dimensions,
                                                                      vector<unsigned long long int> *non_zeros) {

    vector<TransitionMatrix*> matrices;

    for(unsigned int i=0; i<metapath.size()-1; i++) {
        string src(1, metapath[i]);
        string trg(1, metapath[i + 1]);

        string rel = src + trg;

        //cout << "\t" << rel;
        TransitionMatrix *transition_matrix = new TransitionMatrix(rel, this->_config->relations_dir_, dimensions[i], dimensions[i + 1]);

        non_zeros->push_back(transition_matrix->non_zeros());
        matrices.push_back(transition_matrix);
    }
    return matrices;
}

void Executor::write_results_to_file(TransitionMatrix* result, string metapath) {

    if (result->is_in_memory()) {

        if (this->_config->output_dir_.empty()) {

            // print to stdout
            Utils::print(result->get_matrix());
        } else {
            // or write results on disk
            string output_file = this->_config->output_dir_ + "/" + metapath + "_" + this->_config->getAlgorithm() + ".csv";
            ofstream fd = ofstream(output_file);
            result->write_to_file(fd, result->get_matrix(), DataFileType::crs);
            fd.close();
            string sort_cmd = "/usr/bin/sort -k1,1 -k2,2 -n -o" + output_file + " " + output_file;
            system(sort_cmd.c_str());
        }
    }
}

void Executor::batch_run() {

    Utils::debug_msg("Reading constraints from file");
    this->read_constraints();

    HRankSY* hrank = new HRankSY(this->_config);

    istream *infile = new ifstream(this->_config->query_file_);

    string metapath;

    // execute all queries of query file
    while (getline(*infile, metapath)) {

        Utils::debug_msg("Building constraint matrices for " + metapath);

        vector<int> dimensions;
        map<string, ConstraintMatrix*> constraint_matrices = build_constraint_matrices(metapath, &dimensions);

        Utils::debug_msg("Building transition matrices");

        vector<unsigned long long int> non_zeros;
        vector<TransitionMatrix*> matrices = (this->_config->adaptive_)
             // TODO: should apply constraints in adaptive too
             ? adaptive_build_transition_matrices(metapath, dimensions, &non_zeros)
             : build_transition_matrices(metapath, dimensions, constraint_matrices);

        delete_constraint_matrices(constraint_matrices);

        clock_t begin = clock();

        algorithm_type algorithm = this->_config->algo_;
        TransitionMatrix* result = nullptr;

        if (algorithm == algorithm_type::Seq || algorithm == algorithm_type::DynP) {
            result = hrank->run(metapath, matrices, dimensions);
        } else {
            cout << "ERROR: Unknown algorithm given" << endl;
            return;
        }

        this->write_results_to_file(result, metapath);

        cout << metapath << "\t" << double(clock() - begin) / CLOCKS_PER_SEC << endl;

        delete result;
    }

    delete hrank;

    delete infile;
}
