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
    RandomBranching(PropMapUnique_t &prop_map_unique);
    ~RandomBranching()=default;
    AtomicProposition *NextProposition(const ClauseSetUnique_t &clauses,
                                       const PropSetRaw_t &unset_props,
                                       const PropSetRaw_t &set_props) const;

private:
    PropMapRaw_t raw_prop_map;
};

#endif