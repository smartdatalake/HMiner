//
// Created by serafeim on 22/8/2019.
//

#include <iostream>
#include "MatrixMultiplier.h"
#include "DynamicOptimizer.h"
#include "../Utils.h"

using namespace std;

TransitionMatrix* MatrixMultiplier::sequential(vector<TransitionMatrix*> matrices, bool delete_input) {
    TransitionMatrix *result = matrices[0];
    TransitionMatrix *tmp_ptr = nullptr;

    for (unsigned int i=1; i<matrices.size(); i++) {

        tmp_ptr = result;

        result = TransitionMatrix::dot(result, matrices[i]);

        if (delete_input) {
            delete tmp_ptr;
            delete matrices[i];
        }

    }

    return result;
}

TransitionMatrix* MatrixMultiplier::dynamic(vector<TransitionMatrix*> matrices, vector<int> dimensions, bool delete_input) {

    auto dynamic_optimizer = new DynamicOptimizer(dimensions.size());

    dynamic_optimizer->optimal_matrix_chain_order(dimensions);

    // get ordering of multiplications
    vector<pair<int, int>> chain_order;
    dynamic_optimizer->get_optimal_chain_order(0, dimensions.size() - 2, &chain_order);

    vector<TransitionMatrix*> temp;
    TransitionMatrix *tmp_ptr = nullptr;

    for (auto it = chain_order.begin(); it != chain_order.end(); ++it) {

        int k = it->first;
        int l = it->second;
        int n = temp.size();

        if (k >= 0 && l >= 0) {

            TransitionMatrix *res = TransitionMatrix::dot(matrices[k], matrices[l]);

            // delete multiplied matrices
            if (delete_input) {
                delete matrices[k];
                delete matrices[l];
            }

            // store temp result
            temp.push_back(res);

        } else if (k == -1 && l >= 0) {

            tmp_ptr = temp[n-1];
            temp[n-1] = TransitionMatrix::dot(temp[n-1], matrices[l]);

            // delete multiplied matrices
            if (delete_input) {
                delete matrices[l];
                delete tmp_ptr;
            }

        } else if (k >= 0 && l == -1) {

            tmp_ptr = temp[n-1];
            temp[n-1] = TransitionMatrix::dot(matrices[k], temp[n-1]);

            // delete multipliced matrices
            if (delete_input) {
                delete matrices[k];
                delete tmp_ptr;
            }

        } else {

            tmp_ptr = temp[n-2];
            temp[n-2] = TransitionMatrix::dot(temp[n-2], temp[n-1]);

            // delete multiplied matrices
            if (delete_input) {
                delete tmp_ptr;
                delete temp[n - 1];
            }

            temp.pop_back();
        }
    }

    delete dynamic_optimizer;

    return temp[0];
}