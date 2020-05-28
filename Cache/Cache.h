#ifndef MPPAGERANK_CACHE_H
#define MPPAGERANK_CACHE_H

#include <unordered_map>

#include "CacheItem.h"

using namespace std; 

class Cache {

    protected:
        long double _maxMemory { 0 };        // in MB
        long double _curMemory { 0 };        // in MB
        long double _itemThreshold { 0 };   // do not insert cache items larger than this threshold (typically it is set at 80% of the cache size)

        // store keys of cache 
        list<CacheItem*> _cacheKeys;        

        // store references of key in cache 
        unordered_map<CacheItem*, list<CacheItem*>::iterator> _cacheRefs; 

    public: 
        Cache(long double m, long double t) {
            this->_maxMemory = m;
            this->_itemThreshold = t;
        }

        virtual ~Cache() {}

        virtual bool refer(CacheItem *item) = 0; 

        // Function to display contents of cache 
        void display() 
        { 
            cout << endl << "CACHE = [" << endl;
            // Iterate and print all the elements in cache
            for (list<CacheItem*>::iterator it = this->_cacheKeys.begin(); it != this->_cacheKeys.end(); it++) {
                cout << "\t" << (*it)->getMatrix()->get_relation() << " { Cost: " << (*it)->getCost() << ", Mem: " << (*it)->getMemory() << ", Score: " << (*it)->getScore() << ", Score2: " << (*it)->getScore2() << " }," << endl; 
            }
            cout << "] (TOTAL: " << this->_curMemory << ")" << endl << endl;
        } 

        long double getSize() {
            return this->_maxMemory;
        }

        long double getItemThreshold() {
            return this->_itemThreshold;
        }
};

#endif