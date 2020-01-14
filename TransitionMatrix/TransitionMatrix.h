//
// Created by serafeim on 28/5/2019.
//

#ifndef HRANK_TRANSITIONMATRIX_H
#define HRANK_TRANSITIONMATRIX_H

#include <string>
//#include <boost/numeric/ublas/matrix_sparse.hpp>
#include "../ext_libs/Eigen/Sparse"

using namespace std;
using namespace Eigen;

typedef Triplet<int> T;

class TransitionMatrix {

private:
    string _relation;
    SparseMatrix<int, RowMajor> *_matrix;

    int _rows;
    int _cols;

public:
    TransitionMatrix(string relation, int rows, int cols)
        :  _relation(relation), _rows(rows), _cols(cols) {
            this->_matrix = new SparseMatrix<int, RowMajor>(rows, cols);
        }

    TransitionMatrix(string relation, SparseMatrix<int, RowMajor> *matrix, int rows, int cols)
        :  _relation(relation), _matrix(matrix), _rows(rows), _cols(cols) {}

    ~TransitionMatrix() { delete _matrix; }

    SparseMatrix<int, RowMajor>* get_matrix() const;

    const string &get_relation() const;

    double read();
    void print();

    static TransitionMatrix* dot(TransitionMatrix *a, TransitionMatrix *b);

    int get_rows() const;

    int get_cols() const;

    void write_to_file(ofstream &fd, SparseMatrix<int, RowMajor> *tmp_result);

    void createFromTriplets(vector<Triplet<int>>* triplets);
};


#endif //HRANK_TRANSITIONMATRIX_H
