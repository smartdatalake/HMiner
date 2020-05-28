#ifndef MPPAGERANK_GDSCACHE3_H
#define MPPAGERANK_GDSCACHE3_H

#include <unordered_map>

#include "../CacheItem.h"
#include "../Cache.h"

using namespace std; 

class PGDSCache: public Cache {

    private: 
        long double _L { 0 };
    public: 
        PGDSCache(long double m, long double t): Cache(m, t) { }
        
        bool refer(CacheItem *item); 
};

#endif