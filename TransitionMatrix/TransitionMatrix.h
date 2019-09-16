//
// Created by serafeim on 28/5/2019.
//

#ifndef HRANK_TRANSITIONMATRIX_H
#define HRANK_TRANSITIONMATRIX_H

#include <string>
//#include <boost/numeric/ublas/matrix_sparse.hpp>
#include "../ext_libs/Eigen/Sparse"

using namespace std;

typedef Eigen::Triplet<int> T;

enum SplitFileType { rows, cols };
enum DataFileType { crs, ccs };

class TransitionMatrix {

private:
    string relation_;
    string crs_file_;
    string ccs_file_;

    bool has_crs_file_ { false };
    bool has_ccs_file_ { false };

    bool crs_sorted_ { false };
    bool ccs_sorted_ { false };

    bool in_memory_ { false };

    string relation_dir_;

    int rows_ { 0 };
    int cols_ { 0 };

    Eigen::SparseMatrix<int> *matrix_ { nullptr };


    static long double estimate_mem(TransitionMatrix *a, TransitionMatrix *b, const int *a_rows, const int *b_rows);
    static void get_table_parts(TransitionMatrix *a, TransitionMatrix *b, int *a_part_rows, int *b_part_cols, int max_mem);

public:
    explicit TransitionMatrix(string relation, const string& relation_dir, int rows, int cols,
            bool has_crs_file = true, bool has_ccs_file = true, bool crs_sorted = true, bool ccs_sorted = true) {

        relation_ = relation;
        relation_dir_ = relation_dir;
        crs_file_ = relation_dir + relation_ + ".crs";
        ccs_file_= relation_dir + relation_ + ".ccs";
        rows_ = rows;
        cols_ = cols;
        has_crs_file_ = has_crs_file;
        has_ccs_file_ = has_ccs_file;
        crs_sorted_ = crs_sorted;
        ccs_sorted_ = ccs_sorted;
    }

    TransitionMatrix() = default;

    TransitionMatrix(string relation, const string& relation_dir, Eigen::SparseMatrix<int> *matrix, int rows, int cols) {
        relation_ = relation;
        relation_dir_ = relation_dir;
        crs_file_ = relation_dir + relation_ + ".crs";
        ccs_file_= relation_dir + relation_ + ".ccs";
        matrix_ = matrix;
        rows_ = rows;
        cols_ = cols;
        in_memory_ = true;
    }

    ~TransitionMatrix() { delete matrix_; }

    Eigen::SparseMatrix<int>* get_matrix() const;
    unsigned long long int non_zeros();

    const string &get_relation() const;

    double read();
    void print();

    static TransitionMatrix* dot(TransitionMatrix *a, TransitionMatrix *b, string tmp_dir);
    static TransitionMatrix* adaptive_dot(TransitionMatrix *a, TransitionMatrix *b, unsigned long long *reads,
                                          int max_mem, string tmp_dir);

    int get_rows() const;

    int get_cols() const;

    const string &get_relation_dir() const;

    bool is_in_memory() const;

    Eigen::SparseMatrix<int>* read_blocks(int num_rows, ifstream &fd, SplitFileType split);

    const string &get_ccs_file() const;

    const string &get_crs_file() const;

    ofstream open_crs_file();
    ofstream open_ccs_file();

    void write_to_file(ofstream &fd, Eigen::SparseMatrix<int> *tmp_result, DataFileType fileType);
    int copy_files(string src, string trg, DataFileType trg_type);

    int sort_crs_file();
    int sort_ccs_file();

    bool has_crs_file() const;

    bool has_ccs_file() const;

    bool is_crs_sorted() const;

    bool is_ccs_sorted() const;

    void prepare_crs_file();

    void prepare_ccs_file();


};




#endif //HRANK_TRANSITIONMATRIX_H
