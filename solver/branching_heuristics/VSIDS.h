#ifndef VSIDS_H
#define VSIDS_H

#include "Random.h"

class VsidsBranching : public BranchingHeuristic
{
public:
    VsidsBranching(LitMapUnique_t &lit_map_unique,
                   const ClauseSetUnique_t &clauses);

    Literal *NextLiteral(const ClauseSetUnique_t &clauses,
                                       const LitSetRaw_t &unset_lits,
                                       const LitSetRaw_t &set_lits) const;

private:
    std::unique_ptr<BranchingHeuristic> random_brancher;
    LitMapRaw_t raw_lit_map;
};

#endif