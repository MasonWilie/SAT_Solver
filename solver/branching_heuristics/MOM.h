#ifndef MOM_H
#define MOM_H

#include "MaxMinClause.h"

class MomsBranching : public MaxMinClauseHeuristic
{
public:
    MomsBranching(int num_vars,
                  LitMapUnique_t &lit_map_unique);

private:
    int Score(std::pair<int, int> lit_pair) const;

    const float k = 3.0;
};

#endif