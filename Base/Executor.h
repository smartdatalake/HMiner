//
// Created by serafeim on 22/8/2019.
//

#ifndef HRANK_EXECUTOR_H
#define HRANK_EXECUTOR_H


#include "Config.h"
#include "../TransitionMatrix/TransitionMatrix.h"
#include "../TransitionMatrix/ConstraintMatrix.h"

class Executor {
    Config* _config;

    map<string, map<string, string>> _constraints;

public:
    Executor(Config* config);

    void batch_run();

    void read_constraints();
    map<string, ConstraintMatrix *> build_constraint_matrices(string metapath, vector<int> *dimensions);
    void delete_constraint_matrices(map<string, ConstraintMatrix *> constraint_matrices);
    vector<TransitionMatrix*> build_transition_matrices(string metapath, vector<int> dimensions, map<string, ConstraintMatrix*> constraint_matrices);
    vector<TransitionMatrix*> adaptive_build_transition_matrices(string metapath, vector<int> dimensions, vector<unsigned long long int> *non_zeros);
    void write_results_to_file(TransitionMatrix* result, string metapath);

};
#endif //HRANK_EXECUTOR_H
