//
// Created by serafeim on 26/6/2019.
//

#include <cstdlib>
#include <limits>
#include <iostream>

#include "DynamicOptimizer.h"
using namespace std;

int DynamicOptimizer::get_optimal_chain_order(int i, int j, vector<pair<int, int>> *chain_order) {
    if (i == j) {
        return i;
    }
    else {
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
}

void DynamicOptimizer::print_optimal_chain_order(int i, int j) {
    //cout << i << " "<< j << endl;
    if (i == j) {
        printf("%c", i + 65);
    }
    else {
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
