//
// Created by serafeim on 27/5/2019.
//

#ifndef HRANK_HRANKSY_H
#define HRANK_HRANKSY_H

#include "../Base/Config.h"
#include "../TransitionMatrix/TransitionMatrix.h"
#include "../TransitionMatrix/ConstraintMatrix.h"

class HRankSY {
private:
    Config* _config;

public:
    
    HRankSY(Config *config);
    
    TransitionMatrix* run(string metapath, vector<TransitionMatrix*> matrices, vector<int> dimensions);
};


#endif //HRANK_HRANKSY_H
