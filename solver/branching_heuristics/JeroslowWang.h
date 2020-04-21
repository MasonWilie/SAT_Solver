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

    JeroslowWang(Version version, int num_vars, PropMapUnique_t &prop_map_unique);

    AtomicProposition *NextProposition(const ClauseSetUnique_t &clauses,
                                       const PropSetRaw_t &unset_props,
                                       const PropSetRaw_t &set_props) const;

private:
    std::unique_ptr<BranchingHeuristic> random_brancher;

    const Version version;
    const int num_vars;

    PropMapRaw_t raw_prop_map;

    float float_error = 1e-9;
};

#endif