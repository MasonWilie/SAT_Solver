#include "MOM.h"

MomsBranching::MomsBranching(int num_vars,
                             PropMapUnique_t &prop_map_unique)
    : MaxMinClauseHeuristic(num_vars, prop_map_unique)
{
}

int MomsBranching::Score(std::pair<int, int> prop_pair) const
{
    return (prop_pair.first + prop_pair.second) * (int)pow(2.0, k) + (prop_pair.first * prop_pair.second);
}