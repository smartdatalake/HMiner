#include <iostream>
#include <cstring>
#include "./ext_libs/Eigen/Sparse"

#include "Utils.h"
#include "Base/Executor.h"
#include "Base/Config.h"

using namespace std;
using namespace Eigen;

const char *QUERY_FILE = "-qf";
const char *NODES_DIR_ARG = "-indir";
const char *RELATIONS_DIR_ARG = "-idir";
const char *CONSTRAINTS_FILE_ARG = "-c";
const char *ALGORITHM_ARG = "-algo";
const char *ADAPTIVE_ARG = "-ad";
const char *MAX_MEMORY_ARG = "-mem";
const char *OUTPUT_DIR_ARG = "-out";

int main(int argc, char* argv[]) {

    Config config;

    int i = 1;
    while (i < argc) {

        if (!strcmp(argv[i], QUERY_FILE)) {
            i = Utils::checkArg(i, argc);
            config.setQueryFile(argv[i]);

        } else if (!strcmp(argv[i], NODES_DIR_ARG)) {
            i = Utils::checkArg(i, argc);
            config.setNodesDir(argv[i]);

        } else if (!strcmp(argv[i], RELATIONS_DIR_ARG)) {
            i = Utils::checkArg(i, argc);
            config.setRelationsDir(argv[i]);

        } else if (!strcmp(argv[i], CONSTRAINTS_FILE_ARG)) {
            i = Utils::checkArg(i, argc);
            config.setConstraintsFile(argv[i]);

        } else if (!strcmp(argv[i], ALGORITHM_ARG)) {
            i = Utils::checkArg(i, argc);

            if (!strcmp(argv[i], "Seq")) {
                config.setAlgorithm(algorithm_type::Seq);
            } else if (!strcmp(argv[i], "DynP")) {
                config.setAlgorithm(algorithm_type::DynP);
            } else {
                perror("Unrecognised Algorithm given");
                Utils::usage();
                exit(EXIT_FAILURE);
            }
        } else if (!strcmp(argv[i], ADAPTIVE_ARG)) {
            i = Utils::checkArg(i, argc);
            config.setAdaptive(!strcmp(argv[i], "1"));
        } else if (!strcmp(argv[i], MAX_MEMORY_ARG)) {
            i = Utils::checkArg(i, argc);
            config.setMaxMemory(atoi(argv[i]));
        } else if (!strcmp(argv[i], OUTPUT_DIR_ARG)) {
            i = Utils::checkArg(i, argc);
            config.printOutputDir(argv[i]);
        } else {
            Utils::usage();
            exit(1);
        }
        i++;
    }

    #ifdef DEBUG_MSG
        config.printArgs();
    #endif

    Executor* exec = new Executor(&config);
    exec->batch_run();
    delete exec;

    return 0;
}