#ifndef HRANK_DYNAMICOPTIMIZER_H
#define HRANK_DYNAMICOPTIMIZER_H

using namespace std;
#include <vector>
#include "../TransitionMatrix/TransitionMatrix.h"

class DynamicOptimizer {
private:
    int n_;
    long double **m_;
    long double **s_;
public:
    DynamicOptimizer(int n) : n_(n) {};
    ~DynamicOptimizer();

    void optimal_matrix_chain_order(vector<int> dims);
    long double sparse_optimal_matrix_chain_order(vector<int> dims, vector<TransitionMatrix*> matrices);
    void mnc_optimal_matrix_chain_order(vector<int> dims, vector<TransitionMatrix*> matrices);

    int get_optimal_chain_order(int i, int j, vector<pair<int, int>> *chain_order);
    void print_optimal_chain_order(int i, int j);
    static long double compute_sparsity(long double a_sparsity, long double b_sparsity, int common_dim);
    static long double compute_sparse_cost(long double a_sparsity, long double b_sparsity, long double c_sparsity, int a_rows, int b_rows, int b_cols);
    static long double estimateResultMemory(vector<int> dims, vector<TransitionMatrix*> matrices);
};


#endif //HRANK_DYNAMICOPTIMIZER_H
