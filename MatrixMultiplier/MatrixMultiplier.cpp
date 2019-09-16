//
// Created by serafeim on 22/8/2019.
//

#include <iostream>
#include "MatrixMultiplier.h"
#include "DynamicOptimizer.h"
#include "../Utils.h"

using namespace std;

TransitionMatrix* MatrixMultiplier::sequential(vector<TransitionMatrix*> matrices, bool adaptive, int max_memory, bool delete_input) {
    TransitionMatrix *result = matrices[0];
    TransitionMatrix *tmp_ptr = nullptr;

    unsigned long long int reads = 0;


    string tmp_dir = result->get_relation_dir() + "tmp/";

    for (unsigned int i=1; i<matrices.size(); i++) {

        tmp_ptr = result;

#ifdef DEBUG_MSG
        clock_t begin = clock();
        cout << "Sequential: " << result->get_relation() << "(" << result->get_matrix()->nonZeros() << ") x " << matrices[i]->get_relation() << "(" << matrices[i]->get_matrix()->nonZeros() << ")" << endl;
#endif

        if (adaptive) {
            result = TransitionMatrix::adaptive_dot(result, matrices[i], &reads, max_memory, tmp_dir);
        } else {
            result = TransitionMatrix::dot(result, matrices[i], tmp_dir);
        }

#ifdef DEBUG_MSG
        cout << "Time: " << double(clock() - begin) / CLOCKS_PER_SEC << " seconds" << endl;
        cout << "Result: " << result->get_relation() << " = " << result->get_matrix()->nonZeros() << endl;
#endif

        if (delete_input) {
            delete tmp_ptr;
            delete matrices[i];
        }

    }

#ifdef DEBUG_MSG
    if  (adaptive) {
            cout << "Reads: " << reads << endl;
        }
#endif

    return result;
}


TransitionMatrix* MatrixMultiplier::dynamic(vector<TransitionMatrix*> matrices, bool adaptive, int max_memory, vector<int> dimensions, bool delete_input) {
    string tmp_dir = matrices[0]->get_relation_dir() + "tmp/";

    auto dynamic_optimizer = new DynamicOptimizer(dimensions.size());

    dynamic_optimizer->optimal_matrix_chain_order(dimensions);

#ifdef DEBUG_MSG
    cout << "Optimal Chain Order: ";
        dynamic_optimizer->print_optimal_chain_order(0, dimensions.size() - 2);
        cout << endl;
#endif

#ifdef DEBUG_MSG
    for (auto & t : matrices) {
        cout << t->get_relation() << ": " << t->get_matrix()->nonZeros() << endl;
    }
#endif

    // get ordering of multiplications
    vector<pair<int, int>> chain_order;
    dynamic_optimizer->get_optimal_chain_order(0, dimensions.size() - 2, &chain_order);

    vector<TransitionMatrix*> temp;
    TransitionMatrix *tmp_ptr = nullptr;

    unsigned long long int reads = 0;

    for (auto it = chain_order.begin(); it != chain_order.end(); ++it) {

        int k = it->first;
        int l = it->second;
        int n = temp.size();

#ifdef DEBUG_MSG
        clock_t begin = clock();
#endif

        if (k >= 0 && l >= 0) {
#ifdef DEBUG_MSG
    cout << matrices[k]->get_relation() << "(" << matrices[k]->get_matrix()->nonZeros() <<") * "
    << matrices[l]->get_relation() << "(" << matrices[l]->get_matrix()->nonZeros() << ")" << endl;
#endif
            TransitionMatrix *res = (adaptive)
                        ? TransitionMatrix::adaptive_dot(matrices[k], matrices[l], &reads, max_memory, tmp_dir)
                        : TransitionMatrix::dot(matrices[k], matrices[l], tmp_dir);

            // delete multiplied matrices
            if (delete_input) {
                delete matrices[k];
                delete matrices[l];
            }
#ifdef DEBUG_MSG
    cout << "RES: " << res->get_matrix()->nonZeros() << endl;
#endif
            // store temp result
            temp.push_back(res);

        } else if (k == -1 && l >= 0) {

#ifdef DEBUG_MSG
    cout << temp[n-1]->get_relation() << "(" << temp[n-1]->get_matrix()->nonZeros() <<") * "
     << matrices[l]->get_relation() << "(" << matrices[l]->get_matrix()->nonZeros() << ")" << endl;
#endif

            tmp_ptr = temp[n-1];
            temp[n-1] = (adaptive)
                        ? TransitionMatrix::adaptive_dot(temp[n - 1], matrices[l], &reads, max_memory, tmp_dir)
                        : TransitionMatrix::dot(temp[n-1], matrices[l], tmp_dir);
#ifdef DEBUG_MSG
    cout << "RES: " << temp[n-1]->get_matrix()->nonZeros() << endl;
#endif

            // delete multiplied matrices
            if (delete_input) {
                delete matrices[l];
                delete tmp_ptr;
            }

        } else if (k >= 0 && l == -1) {
#ifdef DEBUG_MSG
    cout << matrices[k]->get_relation() << "(" << matrices[k]->get_matrix()->nonZeros() <<") * "
     << temp[n-1]->get_relation() << "(" << temp[n-1]->get_matrix()->nonZeros() << ")" << endl;
#endif

            tmp_ptr = temp[n-1];
            temp[n-1] = (adaptive)
                        ? TransitionMatrix::adaptive_dot(matrices[k], temp[n - 1], &reads, max_memory, tmp_dir)
                        : TransitionMatrix::dot(matrices[k], temp[n-1], tmp_dir);
#ifdef DEBUG_MSG
    cout << "RES: " << temp[n-1]->get_matrix()->nonZeros() << endl;
#endif

            // delete multipliced matrices
            if (delete_input) {
                delete matrices[k];
                delete tmp_ptr;
            }

        } else {
#ifdef DEBUG_MSG
    cout << temp[n-2]->get_relation() << "(" << temp[n-2]->get_matrix()->nonZeros() <<") * "
    << temp[n-1]->get_relation() << "(" << temp[n-1]->get_matrix()->nonZeros() << ")" << endl;
#endif

            tmp_ptr = temp[n-2];
            temp[n-2] = (adaptive)
                        ? TransitionMatrix::adaptive_dot(temp[n - 2], temp[n - 1], &reads, max_memory, tmp_dir)
                        : TransitionMatrix::dot(temp[n-2], temp[n-1], tmp_dir);
#ifdef DEBUG_MSG
    cout << "RES: " << temp[n-2]->get_matrix()->nonZeros() << endl;
#endif

            // delete multiplied matrices
            if (delete_input) {
                delete tmp_ptr;
                delete temp[n - 1];
            }

            temp.pop_back();
        }
#ifdef DEBUG_MSG
    cout << double(clock() - begin) / CLOCKS_PER_SEC << " seconds" << endl;
#endif

    }

    delete dynamic_optimizer;

    return temp[0];
}