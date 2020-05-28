#include "CBS1.h"
#include "../TransitionMatrix/TransitionMatrix.h"
#include "../MatrixMultiplier/MatrixMultiplier.h"
#include "../Cache/LRUCache/LRUCache.h"
#include "../Cache/PGDSCache/PGDSCache.h"
#include "../Cache/CacheItem.h"
#include <iostream>
using namespace std;

CBS1::CBS1(Config *config) {
    this->_config = config;
    if (this->_config->getCachePolicy() == cache_type::PGDS) {
        this->_cache = new PGDSCache(config->getCacheSize(), config->getCacheThreshold());
    } else {
        this->_cache = new LRUCache(config->getCacheSize(), config->getCacheThreshold());
    }

}

TransitionMatrix* CBS1::run(string metapath, vector<TransitionMatrix*> &matrices, vector<int> dimensions, bool &resultIsCached) {
    resultIsCached = false;
    string query = metapath + " " + this->_config->getConstraint();

    // return result if already in cache
    auto it = this->_result_map.find(query);
    if (it != this->_result_map.end() && it->second->isValid()) {

        this->_succReads++;
        resultIsCached = true;

        this->_cache->refer(it->second);
        return it->second->getMatrix();
    }

    this->_failedReads++;

    TransitionMatrix *result = nullptr;
    long double cost = 0;

    result = MatrixMultiplier::dynamic(matrices, this->_config->getThreshold(), dimensions, this->_config->getDynOptimizerType(), true, cost);

    auto *item = new CacheItem(this->_config->getConstraint(), result, cost, 0);

    if (item->getMemory() <= this->_cache->getItemThreshold()) {
        resultIsCached = true;

        // result was cached but is now invalid
        it = this->_result_map.find(query);
        if (it != this->_result_map.end()) {
            delete it->second;
            it->second = item;

        // cache result for first time 
        } else {
            this->_result_map.insert(pair<string, CacheItem*>(query, item));
        }
        this->_cache->refer(item);

        this->_writes++;
    }

    // this->_cache->display();

    return result;
}