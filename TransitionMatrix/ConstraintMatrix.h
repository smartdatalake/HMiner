//
// Created by serafeim on 27/5/2019.
//

#ifndef HRANK_CONSTRAINTMATRIX_H
#define HRANK_CONSTRAINTMATRIX_H

#include "../libs/eigen/Eigen/Sparse"
#include <string>
#include <map>

using namespace std;
using  namespace Eigen;

typedef Eigen::Triplet<int> T;

class ConstraintMatrix {
private:
    string _node_name;
    string _node_data_file;
    int _dimension;

    SparseMatrix<int, RowMajor> *_matrix;
public:
    ConstraintMatrix(string node_name, string node_data_file, int dimension)
     : _node_name(node_name), _node_data_file (node_data_file), _dimension (dimension) {
         this->_matrix = new SparseMatrix<int, RowMajor>(dimension, dimension);
     }

    ~ConstraintMatrix() { delete _matrix; }

    int build(string expression);
    void print();

    SparseMatrix<int, RowMajor>* getMatrix() const;
    long double nonZeros() const;
    int getDimension() const;

};


#endif //HRANK_CONSTRAINTMATRIX_H
