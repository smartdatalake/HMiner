//
// Created by serafeim on 22/8/2019.
//
#include <fstream>
#include <string>
#include <unordered_map>
#include "Executor.h"
#include "../HRank/HRankSY.h"
#include "FileParser.h"
#include "../CacheBaselines/CBS1.h"
#include "../CacheBaselines/CBS2.h"
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

            #ifdef DEBUG_MSG
                cout << "* nonZero(" << node_name << ") = " << matrix->nonZeros() << endl;
            #endif

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

    #ifdef DEBUG_MSG
        cout << "*RELATIONS WITHOUT CONSTRAINTS*" << endl;
    #endif

    // create map with transition matrices for unique relations in metapath
    clock_t begin = clock();
    Utils::logProgress("Parsing relations");

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
            #ifdef DEBUG_MSG 
                cout << "* relations(" << relation << ") = " << tm->nonZeros() << endl;
            #endif
        }
    }
    Utils::logTime(begin);

    #ifdef DEBUG_MSG
        cout << "*TRANSITION MATRICES*" << endl;
    #endif

    // construct matrices (and apply constraints) 
    begin = clock();
    Utils::logProgress("Applying constraints");

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
        
        #ifdef DEBUG_MSG
            cout << "* relation(" << transition_matrix->getRelation() << ") = " << transition_matrix->nonZeros() << endl;
        #endif

    }
    Utils::logTime(begin);

    this->cleanup(constraint_matrices);
    this->cleanup(matrices_map);

    return 0;
}

int Executor::write(int i, TransitionMatrix* result, string output_dir) {
    string outfile = output_dir + "/q" + std::to_string(i) + ".csv";
    ofstream fd = ofstream(outfile);

    if (this->_config->getOutputType() == "condensed") {
        result->writeCondensed(fd, result->getMatrix());
    } else {
        result->write(fd, result->getMatrix());
    }

    fd.close();
    return 0;
}

void Executor::freeMatrices(algorithm_type algorithm, vector<TransitionMatrix*> matrices, TransitionMatrix *result, bool is_cached) {

    if (algorithm == algorithm_type::HRankSeq || algorithm == algorithm_type::HRankDynP) {
        delete result;

    // delete result if is not stored in cache
    } else if ( (algorithm == algorithm_type::OTree || algorithm == algorithm_type::BS1 
        || algorithm == algorithm_type::BS2) && !is_cached) {
        
        delete result;
    }

    // // delete matrices
    // for (TransitionMatrix* m : matrices) {
    //     cout << m << endl;
    //     if (m != nullptr)  {
    //         delete m;   
    //     }
    // }
}


int Executor::execRanking(int i, json query) {

    string cmd = "python pagerank.py "
            + this->_config->getConfigFile() + " "
            + this->_config->_output_dir + "/q" + std::to_string(i) + ".csv" + " "
            + "q" + std::to_string(i) + "_ranking.csv" + " "
            + (string)query["metapath"] + " "
            + (string)query.value("src_field", "null");
    return system(cmd.c_str());
}

void Executor::run(json params) {

    json query = params["query"];
    json ranking = params["ranking"];

    auto* cbs1 = new CBS1(this->_config);
    auto* cbs2 = new CBS2(this->_config);
    auto* hrank = new HRankSY(this->_config);

    istream *infile = new ifstream(params["qf"]);
    string query_line;
    int i = 0;

    while (getline(*infile, query_line)) {

        json query = json::parse(query_line);
        cout << endl;
        this->_config->setConstraint(query["constraints"].dump());

        Utils::logProgress("Processing Metapath: " + to_string(query["metapath"]) + "\n");
        
        // calculate dimensions for matrices 
        clock_t begin = clock();
        vector<int> dimensions;
        
        Utils::logProgress("Calculating Matrix Dimensions");

        if (FileParser::getMatrixDimensions(query["metapath"], this->_config->_nodes_dir, &dimensions)) {
            cerr << "Error: Cannot read matrix dimensions from input files" << endl;
            exit(EXIT_FAILURE);
        }
        Utils::logTime(begin);

        // if constraints are given, build constraint matrices
        Utils::logProgress("Builiding Constraint Matrices");
        begin = clock();
        map<string, ConstraintMatrix*> constraint_matrices;
        if (!query["constraints"].empty()) {
            if (this->buildConstraintMatrices(query, &dimensions, constraint_matrices)) {
                cerr << "Error: Building constraint matrices" << endl;
                exit(EXIT_FAILURE);
            }
        }    
        Utils::logTime(begin);

        // build transition matrices
        vector<TransitionMatrix*> transition_matrices ;
        if (this->buildTransitionMatrices(query["metapath"], dimensions, constraint_matrices, transition_matrices)) {
            cerr << "Error: Building Transition Matrices" << endl;
            exit(EXIT_FAILURE);
        }


        begin = clock();
        Utils::logProgress("Mining Associations");
        algorithm_type algorithm = this->_config->_algo;
        TransitionMatrix* result = nullptr;
        bool is_cached = false;

        if (transition_matrices.size() > 1) {

            // choose appropriate algorithm
            if (algorithm == algorithm_type::HRankSeq || algorithm == algorithm_type::HRankDynP) {
                result = hrank->run(query["metapath"], transition_matrices, dimensions);
            } else if (algorithm == algorithm_type::BS1) {
                result = cbs1->run(query["metapath"], transition_matrices, dimensions, is_cached);
            } else if (algorithm == algorithm_type::BS2 || algorithm == algorithm_type::OTree) {
                result = cbs2->run(query["metapath"], transition_matrices, dimensions, is_cached);
            } else {
                cout << "ERROR: Unknown algorithm given" << endl;
                return;
            }
            
        } else {
            result = transition_matrices[0];
        }

        Utils::logTime(begin);

        begin = clock();
        Utils::logProgress("Writing Results");
        this->write(i, result, this->_config->_output_dir);
        this->execRanking(i, query);
        Utils::logTime(begin);
        
        i++;

        this->freeMatrices(algorithm, transition_matrices, result, is_cached);
    }

    delete hrank;
    delete cbs1;
    delete cbs2;
}
