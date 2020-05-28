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
    if (algorithm == "HRankSeq") 
        config.setAlgorithm(algorithm_type::HRankSeq);
    else if (algorithm == "HRankDynP") 
        config.setAlgorithm(algorithm_type::HRankDynP);
    else if (algorithm == "CBS1")
        config.setAlgorithm(algorithm_type::BS1);
    else if (algorithm == "CBS2" ||  algorithm == "OTree")
        config.setAlgorithm(algorithm_type::BS2);
    else {
        cerr << "Algorithm argument should be one of { HRank, CBS1, CBS2, OTree }" << endl;
        return EXIT_FAILURE;
    }

    string cache_policy = params["cache_policy"];
    if (cache_policy == "PGDS")
        config.setCachePolicy(cache_type::PGDS);
    else
        config.setCachePolicy(cache_type::LRU);

    string dynamic_optimizer = params["dynamic_optimizer"];
    if (dynamic_optimizer == "Sparse") {
        config.setDynOptimizerType(optimizer_type::Sparse);
    } else if (dynamic_optimizer == "Dense") {
        config.setDynOptimizerType(optimizer_type::Dense);
    } else {
        perror("Unrecognised Dynamic Optimizer Type given");
        Utils::usage();
        exit(EXIT_FAILURE);
    }
    config.setOutputDir(params["hin_out"]);
    
    // this can also set to condensed, 
    // but this is not currently handled from pagerank
    config.setOutputType("verbose");    
    config.setCacheSize(params["cache_size"]);

    // check if config["ranking"]["threshold"] exists
    if (params["ranking"].find("threshold") != params["ranking"].end()) {
        config.setTreshold(params["ranking"]["threshold"]);
    }

    Executor* exec = new Executor(&config);
    exec->run(params);
    delete exec;

    return 0;
}