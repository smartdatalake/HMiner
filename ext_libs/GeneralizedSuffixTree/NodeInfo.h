//
// Created by serafeim on 22/8/2019.
//

#ifndef HRANK_CACHEENTRY_H
#define HRANK_CACHEENTRY_H


#include "STnode.h"

class NodeInfo {

public:
    long _str_id;
    long _start;
    long _size;
    STnode* _node_ptr;

    NodeInfo(long str_id, long start, long size, STnode *node_ptr)
        : _str_id(str_id), _start(start), _size(size), _node_ptr(node_ptr) {}

};

#endif //HRANK_CACHEENTRY_H
