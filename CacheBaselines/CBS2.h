#ifndef ADVANCED_BASELINE_H
#define ADVANCED_BASELINE_H

#include "../Base/Config.h"
#include "../Cache/Cache.h"
#include "../TransitionMatrix/TransitionMatrix.h"

#include <unordered_map> 

class CBS2 {
private:
    Config* _config;
    Cache *_cache { nullptr };
    unordered_map<string, CacheItem*> _result_map;   

    unsigned int _succReads = 0;
    unsigned int _failedReads = 0;
    unsigned int _writes = 0;

public:

    CBS2(Config *config);

    ~CBS2() {
        delete this->_cache;
    }

    TransitionMatrix* run(string metapath, vector<TransitionMatrix*> &matrices, vector<int> dimensions, bool &resultIsCached);
    TransitionMatrix* dynamic(vector<TransitionMatrix*> &matrices, int threshold,
        vector<int> dimensions, optimizer_type sparse_optimizer, vector<TransitionMatrix*> &tmp_results, bool delete_input);
    bool insertInCache(TransitionMatrix* result, string subpath);
    void insertResultsInCache(vector<TransitionMatrix*> tmp_results, string con);
    TransitionMatrix*  findSubpathToUse(vector<string> subpaths, string constraint_str);
    void findAllOccurences(std::vector<size_t> &vec, std::string data, string needle);
    TransitionMatrix* computeMetapathFromSubpath(string metapath, string cachedSubpath, vector<TransitionMatrix*> &matrices,
        vector<int> dimensions, TransitionMatrix* cachedMatrix, string constraint_str);

    unsigned int getCacheFailedReads() { return this->_failedReads; }
    unsigned int getCacheSuccReads() { return this->_succReads; }
    unsigned int getCacheWrites() { return this->_writes; }


};


#endif //SIMPLE_BASELINE_H
