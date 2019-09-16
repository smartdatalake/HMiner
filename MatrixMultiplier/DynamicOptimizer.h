//
// Created by serafeim on 26/6/2019.
//

#ifndef HRANK_DYNAMICOPTIMIZER_H
#define HRANK_DYNAMICOPTIMIZER_H

using namespace std;
#include <vector>

class DynamicOptimizer {
private:
    int n_;
    long double **m_;
    long double **s_;
public:
    DynamicOptimizer(int n) : n_(n) {};
    ~DynamicOptimizer();
    void optimal_matrix_chain_order(vector<int> dims);
    int get_optimal_chain_order(int i, int j, vector<pair<int, int>> *chain_order);
    void print_optimal_chain_order(int i, int j);
};


#endif //HRANK_DYNAMICOPTIMIZER_H
