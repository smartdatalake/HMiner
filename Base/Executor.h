//
// Created by serafeim on 22/8/2019.
//

#ifndef HRANK_EXECUTOR_H
#define HRANK_EXECUTOR_H

#include <iostream>
#include <unordered_map>
#include "Config.h"
#include "../TransitionMatrix/TransitionMatrix.h"
#include "../TransitionMatrix/ConstraintMatrix.h"
#include "../libs/json-parser/single_include/nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

class Executor {
    Config* _config;

    map<string, map<string, string>> _constraints;

public:
    Executor(Config* config);

    void run(json query);

    int buildConstraintMatrices(json query, vector<int> *dimensions, map<string, ConstraintMatrix*> &constraint_matrices);
    int buildTransitionMatrices(string metapath, vector<int> dimensions, map<string, ConstraintMatrix*> constraint_matrices, vector<TransitionMatrix*> &transition_matrices);

    void cleanup(map<string, ConstraintMatrix*> constraint_matrices);
    void cleanup(unordered_map<string, TransitionMatrix*> transition_matrices);
    int write(int i, TransitionMatrix* result, string metapath);
    int execRanking(int i, json query);

};
#endif //HRANK_EXECUTOR_H
