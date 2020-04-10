#ifndef VSIDS_H
#define VSIDS_H

#include "Random.h"

class VsidsBranching : public BranchingHeuristic
{
public:
    VsidsBranching(PropMapUnique_t &prop_map_unique,
                   const ClauseSetUnique_t &clauses);

    AtomicProposition *NextProposition(const ClauseSetUnique_t &clauses,
                                       const PropSetRaw_t &unset_props,
                                       const PropSetRaw_t &set_props) const;

private:
    std::unique_ptr<BranchingHeuristic> random_brancher;

    std::map<AtomicProposition *, int> count;
    PropMapRaw_t raw_prop_map;
};

#endif