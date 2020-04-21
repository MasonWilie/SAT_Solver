#ifndef BOHM_H
#define BOHM_h

#include "MaxMinClause.h"

class BohmsBranching : public MaxMinClauseHeuristic
{
public:
    BohmsBranching(int num_vars,
                   PropMapUnique_t &prop_map_unique);

private:
    int Score(std::pair<int, int> prop_pair) const;

    const int alpha = 1;
    const int beta = 2;
};

#endif
