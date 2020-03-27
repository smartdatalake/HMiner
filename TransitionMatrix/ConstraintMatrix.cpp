//
// Created by serafeim on 27/5/2019.
//

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>

#include "ConstraintMatrix.h"
#include "../Utils.h"
#include "../libs/csv-filter/dsv_filter.hpp"

using namespace std;
using namespace Eigen;

int ConstraintMatrix::build(string expression) {

    dsv_filter filter;
    filter.set_input_delimiter("\t");

    if (!filter.load(this->_node_data_file)) {
        return -1;
    }

    filter.add_filter(expression);

    vector<bool> selected_column_list(filter.column_count(), false);
    selected_column_list[0] = true;

    std::vector<Triplet<double>> triplet_list;
    for (std::size_t row = 1; row < filter.row_count(); ++row) {

        if (dsv_filter::e_match == filter[row]) {
            string id_str;
            filter.row(row, selected_column_list, id_str);
            int id = stoi(id_str);
            triplet_list.emplace_back(id, id, 1.0);
        }
    }

    this->_matrix->setFromTriplets(triplet_list.begin(), triplet_list.end());
    this->_matrix->makeCompressed();

    return 0;
}

int ConstraintMatrix::getDimension() const {
    return _dimension;
}

SparseMatrix<double, RowMajor>* ConstraintMatrix::getMatrix() const {
    return _matrix;
}

void ConstraintMatrix::print() {
    for (int k=0; k < (*_matrix).outerSize(); ++k) {
        for (Eigen::SparseMatrix<double, RowMajor>::InnerIterator it(*_matrix, k); it; ++it) {
            std::cout << "(" << it.row() << ","; // row index
            std::cout << it.col() << ")\t"; // col index (here it is equal to k)
        }
    }
}

long double ConstraintMatrix::nonZeros() const {
    return this->_matrix->nonZeros();
}
