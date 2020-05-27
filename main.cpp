#include <iostream>
#include <cstring>
#include "Utils.h"
#include "Base/Executor.h"
#include "Base/Config.h"
#include "Base/FileParser.h"

using namespace std;
using namespace Eigen;

const char *QUERY_FILE = "-qf";
const char *NODES_DIR = "-indir";
const char *RELATIONS_FILE = "-irf";
const char *RELATIONS_DIR = "-irdir";
const char *CONSTRAINTS_FILE_ARG = "-c";
const char *ALGORITHM = "-algo";
const char *OUTPUT_FILE = "-out";
const char *CONFIG_FILE = "-c";

int main(int argc, char* argv[]) {

    if (argc != 3) {
        Utils::usage();
        return EXIT_FAILURE;
    }
    int i = Utils::checkArg(1, argc);

    // read params from config file
    json params = FileParser::readJson(argv[i]);

    // set parameters according to config file
    Config config;
    config.setConfigFile(argv[i]);
    config.setNodesDir(params["indir"]);
    if (!params["irdir"].empty())
        config.setRelationsDir(params["irdir"]);
    else 
        config.setRelationsFile(params["irf"]);
    
    string algorithm = params["algorithm"];
    if (algorithm == "HRank") 
        config.setAlgorithm(algorithm_type::HRank);
    else if (algorithm == "CBS1")
        config.setAlgorithm(algorithm_type::BS1);
    else if (algorithm == "CBS2" ||  algorithm == "OTree")
        config.setAlgorithm(algorithm_type::BS2);
    else {
        cerr << "Algorithm argument should be one of { HRank, CBS1, CBS2, OTree }" << endl;
        return EXIT_FAILURE;
    }

    config.setOutputDir(params["hin_out"]);
    config.setOutputType(params["analysis_out_type"]);

    Executor* exec = new Executor(&config);
    exec->run(params);
    delete exec;

    return 0;
}