#include <iostream>
#include <cstring>
#include "./libs/Eigen/Sparse"

#include "Utils.h"
#include "Base/Executor.h"
#include "Base/Config.h"

using namespace std;
using namespace Eigen;

const char *QUERY_FILE = "-qf";
const char *NODES_DIR = "-indir";
const char *RELATIONS_FILE = "-irdir";
const char *CONSTRAINTS_FILE_ARG = "-c";
const char *ALGORITHM = "-algo";
const char *OUTPUT_FILE = "-out";

int main(int argc, char* argv[]) {

    Config config;

    int i = 1;
    while (i < argc) {

        if (!strcmp(argv[i], QUERY_FILE)) {
            i = Utils::checkArg(i, argc);
            config.setQueryFile(argv[i]);

        } else if (!strcmp(argv[i], NODES_DIR)) {
            i = Utils::checkArg(i, argc);
            config.setNodesDir(argv[i]);

        } else if (!strcmp(argv[i], RELATIONS_FILE)) {
            i = Utils::checkArg(i, argc);
            config.setRelationsFile(argv[i]);

        } else if (!strcmp(argv[i], CONSTRAINTS_FILE_ARG)) {
            i = Utils::checkArg(i, argc);
            config.setConstraintsFile(argv[i]);

        } else if (!strcmp(argv[i], ALGORITHM)) {
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
        } else if (!strcmp(argv[i], OUTPUT_FILE)) {
            i = Utils::checkArg(i, argc);
            config.setOutputFile(argv[i]);
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
    exec->run();
    delete exec;

    return 0;
}