//
// Created by serafeim on 28/5/2019.
//

#include <iostream>
#include <fstream>
#include <ctime>
#include <unistd.h>
#ifdef __APPLE__
    #include <sys/wait.h>
#else
    #include <wait.h>
#endif
#include "TransitionMatrix.h"
#include "../ext_libs/Eigen/Sparse"
#include "../Utils.h"

using namespace std;
using namespace Eigen;

void TransitionMatrix::createFromTriplets(vector<Triplet<int>>* triplets) {
    this->_matrix->setFromTriplets(triplets->begin(), triplets->end());
    this->_matrix->makeCompressed();
}

void TransitionMatrix::print() {
    cout << *_matrix << endl;
}

SparseMatrix<int, RowMajor>* TransitionMatrix::get_matrix() const {
    return _matrix;
}

const string &TransitionMatrix::get_relation() const {
    return _relation;
}

TransitionMatrix* TransitionMatrix::dot(TransitionMatrix *a, TransitionMatrix *b) {
    string res_relation = a->get_relation() + "_" + b->get_relation();
    auto *result = new SparseMatrix<int, RowMajor>();
    *result = (*(a->get_matrix()) * *(b->get_matrix()));       // multiply and prune zeros

    return new TransitionMatrix(res_relation, result, result->rows(), result->cols());
}

void TransitionMatrix::write_to_file(ofstream &fd, Eigen::SparseMatrix<int, RowMajor> *tmp_result) {
    for (int k=0; k < (*tmp_result).outerSize(); ++k) {
        for (Eigen::SparseMatrix<int, RowMajor>::InnerIterator it(*tmp_result, k); it; ++it) {
            fd << it.row() << "\t" << it.col() << "\t" << it.value() << "\n";
        }
    }
}

int TransitionMatrix::get_rows() const {
    return _rows;
}

int TransitionMatrix::get_cols() const {
    return _cols;
}
