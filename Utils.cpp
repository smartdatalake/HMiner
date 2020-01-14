//
// Created by serafeim on 27/5/2019.
//
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include "Utils.h"
//#include "Eigen/src/SparseCore/SparseMatrix.h"
#include "./ext_libs/Eigen/Sparse"

using namespace std;

void Utils::usage() {
    cerr << "run [-qf query_file] [-n nodes_dir] [-r relations_dir] [-co constraints_file] " << endl
         << " -qf query_file" << endl
         << "    query file that contains the metapaths to be used" << endl
         << " -indir nodes_dir" << endl
         << "    directory with node data files" << endl
         << " -idir relations_file" << endl
         << "    directory with relations files" << endl
         << " -c constraints_file" << endl
         << "    file with node constraints" << endl
         << " -algo algorithm to be used" << endl
         << "    available algorithms: { 'Seq', 'DynP' }" << endl
         << " -ad adaptive matrix multiplication (disk based)" << endl
         << "    values : {0, 1}" << endl
         << " -mem total memory to be used in case of adaptive matrix mulitplication" << endl
         << "    total memory in MB" << endl
         << " -out output directory" << endl
         << "    if not given results are printed on stdout" << endl;
}

int Utils::checkArg(int i, int max) {
    if (i == max) {
        usage();
        exit(1);
    }
    return i + 1;
}

int Utils::get_column_idx(string filename, string fieldName) {

    ifstream infile(filename);

    if (infile.good()) {
        string line;

        // read first line
        getline(infile, line);

        vector<string> tokens;
        boost::split(tokens, line, boost::is_any_of("\t"));
        for (unsigned int i=0; i<tokens.size(); i++) {
            if (tokens[i] == fieldName) {
                infile.close();
                return i;
            }
        }
        infile.close();
    }
    return -1;
}

int Utils::get_max_column_value(string filename, int column_idx) {
    ifstream infile;
    infile.open(filename);

    int max_value = -1;

    if (infile.good()) {
        string line;

        while (getline(infile, line)) {

            int i = 0;
            std::istringstream iss(line);
            while (getline(iss, line, '\t')) {
                if (i == column_idx) {
                    int cur_value = strtol(line.c_str(), NULL, 10);
                    if (cur_value > max_value) {
                        max_value = cur_value;
                    }
                }
                i++;

            }
        }
    }

    infile.close();
    return max_value;
}

void Utils::print(Eigen::SparseMatrix<int, RowMajor> *matrix_) {
    for (int k=0; k < (*matrix_).outerSize(); ++k)
    {
        for (Eigen::SparseMatrix<int, RowMajor>::InnerIterator it(*matrix_, k); it; ++it)
        {
            std::cout << it.row() << "\t" << it.col() << "\t" << it.value() << endl;
        }
    }
    std::cout << endl;
}

void Utils::log(string msg) {
   time_t now = time(0);
   tm *ltm = localtime(&now);

    cout << "[" << 1900 + ltm->tm_year;
    cout << "-" << 1 + ltm->tm_mon;
    cout << "-" <<  ltm->tm_mday;
    cout << " "<< 1 + ltm->tm_hour << ":";
    cout << 1 + ltm->tm_min << ":";
    cout << 1 + ltm->tm_sec << "] ";

    cout << msg;
}

void Utils::logTime(clock_t begin) {
    cout << " Time = ";
    cout << double(clock() - begin) / CLOCKS_PER_SEC << endl;
}

vector<TransitionMatrix*> Utils::slice(vector<TransitionMatrix*> matrices, size_t start, size_t len) {
    return vector<TransitionMatrix*>(matrices.begin() + start, matrices.begin() + start + len);
}

vector<int> Utils::slice(vector<int> matrices, size_t start, size_t len) {
    return vector<int>(matrices.begin() + start, matrices.begin() + start + len);
}