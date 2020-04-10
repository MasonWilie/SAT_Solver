#ifndef MAX_MIN_CLAUSE_H
#define MAX_MIN_CLAUSE_H

#include "Random.h"

class MaxMinClauseHeuristic : public BranchingHeuristic
{
public:
    MaxMinClauseHeuristic(long long num_vars,
                          PropMapUnique_t &prop_map_unique);

    AtomicProposition *NextProposition(const ClauseSetUnique_t &clauses,
                                       const PropSetRaw_t &unset_props,
                                       const PropSetRaw_t &set_props) const;

private:
    virtual int Score(std::pair<int, int> prop_pair) const = 0;

    const long long num_vars;
    std::unique_ptr<BranchingHeuristic> random_brancher;
    PropMapRaw_t raw_prop_map;
};

#endif