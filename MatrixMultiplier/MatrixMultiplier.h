//
// Created by serafeim on 22/8/2019.
//

#ifndef HRANK_MATRIXMULTIPLIER_H
#define HRANK_MATRIXMULTIPLIER_H

#include "../Utils.h"
#include "../TransitionMatrix/TransitionMatrix.h"

class MatrixMultiplier {
public:
    static TransitionMatrix* sequential(vector<TransitionMatrix*> matrices, int threshold, bool delete_input);
    static TransitionMatrix* dynamic(vector<TransitionMatrix*> matrices, int threshold, vector<int> dimensions, optimizer_type sparse_optimizer, bool delete_input, long double &cost);

    static vector<string> getSubpathsFromDynamicPlanning(vector<TransitionMatrix*> matrices, vector<int> dimensions);
    static long double estimateResultMemory(vector<int> dims, vector<TransitionMatrix*> matrices);
};


#endif //HRANK_MATRIXMULTIPLIER_H
