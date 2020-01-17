//
// Created by serafeim on 22/8/2019.
//
#include <fstream>
#include <string>
#include <unordered_map>
#include "Executor.h"
#include "../TransitionMatrix/TransitionMatrix.h"
#include "../HRank/HRankSY.h"
#include "FileParser.h"

using json = nlohmann::json;
using namespace Eigen;

Executor::Executor(Config* config) : _config(config) {}

int Executor::buildConstraintMatrices(json query, vector<int> *dimensions, map<string, ConstraintMatrix*> &constraint_matrices) {

    string metapath = query["metapath"];

    for(unsigned int i=0; i<metapath.size(); i++) {

        // convert char to string
        string node_name = metapath.substr(i, 1);

        // if constraint matrix for this node type
        // is already created, then continue
        auto it = constraint_matrices.find(node_name);
        if (it != constraint_matrices.end())
            continue;

        // find dimension of the constraint table
        string node_data_file = this->_config->_nodes_dir + node_name + ".csv";

        // if constraints are given for this node type
        // then create constraint matrix
        if (query["constraints"].find(node_name) != query["constraints"].end()) {
            
            string expression = query["constraints"][node_name];

            // check for empty constraint string
            Utils::trim(expression);
            if (expression.empty()) {
                continue;
            }

            auto *matrix = new ConstraintMatrix(node_name, node_data_file, (*dimensions)[i]);
            
            if (matrix->build(expression)) {
                cerr << "Error: Building constraint matrix for entity " << node_name << endl;
                return -1;
            }
            cout << "* nonZero(" << node_name << ") = " << matrix->nonZeros() << endl;

            constraint_matrices.emplace(node_name, matrix);
        }
    }

    return 0;
}

void Executor::cleanup(map<string, ConstraintMatrix *> constraint_matrices) {

    auto it = constraint_matrices.begin();

    while (it != constraint_matrices.end()) {
        delete it->second;
        it++;
    }
}

void Executor::cleanup(unordered_map<string, TransitionMatrix*> transition_matrices) {
    auto it = transition_matrices.begin();

    while (it != transition_matrices.end()) {
        delete it->second;
        it++;
    }
}

int Executor::buildTransitionMatrices(string metapath, vector<int> dimensions, map<string, 
        ConstraintMatrix*> constraint_matrices, vector<TransitionMatrix*> &transition_matrices) {

    cout << "*RELATIONS WITHOUT CONSTRAINTS*" << endl;

    // create map with transition matrices for unique relations in metapath
    clock_t begin = clock();
    unordered_map<string, TransitionMatrix*> matrices_map;
    for(unsigned int i=0; i<metapath.size()-1; i++) {
        
        string relation = metapath.substr(i, 2);
        if (matrices_map.find(relation) == matrices_map.end()) {
            auto* tm = new TransitionMatrix(relation, dimensions[i], dimensions[i+1]);

            // if dir containing relation files is given (relations are loaded faster)
            if (!this->_config->_relations_dir.empty()) {
                if (tm->build(this->_config->_relations_dir)) {
                    cerr << "Error building transition matrix for relation " << relation << endl;
                    return -1;
                }
            
            // else use relations file
            } else {

                if (tm->buildFromSingleFile(this->_config->getRelationsFile())) {
                    cerr << "Error building transition matrix for relation " << relation << endl;
                    return -1;
                }
            }

            matrices_map.emplace(relation, tm);
            cout << "* relations(" << relation << ") = " << tm->nonZeros() << endl;

        }
    }
    Utils::log("Read relations from file in " + to_string(Utils::diffTime(begin)) + " sec");

    cout << "*TRANSITION MATRICES*" << endl;

    // construct matrices (and apply constraints) 
    begin = clock();
    for(unsigned int i=0; i<metapath.size()-1; i++) {
        string src = metapath.substr(i, 1);
        string dest = metapath.substr(i+1, 1);
        string relation = src + dest;

        auto it = matrices_map.find(relation);
        if (it == matrices_map.end()) {
            cerr << "Error: cannot find matrix for relation: " << relation << endl;
            return -2;
        }
        
        // copy constructor
        auto *transition_matrix = new TransitionMatrix(it->second);
        auto *t = transition_matrix->getMatrix();

        // apply constraints of source node type
        auto cit = constraint_matrices.find(src);
        if (cit != constraint_matrices.end()) {
            *t = *(cit->second->getMatrix()) * (*t);
        }

        // apply constraints of target node type
        cit = constraint_matrices.find(dest);
        if (cit != constraint_matrices.end()) {
            *t = (*t) * *(cit->second->getMatrix());
        }

        transition_matrices.push_back(transition_matrix);
        cout << "* relation(" << transition_matrix->getRelation() << ") = " << transition_matrix->nonZeros() << endl;

    }
    Utils::log("Apply constraints in " + to_string(Utils::diffTime(begin)) + " sec");

    this->cleanup(constraint_matrices);
    this->cleanup(matrices_map);

    return 0;
}

int Executor::write(TransitionMatrix* result, string outfile) {

    ofstream fd = ofstream(outfile);
    result->writeForPagerank(fd, result->getMatrix());
    fd.close();
    return 0;
}

void Executor::run() {

    // read & parse query from query file
    json query = FileParser::readQuery(this->_config->_query_file);

    Utils::log("Process metapath: " + to_string(query["metapath"]));

    // calculate dimensions for matrices 
    clock_t begin = clock();
    vector<int> dimensions;
    if (FileParser::getMatrixDimensions(query["metapath"], this->_config->_nodes_dir, &dimensions)) {
        cerr << "Error: Cannot read matrix dimensions from input files" << endl;
        exit(EXIT_FAILURE);
    }

    Utils::log("Matrix Dimensions calculated in " + to_string(Utils::diffTime(begin)) + " sec");

    // if constraints are given, build constraint matrices
    begin = clock();
    map<string, ConstraintMatrix*> constraint_matrices;
    if (!query["constraints"].empty()) {
        if (this->buildConstraintMatrices(query, &dimensions, constraint_matrices)) {
            cerr << "Error: Building constraint matrices" << endl;
            exit(EXIT_FAILURE);
        }
    }    
    Utils::log("Constraint Matrices successfully built " + to_string(Utils::diffTime(begin)) + " sec");

    // build transition matrices
    begin = clock();
    vector<TransitionMatrix*> transition_matrices ;
    if (this->buildTransitionMatrices(query["metapath"], dimensions, constraint_matrices, transition_matrices)) {
        cerr << "Error: Building transition matrices" << endl;
        exit(EXIT_FAILURE);
    }

    auto* hrank = new HRankSY(this->_config);

    begin = clock();
    algorithm_type algorithm = this->_config->_algo;
    TransitionMatrix* result = nullptr;

    if (algorithm == algorithm_type::Seq || algorithm == algorithm_type::DynP) {
        result = hrank->run(query["metapath"], transition_matrices, dimensions);
    } else {
        cerr << "Error: Unknown algorithm given" << endl;
        exit(EXIT_FAILURE);
    } 
    Utils::log("Matrix multiplication took " + to_string(Utils::diffTime(begin)) + " sec");

    begin = clock();
    this->write(result, this->_config->_output_file);
    Utils::log("Write result in file in " + to_string(Utils::diffTime(begin)) + " sec");

    delete result;
    delete hrank;
}
