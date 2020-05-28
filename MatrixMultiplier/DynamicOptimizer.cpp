#include <cstdlib>
#include <limits>
#include <iostream>
#include <math.h>

#include "DynamicOptimizer.h"
using namespace std;

int DynamicOptimizer::get_optimal_chain_order(int i, int j, vector<pair<int, int>> *chain_order) {
    if (i == j) {
        return i;
    } else {
        int k = get_optimal_chain_order(i, s_[i][j], chain_order);
        int l = get_optimal_chain_order(s_[i][j] + 1, j, chain_order);
        chain_order->emplace_back(k, l);

        return -1;
    }
}

void DynamicOptimizer::optimal_matrix_chain_order(vector<int> dims) {
    int len, i, j, k;
    long double temp, cost;

    n_--;
    m_ = (long double**)malloc(n_ * sizeof(long double*));
    for (i = 0; i < n_; ++i) {
        m_[i] = (long double*)calloc(n_, sizeof(long double));
    }

    s_ = (long double**)malloc(n_ * sizeof(long double*));
    for (i = 0; i < n_; ++i) {
        s_[i] = (long double*)calloc(n_, sizeof(long double));
    }

    for (len = 1; len < n_; ++len) {
        for (i = 0; i < n_ - len; ++i) {
            j = i + len;
            m_[i][j] = numeric_limits<long double>::max();
            for (k = i; k < j; ++k) {

                temp = dims[i] * (long double)dims[k + 1];
                temp *= dims[j + 1];
                cost = m_[i][k] + m_[k + 1][j] + temp;

                if (cost < m_[i][j]) {
                    m_[i][j] = cost;
                    s_[i][j] = k;
                }
            }
        }
    }
    // cout << "COST: " << m_[1][n_-1] << endl;
}

long double DynamicOptimizer::sparse_optimal_matrix_chain_order(vector<int> dims, vector<TransitionMatrix*> matrices) {
    int len, i, j, k;
    long double cost;

    n_--;

    m_ = (long double**)malloc(n_ * sizeof(long double*));
    for (i = 0; i < n_; ++i) {
        m_[i] = (long double*)calloc(n_, sizeof(long double));
    }

    s_ = (long double**)malloc(n_ * sizeof(long double*));
    for (i = 0; i < n_; ++i) {
        s_[i] = (long double*)calloc(n_, sizeof(long double));
    }

    // keep track of intermediate results sparsity
    long double sparsities[n_][n_];
    for (i = 0; i < n_; ++i) {
        for (j = 0; j < n_; ++j) {
            sparsities[i][j] = 0;
        }
    }

    for (len = 1; len < n_; ++len) {

        for (i = 0; i < n_ - len; ++i) {
            j = i + len;
            m_[i][j] = numeric_limits<long double>::max();
            for (k = i; k < j; ++k) {

                long double a_sparsity = sparsities[i][k];
                if (!a_sparsity) {
                    a_sparsity = matrices[i]->getSparsity();
                }

                long double b_sparsity = sparsities[k + 1][j];
                if (!b_sparsity) {
                    b_sparsity = matrices[j]->getSparsity();
                }
                long double result_sparsity = DynamicOptimizer::compute_sparsity(a_sparsity, b_sparsity, dims[k+1]);

                long double cur_sparse_cost =  DynamicOptimizer::compute_sparse_cost(a_sparsity, b_sparsity, result_sparsity, dims[i], dims[k+1], dims[j+1]);

                cost = m_[i][k] + m_[k + 1][j] + cur_sparse_cost;

                if (cost < m_[i][j]) {
                    m_[i][j] = cost;
                    s_[i][j] = k;
                    sparsities[i][j] = result_sparsity;
                }
            }
        }
    }

    // return min cost
    return m_[0][n_-1];
}

long double DynamicOptimizer::compute_sparse_cost(long double a_sparsity, long double b_sparsity, long double c_sparsity, int a_rows, int b_rows, int b_cols) {
    //long double nnzA_per_col = a_sparsity * (long double) a_rows;
    //long double nnzB_per_col = b_sparsity * (long double) b_rows;
    //return nnzA_per_col * ((long double) nnzB_per_col * b_cols);
    long double tempA = a_rows * ((long double)(a_sparsity * b_rows));
    long double tempB = (((long double) a_rows * b_rows) * a_sparsity * b_cols) * b_sparsity;
    long double tempC = ((long double)(a_rows * c_sparsity)) * b_cols ;

    return tempA + tempB + tempC;
}

long double DynamicOptimizer::compute_sparsity(long double a_sparsity, long double b_sparsity, int common_dim) {
    return (1 - pow(1 - a_sparsity * b_sparsity, common_dim));
}

long double DynamicOptimizer::estimateResultMemory(vector<int> dims, vector<TransitionMatrix*> matrices) {

    long double result_sparsity = matrices[0]->getSparsity();

    for (int i=1; i<matrices.size()-1; i++){
        result_sparsity = DynamicOptimizer::compute_sparsity(result_sparsity, matrices[i]->getSparsity(), dims[i]);
    }

    // return result_sparsity * dims[0] * dims[dims.size()-1];
    long double memSize = result_sparsity * dims[0] * dims[dims.size()-1] * 2 * sizeof(int);

    // size of storing outer-starts
    memSize += dims[0] * sizeof(int);

    return memSize / (1024.0 * 1024.0);
}

void DynamicOptimizer::print_optimal_chain_order(int i, int j) {
    //cout << i << " "<< j << endl;
    if (i == j) {
        printf("%c", i + 65);
    } else {
        printf("(");
        print_optimal_chain_order(i, s_[i][j]);
        print_optimal_chain_order(s_[i][j] + 1, j);
        printf(")");
    }
}

DynamicOptimizer::~DynamicOptimizer() {
    for(int i = 0; i < n_; i++) {
        free(m_[i]);
    }
    free(m_);

    for(int i = 0; i < n_; ++i) {
        free(s_[i]);
    }
    free(s_);
}