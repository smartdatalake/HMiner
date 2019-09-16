//
// Created by serafeim on 22/8/2019.
//

#ifndef HRANK_MATRIXMULTIPLIER_H
#define HRANK_MATRIXMULTIPLIER_H


#include "../TransitionMatrix/TransitionMatrix.h"

class MatrixMultiplier {
public:
    static TransitionMatrix* sequential(vector<TransitionMatrix*> matrices, bool adaptive, int max_memory, bool delete_input);
    static TransitionMatrix* dynamic(vector<TransitionMatrix*> matrices, bool adaptive, int max_memory, vector<int> dimensions, bool delete_input);
};


#endif //HRANK_MATRIXMULTIPLIER_H
