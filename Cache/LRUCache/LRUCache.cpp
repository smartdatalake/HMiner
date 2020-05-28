
#include <iostream>
#include "LRUCache.h"

// Refers key `item` within the LRU cache 
bool LRUCache::refer(CacheItem *item) {

    long double itemMem = item->getMemory();

    // item is larger that the pre-defined threshold
    if (itemMem > this->_itemThreshold) {
        return false;
    }
 
    // not present in cache 
    if (this->_cacheRefs.find(item) == this->_cacheRefs.end()) { 

        // cache is full 
        while ((this->_curMemory + itemMem > this->_maxMemory) && !this->_cacheKeys.empty()) { 
        
            // delete least recently used element 
            CacheItem *last = this->_cacheKeys.back(); 

            // Pops the last elmeent 
            this->_cacheKeys.pop_back(); 
  
            // Erase the last 
            this->_cacheRefs.erase(last); 

            // free memory of deleted item
            long double freedMem = last->getMemory();
            this->_curMemory -= freedMem;
            last->deleteMatrix();
        } 

        // add memory of current item to total   
        this->_curMemory += itemMem;
    } 

    // present in cache 
    else {
        this->_cacheKeys.erase(_cacheRefs[item]); 
    }

    // update reference 
    this->_cacheKeys.push_front(item); 
    this->_cacheRefs[item] = this->_cacheKeys.begin(); 

    return true;
}
