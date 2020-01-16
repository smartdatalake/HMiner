//
// Created by serafeim on 27/5/2019.
//

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <boost/numeric/ublas/matrix_sparse.hpp>

#include "ConstraintMatrix.h"
#include "../Utils.h"
#include "../libs/Eigen/Sparse"
#include "../libs/csv-filter/dsv_filter.hpp"

using namespace std;
using namespace Eigen;

void ConstraintMatrix::build() {

    dsv_filter filter;
    filter.set_input_delimiter("\t");

    if (!filter.load(this->node_data_file_)) {
        cout << "Could not open" << endl;
    }
    
    string expression = "(year >= 2017)";

    filter.add_filter(expression);

    vector<bool> selected_column_list(filter.column_count(), false);
    selected_column_list[0] = true;

    std::vector<Triplet<int>> triplet_list;
    for (std::size_t row = 1; row < filter.row_count(); ++row) {

        if (dsv_filter::e_match == filter[row]) {
            string id_str;
            filter.row(row, selected_column_list, id_str);
            int id = stoi(id_str);
            triplet_list.emplace_back(id, id, 1);
        }
    }

    this->_matrix->setFromTriplets(triplet_list.begin(), triplet_list.end());
    this->_matrix->makeCompressed();
}

int ConstraintMatrix::get_dimension() const {
    return dimension_;
}

SparseMatrix<int, RowMajor>* ConstraintMatrix::get_matrix() const {
    return _matrix;
}

void ConstraintMatrix::print() {
    for (int k=0; k < (*_matrix).outerSize(); ++k) {
        for (Eigen::SparseMatrix<int, RowMajor>::InnerIterator it(*_matrix, k); it; ++it) {
            std::cout << "(" << it.row() << ","; // row index
            std::cout << it.col() << ")\t"; // col index (here it is equal to k)
        }
    }
}
