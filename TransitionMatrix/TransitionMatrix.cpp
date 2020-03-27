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
#include "../Utils.h"
#include "../libs/csv-filter/dsv_filter.hpp"

using namespace std;
using namespace Eigen;

int TransitionMatrix::buildFromSingleFile(string relations_file) {

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

SparseMatrix<double, RowMajor>* TransitionMatrix::getMatrix() const {
    return this->_matrix;
}

long double TransitionMatrix::nonZeros() const {
    return this->_matrix->nonZeros();
}

const string &TransitionMatrix::getRelation() const {
    return _relation;
}

TransitionMatrix* TransitionMatrix::dot(TransitionMatrix *a, TransitionMatrix *b, int threshold) {
    string res_relation = a->getRelation() + "_" + b->getRelation();
    auto *result = new SparseMatrix<double, RowMajor>();

    if (threshold == -1) {
        *result = (*(a->getMatrix()) * *(b->getMatrix())).pruned();       // multiply and prune zeros
    } else {
        *result = (*(a->getMatrix()) * *(b->getMatrix())).pruned(threshold, 1);       // multiply and prune values less than threshold
    }
    

    return new TransitionMatrix(res_relation, result, result->rows(), result->cols());
}

void TransitionMatrix::writeForPagerank(ofstream &fd, Eigen::SparseMatrix<double, RowMajor> *tmp_result) {
    long double initial_score = 1.0 / tmp_result->rows();

    for (int key=1; key < tmp_result->rows(); ++key) {

        string values;
        int length = 0;
        for (Eigen::SparseMatrix<double, RowMajor>::InnerIterator it(*tmp_result, key); it; ++it) {
            
            if (!values.empty()) {
                values += ",";
            }

            values += to_string(it.col());
            length++;
        }

        if (values.empty()){
            values = "0";
        }

        fd << key << "\t" << values << "|" << length << "|" << initial_score << "\t0\t0" << "\n";
    }   
}

void TransitionMatrix::write(ofstream &fd, Eigen::SparseMatrix<double, RowMajor> *tmp_result) {

    for (int i=0; i < (*tmp_result).rows(); ++i) {
        for (Eigen::SparseMatrix<double, RowMajor>::InnerIterator it(*tmp_result, i); it; ++it) {
            fd << it.row() << "\t" << it.col() << "\t" << it.value() << "\n";
        }
    }
}

void TransitionMatrix::writeCondensed(ofstream &fd, Eigen::SparseMatrix<double, RowMajor> *tmp_result) {

    for (int i=0; i < (*tmp_result).rows(); ++i) {
        int srcNode = -1;
        bool added = false;

        for (Eigen::SparseMatrix<double, RowMajor>::InnerIterator it(*tmp_result, i); it; ++it) {
            if (srcNode != it.row()) {
                srcNode = it.row();
                fd << srcNode << "\t" << it.col() << "|" << it.value();
            } else {
                fd << ";" << it.col() << "|" << it.value();
            }
            added = true;
        }
        if (added)
            fd << "\n";
    }
}

int TransitionMatrix::rows() const {
    return _rows;
}

int TransitionMatrix::cols() const {
    return _cols;
}

int TransitionMatrix::build(string relations_dir) {

    std::vector< Eigen::Triplet<double> > tripletList;
    string relations_file = relations_dir + this->_relation + ".csv";
    ifstream infile(relations_file);

    if (!infile.good()) {
        return -1;
    }

    string line;

    while (getline(infile, line)) {

        int i = 0;
        int src = -1, dest = -1;
        double val = 1;
        std::istringstream iss(line);

        while (getline(iss, line, '\t')) {
            if (i == 0) {
                src = strtol(line.c_str(), nullptr, 10);
            } else if (i == 1){
                dest = strtol(line.c_str(), nullptr, 10);
            } else if (i == 2) {
                val = stod(line);
            }
            i++;
        }

        tripletList.emplace_back(src, dest, val);
    }

    this->_matrix->setFromTriplets(tripletList.begin(), tripletList.end());
    this->_matrix->makeCompressed();

    infile.close();
    
    return 0;
}
