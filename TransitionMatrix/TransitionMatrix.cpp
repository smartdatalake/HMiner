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
#include "../libs/Eigen/Sparse"
#include "../Utils.h"
#include "../libs/csv-filter/dsv_filter.hpp"

using namespace std;
using namespace Eigen;

int TransitionMatrix::build(string relations_file) {

    dsv_filter filter;
    filter.set_input_delimiter("\t");

    if (!filter.load(relations_file)) {
        return -1;
    }

    // find all relations of this type
    string expression = "type = '" + this->_relation + "'";
    filter.add_filter(expression);

    // select src and dest from relation file (i.e. 2nd & 3d column)
    vector<bool> selected_column_list { false, true, true };

    std::vector<Triplet<int>> triplet_list;
    for (std::size_t row = 1; row < filter.row_count(); ++row) {

        if (dsv_filter::e_match == filter[row]) {
            string row_str;
            filter.row(row, selected_column_list, row_str);

            // selected columns are returned in a string with '|' as separator
            int pos = row_str.find('|');

            // split source and destination and add to tiplets list
            int src = stoi(row_str.substr(0, pos));
            int dest = stoi(row_str.substr(pos + 1, row_str.size () - pos -1));
            triplet_list.emplace_back(src, dest, 1);
        }
    }

    // create matrix from triplets
    this->_matrix->setFromTriplets(triplet_list.begin(), triplet_list.end());
    this->_matrix->makeCompressed();

    return 0;
}

void TransitionMatrix::print() {
    cout << *_matrix << endl;
}

SparseMatrix<int, RowMajor>* TransitionMatrix::getMatrix() const {
    return this->_matrix;
}

long double TransitionMatrix::nonZeros() const {
    return this->_matrix->nonZeros();
}

const string &TransitionMatrix::getRelation() const {
    return _relation;
}

TransitionMatrix* TransitionMatrix::dot(TransitionMatrix *a, TransitionMatrix *b) {
    string res_relation = a->getRelation() + "_" + b->getRelation();
    auto *result = new SparseMatrix<int, RowMajor>();
    *result = (*(a->getMatrix()) * *(b->getMatrix())).pruned();       // multiply and prune zeros

    return new TransitionMatrix(res_relation, result, result->rows(), result->cols());
}

void TransitionMatrix::write(ofstream &fd, Eigen::SparseMatrix<int, RowMajor> *tmp_result) {
    for (int k=0; k < (*tmp_result).outerSize(); ++k) {
        for (Eigen::SparseMatrix<int, RowMajor>::InnerIterator it(*tmp_result, k); it; ++it) {
            fd << it.row() << "\t" << it.col() << "\t" << it.value() << "\n";
        }
    }
}

int TransitionMatrix::rows() const {
    return _rows;
}

int TransitionMatrix::cols() const {
    return _cols;
}
