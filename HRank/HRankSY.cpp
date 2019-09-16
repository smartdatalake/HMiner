//
// Created by serafeim on 27/5/2019.
//

#include <iostream>
#include <ctime>
#include <fstream>

#include "HRankSY.h"
#include "../MatrixMultiplier/DynamicOptimizer.h"
#include "../TransitionMatrix/ConstraintMatrix.h"
#include "../TransitionMatrix/TransitionMatrix.h"
#include "../MatrixMultiplier/MatrixMultiplier.h"
#include "../Utils.h"


using namespace std;
using namespace Eigen;


TransitionMatrix* HRankSY::run(string metapath, vector<TransitionMatrix*> matrices, vector<int> dimensions) {

    Utils::debug_msg("Running HRank multiplication");

    TransitionMatrix *result = nullptr;

    if (this->_config->algo_ == algorithm_type::Seq) {
        result = MatrixMultiplier::sequential(matrices, this->_config->adaptive_, this->_config->max_memory_, true);
    } else {
        result = MatrixMultiplier::dynamic(matrices, this->_config->adaptive_, this->_config->max_memory_, dimensions, true);
    }

    return result;
}

void HRankSY::delete_transition_matrices(vector<TransitionMatrix*> matrices) {
    for (auto & matrix : matrices) {
        delete matrix;
    }
}

HRankSY::HRankSY(Config *config) : _config(config) {}



