//
// Created by serafeim on 28/5/2019.
//

#ifndef HRANK_TRANSITIONMATRIX_H
#define HRANK_TRANSITIONMATRIX_H

#include <string>
#include "../libs/eigen/Eigen/Sparse"

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

    TransitionMatrix(const TransitionMatrix *that)
        : _relation(that->getRelation()), _rows(that->rows()), _cols(that->cols()) {
        this->_matrix = new SparseMatrix<int, RowMajor>(*(that->getMatrix()));
    }
    
    ~TransitionMatrix() { delete _matrix; }

    int build(string relations_file);
    int buildFromSingleFile(string relations_file);
    
    SparseMatrix<int, RowMajor>* getMatrix() const;

    const string &getRelation() const;

    double read();
    void print();

    static TransitionMatrix* dot(TransitionMatrix *a, TransitionMatrix *b);

    int rows() const;
    int cols() const;
    long double nonZeros() const;

    void write(ofstream &fd, SparseMatrix<int, RowMajor> *tmp_result);
    void writeForPagerank(ofstream &fd, Eigen::SparseMatrix<int, RowMajor> *tmp_result);
};


#endif //HRANK_TRANSITIONMATRIX_H
