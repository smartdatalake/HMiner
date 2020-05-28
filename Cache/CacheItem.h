#ifndef MPPAGERANK_CACHEITEM_H
#define MPPAGERANK_CACHEITEM_H

#include "../TransitionMatrix/TransitionMatrix.h"

#include <iostream>

// forward declaration of STnode
class STnode;

class CacheItem {
    private:
        bool _valid { false };
        TransitionMatrix *_matrix { nullptr };
        long double _memSize { 0 };
        long double _cost { 0 };
        long double _score { 0 };
        long double _score2 { 0 };
        int _freq { 0 };
        string _constraint;

    public: 

        CacheItem(string constraint, TransitionMatrix *m, long double cost, int freq) {
            this->_constraint = constraint;
            this->_matrix = m;
            this->_valid = true;
            this->_memSize = m->memory();
            this->_cost = cost;
            this->_freq = freq;
            this->_score = this->_cost / this->_memSize;
            this->_score2 = this->_freq * this->_cost / this->_memSize;
        }

        ~CacheItem() {
            if (this->_valid) {
                delete this->_matrix;
            }
        }
        
        string getConstraint() {
            return this->_constraint;
        }

        long double getMemory() const {
            return this->_memSize;
        }

        long double getCost() const {
            return this->_cost;
        }

        long double getScore() const {
            return this->_score;
        }

        long double getScore2() const {
            return this->_score2;
        }

        void setScore(long double score) {
            this->_score = score;
        }

        void setScore2(long double score) {
            this->_score2 = score;
        }

        bool isValid() const {
            return this->_valid;
        }

        TransitionMatrix* getMatrix() {
            return this->_matrix;
        }

        void deleteMatrix() {
            delete this->_matrix;
            this->_valid = false;
        }

        void setValid(bool v) {
            this->_valid = v;
        }

        void restoreScore(long double L) {
            this->_score = this->_cost / this->_memSize + L;
        }

        void restoreScore2(long double L) {
            this->_freq++;
            this->_score2 = this->_freq * this->_cost / this->_memSize + L;
        }

        void addCost(long double cost, long double L) {
            this->_cost += cost;
            this->_score2 = this->_freq * this->_cost / this->_memSize + L;
        }

        void substractCost(long double cost, long double L) {
            this->_cost -= cost;
            this->_score2 = this->_freq * this->_cost / this->_memSize + L;
        }

};

#endif