#include "MOM.h"

MomsBranching::MomsBranching(int num_vars,
                             LitMapUnique_t &lit_map_unique)
    : MaxMinClauseHeuristic(num_vars, lit_map_unique)
{
}

int MomsBranching::Score(std::pair<int, int> lit_pair) const
{
    return (lit_pair.first + lit_pair.second) * (int)pow(2.0, k) + (lit_pair.first * lit_pair.second);
}