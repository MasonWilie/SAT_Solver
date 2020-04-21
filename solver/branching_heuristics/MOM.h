#ifndef MOM_H
#define MOM_H

#include "MaxMinClause.h"

class MomsBranching : public MaxMinClauseHeuristic
{
public:
    MomsBranching(int num_vars,
                  PropMapUnique_t &prop_map_unique);

private:
    int Score(std::pair<int, int> prop_pair) const;

    const float k = 3.0;
};

#endif