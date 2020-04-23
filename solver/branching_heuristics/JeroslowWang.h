#ifndef JEROSLOW_WANG_H
#define JEROSLOW_WANG_H

#include "Random.h"

class JeroslowWang : public BranchingHeuristic
{
public:
    enum class Version
    {
        ONE_SIDED,
        TWO_SIDED
    };

    JeroslowWang(Version version, int num_vars, LitMapUnique_t &lit_map_unique);

    Literal *NextLiteral(const ClauseSetUnique_t &clauses,
                                       const LitSetRaw_t &unset_lits,
                                       const LitSetRaw_t &set_lits) const;

private:
    std::unique_ptr<BranchingHeuristic> random_brancher;

    const Version version;
    const int num_vars;

    LitMapRaw_t raw_lit_map;

    float float_error = 1e-9;
};

#endif