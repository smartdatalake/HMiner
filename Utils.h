//
// Created by serafeim on 27/5/2019.
//

#ifndef HRANK_UTILS_H
#define HRANK_UTILS_H

#include <string>
#include <vector>
#include "./libs/eigen/Eigen/Sparse"
#include "TransitionMatrix/TransitionMatrix.h"

using namespace std;

class Utils {
public:
    static void usage();
    static int checkArg(int i, int max);
    static int get_column_idx(string filename, string fieldName);
    static int get_max_column_value(string filename, int column_idx);
    static void print(Eigen::SparseMatrix<int, RowMajor> *matrix_);
    static void log(string msg);
    static void logProgress(string msg);
    static double diffTime(clock_t begin);
    static void logTime(clock_t begin);
    static void split(string line, vector<string> &tokens, char delim);
    static void printConstraint(tuple<string, string, string> constraint);

    // TODO: merge two functions below (templates?)
    static vector<TransitionMatrix*> slice(vector<TransitionMatrix*> matrices, size_t start, size_t len);
    static vector<int> slice(vector<int> matrices, size_t start, size_t len);

    static void trim(std::string &s);
};

enum algorithm_type { HRankSeq, HRankDynP, BS1, BS2, OTree };
enum optimizer_type { Dense, Sparse };
enum cache_type { LRU, PGDS };

#endif //HRANK_UTILS_H
