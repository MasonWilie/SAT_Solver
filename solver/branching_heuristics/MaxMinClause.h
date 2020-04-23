#ifndef MAX_MIN_CLAUSE_H
#define MAX_MIN_CLAUSE_H

#include "Random.h"

class MaxMinClauseHeuristic : public BranchingHeuristic
{
public:
    MaxMinClauseHeuristic(int num_vars,
                          LitMapUnique_t &lit_map_unique);

    Literal *NextLiteral(const ClauseSetUnique_t &clauses,
                                       const LitSetRaw_t &unset_lits,
                                       const LitSetRaw_t &set_lits) const;

private:
    virtual int Score(std::pair<int, int> lit_pair) const = 0;

    const int num_vars;
    std::unique_ptr<BranchingHeuristic> random_brancher;
    LitMapRaw_t raw_lit_map;
};

#endif