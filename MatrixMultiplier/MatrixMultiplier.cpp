//
// Created by serafeim on 22/8/2019.
//

#include <iostream>
#include "MatrixMultiplier.h"
#include "DynamicOptimizer.h"
#include "../Utils.h"

using namespace std;

TransitionMatrix* MatrixMultiplier::sequential(vector<TransitionMatrix*> matrices, int threshold, bool delete_input) {
    TransitionMatrix *result = matrices[0];
    TransitionMatrix *tmp_ptr = nullptr;

    for (unsigned int i=1; i<matrices.size(); i++) {

        tmp_ptr = result;

        result = TransitionMatrix::dot(result, matrices[i], threshold);

        if (delete_input) {
            delete tmp_ptr;
            delete matrices[i];
        }

    }

    return result;
}

TransitionMatrix* MatrixMultiplier::dynamic(vector<TransitionMatrix*> matrices, int threshold, vector<int> dimensions, optimizer_type sparse_optimizer, bool delete_input, long double &cost) {

    auto dynamic_optimizer = new DynamicOptimizer(dimensions.size());

    // plan multiplications based on sparsity or only on matrix dimensions
    if (sparse_optimizer == optimizer_type::Sparse) {
        cost = dynamic_optimizer->sparse_optimal_matrix_chain_order(dimensions, matrices);
    } else if (sparse_optimizer == optimizer_type::Dense){
        dynamic_optimizer->optimal_matrix_chain_order(dimensions);
    } else {
        cerr << "Error: Not valid dynamic optimizer given" << endl;
        return nullptr;
    }

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

            TransitionMatrix *res = TransitionMatrix::dot(matrices[k], matrices[l], threshold);

            // delete multiplied matrices
            if (delete_input) {
                delete matrices[k];
                matrices[k] = nullptr;
                delete matrices[l];
                matrices[l] = nullptr;
            }

            // store temp result
            temp.push_back(res);

        } else if (k == -1 && l >= 0) {

            tmp_ptr = temp[n-1];
            temp[n-1] = TransitionMatrix::dot(temp[n-1], matrices[l], threshold);

            // delete multiplied matrices
            if (delete_input) {
                delete matrices[l];
                matrices[l] = nullptr;
                delete tmp_ptr;
                tmp_ptr = nullptr;
            }

        } else if (k >= 0 && l == -1) {

            tmp_ptr = temp[n-1];
            temp[n-1] = TransitionMatrix::dot(matrices[k], temp[n-1], threshold);

            // delete multipliced matrices
            if (delete_input) {
                delete matrices[k];
                matrices[k] = nullptr;
                delete tmp_ptr;
                tmp_ptr = nullptr;
            }

        } else {

            tmp_ptr = temp[n-2];
            temp[n-2] = TransitionMatrix::dot(temp[n-2], temp[n-1], threshold);

            // delete multiplied matrices
            if (delete_input) {
                delete tmp_ptr;
                tmp_ptr = nullptr;
                delete temp[n - 1];
                temp[n - 1] = nullptr;
            }

            temp.pop_back();
        }
    }

    delete dynamic_optimizer;

    return temp[0];
}

vector<string> MatrixMultiplier::getSubpathsFromDynamicPlanning(vector<TransitionMatrix*> matrices, vector<int> dimensions) {
    auto dynamic_optimizer = new DynamicOptimizer(dimensions.size());

    long double cost = dynamic_optimizer->sparse_optimal_matrix_chain_order(dimensions, matrices);

    // get ordering of multiplications
    vector<pair<int, int>> chain_order;
    dynamic_optimizer->get_optimal_chain_order(0, dimensions.size() - 2, &chain_order);

    vector<string> subpaths;

    for (auto it = chain_order.begin(); it != chain_order.end(); ++it) {

        int k = it->first;
        int l = it->second;
        int n = subpaths.size();

        if (k >= 0 && l >= 0) {
            subpaths.push_back(matrices[k]->get_relation() + matrices[l]->get_relation().substr(1));
        } else if (k == -1 && l >= 0) {
            subpaths.push_back(subpaths[n-1] + matrices[l]->get_relation().substr(1));
        } else if (k >= 0 && l == -1) {
            subpaths.push_back(matrices[k]->get_relation() + subpaths[n-1].substr(1));
        } else {
            subpaths.push_back(subpaths[n-2] + subpaths[n-1].substr(1));
        }
    }

    delete dynamic_optimizer;

    return subpaths;
}

long double MatrixMultiplier::estimateResultMemory(vector<int> dims, vector<TransitionMatrix*> matrices) {
    return DynamicOptimizer::estimateResultMemory(dims, matrices);
}
