#ifndef MPPAGERANK_LRUCACHE_H
#define MPPAGERANK_LRUCACHE_H

#include <unordered_map>

#include "../CacheItem.h"
#include "../Cache.h"

using namespace std; 

class LRUCache: public Cache {

    public: 
        LRUCache(long double m, long double t): Cache(m, t) {}

        bool refer(CacheItem *item); 
};

#endif