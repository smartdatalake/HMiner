//
// Created by serafeim on 27/5/2019.
//

#ifndef HRANK_CONSTRAINTMATRIX_H
#define HRANK_CONSTRAINTMATRIX_H

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include "../ext_libs/Eigen/Sparse"
#include <string>
#include <map>

using namespace std;
using  namespace Eigen;

typedef Eigen::Triplet<int> T;

class ConstraintMatrix {
private:
    string node_name_;
    string node_data_file_;
    map<string, string> constraints_;
    int dimension_;

    SparseMatrix<int> *matrix_;
public:
    ConstraintMatrix(string node_name, string node_data_file, map<string, string> constraints, int dimension)
     : node_name_(node_name), node_data_file_ (node_data_file), constraints_ (constraints), dimension_ (dimension) {}

    ~ConstraintMatrix() { delete matrix_; }

    void build();

    int get_dimension() const;

    void print_constraints();
    void print();

    SparseMatrix<int>* get_matrix() const;
};


#endif //HRANK_CONSTRAINTMATRIX_H
