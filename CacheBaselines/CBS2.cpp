
#include "CBS2.h"
#include "../TransitionMatrix/TransitionMatrix.h"
#include "../MatrixMultiplier/DynamicOptimizer.h"
#include "../MatrixMultiplier/MatrixMultiplier.h"
#include "../Cache/LRUCache/LRUCache.h"
#include "../Cache/PGDSCache/PGDSCache.h"
#include "../Cache/CacheItem.h"
#include <iostream>
using namespace std;

CBS2::CBS2(Config *config) {
        this->_config = config;
        
        if (this->_config->getCachePolicy() == cache_type::PGDS) {
            this->_cache = new PGDSCache(config->getCacheSize(), config->getCacheThreshold());
        } else {
            this->_cache = new LRUCache(config->getCacheSize(), config->getCacheThreshold());
        }    
}

TransitionMatrix* CBS2::run(string metapath, vector<TransitionMatrix*> &matrices, vector<int> dimensions, bool &resultIsCached) {
    resultIsCached = true;  // result is either in cache or will be inserted
    auto constraint_str = this->_config->getConstraint();

    vector<string> subpaths = MatrixMultiplier::getSubpathsFromDynamicPlanning(matrices, dimensions);

    TransitionMatrix* cached_matrix = findSubpathToUse(subpaths, constraint_str);
    if (cached_matrix) {
        this->_succReads++;

        string subpath = cached_matrix->get_relation();
        // cout << "USE: " <<  subpath << endl;
        
        return this->computeMetapathFromSubpath(metapath, subpath, matrices, dimensions, cached_matrix, constraint_str);
    }

    this->_failedReads++;
    
    vector<TransitionMatrix*> tmp_results;

    auto *result = this->dynamic(matrices, this->_config->getThreshold(), dimensions, this->_config->getDynOptimizerType(), tmp_results, true);

    this->insertResultsInCache(tmp_results, constraint_str);

    // this->_cache->display();

    return result;
}

TransitionMatrix* CBS2::computeMetapathFromSubpath(string metapath, string cachedSubpath, vector<TransitionMatrix*> &matrices,
        vector<int> dimensions, TransitionMatrix* cachedMatrix, string constraint_str) {


    #ifdef DEBUG_MSG 
        cout << "\t4.1) Use cached subpath: " << cachedSubpath << endl;
    #endif

    vector<size_t> cachedStartIndices;
    this->findAllOccurences(cachedStartIndices, metapath, cachedSubpath);

    vector<TransitionMatrix*> metapathMatrices;
    vector<int> metapathDimensions;

    // dimension of first relation is fixed
    metapathDimensions.push_back(dimensions[0]);

    size_t c_pos = 0;       // index in the cached subpath string
    size_t m_pos = 0;       // index in the metapath string

    while(m_pos < metapath.size()-1) {
        //cout << m_pos << ": " << metapath[m_pos] << endl;

        // if a cached result can be used from this position
        if (c_pos < cachedStartIndices.size() 
                    && cachedStartIndices[c_pos] == m_pos) {
            
            //cout << "Found at position: " << cached_start_indices[c_pos] << endl;

            metapathMatrices.push_back(cachedMatrix);

            c_pos++;
            m_pos += cachedSubpath.size() - 1;
            //cout << "Go to index: " << m_pos << endl;
            metapathDimensions.push_back(dimensions[m_pos]);

        } else {
            metapathMatrices.push_back(matrices[m_pos]);

            // this matrix will be deleted during matrix multiplication
            // so mark it not to be deleted from the Executor
            matrices[m_pos] = nullptr;

            m_pos++;
            metapathDimensions.push_back(dimensions[m_pos]);
        }
    }

    // result found in cache
    if (metapathMatrices.size() == 1) {
        return metapathMatrices[0];
    }

    vector<TransitionMatrix*> tmp_results;
    auto *result = this->dynamic(metapathMatrices, this->_config->getThreshold(), metapathDimensions, this->_config->getDynOptimizerType(), tmp_results, false);

    this->insertResultsInCache(tmp_results, constraint_str);
    return result;
}

void CBS2::findAllOccurences(std::vector<size_t> &vec, std::string data, string needle)
{
    // Get the first occurrence
    size_t pos = data.find(needle);

    // Repeat till end is reached
    while(pos != std::string::npos)
    {
        vec.push_back(pos);

        // Get the next occurrence from the current position
        pos = data.find(needle, pos + needle.size()-1);
    }
}

void CBS2::insertResultsInCache(vector<TransitionMatrix*> tmp_results, string constraint_str) {
    for (TransitionMatrix* t : tmp_results) {
        string rel = t->get_relation() + ' ' + constraint_str;
        // cout << rel << endl;

        auto it = this->_result_map.find(rel);
        if (it == this->_result_map.end()) {
            if (!this->insertInCache(t, rel)) {
                delete t;
            }
        } else {
            delete t;
        }
    }
}

TransitionMatrix* CBS2::dynamic(vector<TransitionMatrix*> &matrices, int threshold, vector<int> dimensions, optimizer_type sparse_optimizer, vector<TransitionMatrix*> &tmp_results, bool delete_input) {

    auto dynamic_optimizer = new DynamicOptimizer(dimensions.size());

    // clock_t begin = clock();

    // plan multiplications based on sparsity or only on matrix dimensions
    if (sparse_optimizer == optimizer_type::Sparse) {
        long double cost = dynamic_optimizer->sparse_optimal_matrix_chain_order(dimensions, matrices);
    } else if (sparse_optimizer == optimizer_type::Dense){
        dynamic_optimizer->optimal_matrix_chain_order(dimensions);
    } else {
        cerr << "Error: Not valid dynamic optimizer given" << endl;
        return nullptr;
    }

    // get ordering of multiplications
    vector<pair<int, int>> chain_order;
    dynamic_optimizer->get_optimal_chain_order(0, dimensions.size() - 2, &chain_order); 

    vector<TransitionMatrix*> temp;
    TransitionMatrix *tmp_ptr = nullptr;

    unsigned long long int reads = 0;

    for (auto it = chain_order.begin(); it != chain_order.end(); ++it) {

        int k = it->first;
        int l = it->second;
        int n = temp.size();

        if (k >= 0 && l >= 0) {

            TransitionMatrix *res = TransitionMatrix::dot(matrices[k], matrices[l], threshold);

            // delete multiplied matrices
            if (delete_input) {
                delete matrices[k];
                matrices[k] = nullptr;
                delete matrices[l];
                matrices[l] = nullptr;
            }

            tmp_results.push_back(res);

            temp.push_back(res);

        } else if (k == -1 && l >= 0) {

            tmp_ptr = temp[n-1];
            temp[n-1] = TransitionMatrix::dot(temp[n-1], matrices[l], threshold);

            // delete multiplied matrices
            if (delete_input) {
                delete matrices[l];
                matrices[l] = nullptr;
            }

            tmp_results.push_back(temp[n-1]);

        } else if (k >= 0 && l == -1) {

            temp[n-1] = TransitionMatrix::dot(matrices[k], temp[n-1], threshold);

            // delete multipliced matrices
            if (delete_input) {
                delete matrices[k];
                matrices[k] = nullptr;
            }

            tmp_results.push_back(temp[n-1]);
            
            
        } else {

            tmp_ptr = temp[n-2];
            temp[n-2] = TransitionMatrix::dot(temp[n-2], temp[n-1], threshold);


            tmp_results.push_back(temp[n-2]);
            temp.pop_back();
        }
    }

    delete dynamic_optimizer;

    return temp[0];
}

bool CBS2::insertInCache(TransitionMatrix* result, string subpath) {
    long double resultMemSize = result->memory();
    bool fitsInCache = (resultMemSize <= this->_cache->getItemThreshold());

    if (fitsInCache) {
        auto *item = new CacheItem(this->_config->getConstraint(), result, 0, 0);

        // result was cached but is now invalid
        auto it = this->_result_map.find(subpath);
        if (it != this->_result_map.end()) {
            delete it->second;
            it->second = item;

        // cache result for first time 
        } else {
            // cout << "INSERT " << subpath << endl;
            this->_result_map.insert(pair<string, CacheItem*>(subpath, item));
        }

        this->_cache->refer(item);
        this->_writes++;
        return true;
    }
    return false;
}

TransitionMatrix* CBS2::findSubpathToUse(vector<string> subpaths, string constraint_str) {
    size_t maxLen = 0;
    TransitionMatrix* cachedMatrix = nullptr;
    CacheItem *refPtr = nullptr;

    for (string s : subpaths) {
        string subpath_with_constraint = s + ' ' + constraint_str;

        auto it = this->_result_map.find(subpath_with_constraint);
        if (it != this->_result_map.end() && it->second->isValid()) {

            // find largest subpath to be used
            if (it->first.size() > maxLen) {
                maxLen = it->first.size();
                cachedMatrix = it->second->getMatrix();
                refPtr = it->second;
            }
        }
    }

    // refer in cache the item to be used
    if (refPtr) {
        this->_cache->refer(refPtr);
    }

    return cachedMatrix;
}