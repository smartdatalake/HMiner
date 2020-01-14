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
#include "../ext_libs/Eigen/Sparse"

using namespace std;
using namespace Eigen;

void ConstraintMatrix::build() {

    std::vector<Triplet<int>> triplet_list;

    map<int, string> field_constraints;

    // map constraints to column indexes
    for(auto it = constraints_.cbegin(); it != constraints_.cend(); ++it) {
        int column_idx = Utils::get_column_idx(node_data_file_, it->first);
        field_constraints.insert(make_pair(column_idx, it->second));
    }

    int id_idx = Utils::get_column_idx(node_data_file_, "id");

    ifstream infile(node_data_file_);

    if (infile.good()) {
        string line;

        while (getline(infile, line)) {

            bool valid = true;

            int i = 0;
            int id = -1;
            std::istringstream iss(line);
            while (getline(iss, line, '\t')) {
                if (id_idx == i) {
                    id = strtol(line.c_str(), nullptr, 10);
                }

                auto it = field_constraints.find(i);
                if (it != field_constraints.end()) {
                    if (it->second.compare(line)) {
                        valid = false;
                        break;
                    }
                }
                i++;

            }

            if (valid) {
                triplet_list.emplace_back(id, id, 1);
            }
        }

        this->_matrix->setFromTriplets(triplet_list.begin(), triplet_list.end());
    }

    infile.close();
}

int ConstraintMatrix::get_dimension() const {
    return dimension_;
}

SparseMatrix<int, RowMajor>* ConstraintMatrix::get_matrix() const {
    return _matrix;
}

void ConstraintMatrix::print_constraints() {
    for(auto it = constraints_.cbegin(); it != constraints_.cend(); ++it) {
        cout << "Constraint on column: " << it->first << " -> " << it->second << endl;
    }
}

void ConstraintMatrix::print() {
    for (int k=0; k < (*_matrix).outerSize(); ++k) {
        for (Eigen::SparseMatrix<int, RowMajor>::InnerIterator it(*_matrix, k); it; ++it) {
            std::cout << "(" << it.row() << ","; // row index
            std::cout << it.col() << ")\t"; // col index (here it is equal to k)
        }
    }
}
