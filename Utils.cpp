//
// Created by serafeim on 27/5/2019.
//
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Utils.h"
#include "./libs/Eigen/Sparse"

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

        string line, columnName;

        getline(infile, line);
        std::istringstream iss(line);

        int i = 0;
        while (getline(iss, columnName, '\t')) {

            if (columnName == fieldName) {
                infile.close();
                return i;
            }
            i++;
        }
    }
    infile.close();
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

    cout << msg << endl;
}

double Utils::diffTime(clock_t begin) {
    return double(clock() - begin) / CLOCKS_PER_SEC;
}

vector<TransitionMatrix*> Utils::slice(vector<TransitionMatrix*> matrices, size_t start, size_t len) {
    return vector<TransitionMatrix*>(matrices.begin() + start, matrices.begin() + start + len);
}

vector<int> Utils::slice(vector<int> matrices, size_t start, size_t len) {
    return vector<int>(matrices.begin() + start, matrices.begin() + start + len);
}

void Utils::split(string line, vector<string> &tokens, char delim) {
    std::istringstream iss(line);
    std::string token;
    while(std::getline(iss, token, delim)) {
        tokens.push_back(token);
    }
}

void Utils::printConstraint(tuple<string, string, string> constraint) {
    cout << "\t-\t[ C: " << get<0>(constraint) << ", P: " << get<1>(constraint) << ", V: " << get<2>(constraint) << " ]";
}

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends (in place)
void Utils::trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}