#include <memory.h>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <time.h>
#include <map>
#include <utility>
#include <math.h>

#include "Clause.h"

#ifndef BRANCHING_HEURISTIC
#define BRANCHING_HEURISTIC

/**
 * @brief Virtual class that represents a branching heuristic,
 * which chooses the next variable to assert.
 * 
 */
class BranchingHeuristic
{
public:
    enum class BranchingType
    {
        RANDOM,
        BOHM
    };

    BranchingHeuristic()=default;
    ~BranchingHeuristic()=default;
    virtual AtomicProposition* NextProposition(const ClauseSetUnique_t &clauses, const PropSetRaw_t &unset_props, const PropSetRaw_t &set_props) const=0;
};


/**
 * @brief Random branching heuristic, which chooses a
 * random variable. Most simple and least efficient one.
 * 
 */
class RandomBranching : public BranchingHeuristic
{
public:
    RandomBranching();
    AtomicProposition* NextProposition(const ClauseSetUnique_t &clauses, const PropSetRaw_t &unset_props, const PropSetRaw_t &set_props) const;
};

class BohmsBranching : public BranchingHeuristic
{
public:
    BohmsBranching(long long num_vars, PropMapUnique_t &prop_map_unique, const ClauseSetUnique_t &clauses);
    AtomicProposition* NextProposition(const ClauseSetUnique_t &clauses, const PropSetRaw_t &unset_props, const PropSetRaw_t &set_props) const;

private:
    std::unique_ptr<BranchingHeuristic> random_brancher;
    
    const long long num_vars;
    PropMapRaw_t raw_prop_map;

    size_t max_clause_size;

    const int alpha = 1;
    const int beta = 2;
};


#endif
