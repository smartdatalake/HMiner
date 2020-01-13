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

    TransitionMatrix *result = nullptr;

    if (this->_config->_algo == algorithm_type::Seq) {
        result = MatrixMultiplier::sequential(matrices, true);
    } else {
        result = MatrixMultiplier::dynamic(matrices, dimensions, true);
    }

    return result;
}

HRankSY::HRankSY(Config *config) : _config(config) {}



