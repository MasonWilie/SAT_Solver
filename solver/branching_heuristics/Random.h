#ifndef RANDOM_H
#define RANDOM_H

#include "BranchingHeuristic.hpp"

/**
 * @brief Random branching heuristic, which chooses a
 * random variable. Most simple and least efficient one.
 * 
 */
class RandomBranching : public BranchingHeuristic
{
public:
    RandomBranching(LitMapUnique_t &lit_map_unique);
    ~RandomBranching()=default;
    Literal *NextLiteral(const ClauseSetUnique_t &clauses,
                                       const LitSetRaw_t &unset_lits,
                                       const LitSetRaw_t &set_lits) const;

private:
    LitMapRaw_t raw_lit_map;
};

#endif