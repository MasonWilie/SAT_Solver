#ifndef BRANCHING_HEURISTIC
#define BRANCHING_HEURISTIC

#include <memory.h>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <time.h>
#include <map>
#include <utility>
#include <math.h>

#include "Clause.h"

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
        BOHM,
        MOMS
    };

    BranchingHeuristic() = default;
    ~BranchingHeuristic() = default;
    virtual AtomicProposition *NextProposition(const ClauseSetUnique_t &clauses,
                                               const PropSetRaw_t &unset_props,
                                               const PropSetRaw_t &set_props) const = 0;
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
    AtomicProposition *NextProposition(const ClauseSetUnique_t &clauses,
                                       const PropSetRaw_t &unset_props,
                                       const PropSetRaw_t &set_props) const;
};

class MaxMinClause : public BranchingHeuristic
{
public:
    MaxMinClause(long long num_vars,
                 PropMapUnique_t &prop_map_unique,
                 const ClauseSetUnique_t &clauses);

    AtomicProposition *NextProposition(const ClauseSetUnique_t &clauses,
                                       const PropSetRaw_t &unset_props,
                                       const PropSetRaw_t &set_props) const;

private:
    std::unique_ptr<BranchingHeuristic> random_brancher;

    virtual int Score(std::pair<int, int> prop_pair) const = 0;

    const long long num_vars;
    PropMapRaw_t raw_prop_map;
};

class BohmsBranching : public MaxMinClause
{
public:
    BohmsBranching(long long num_vars,
                   PropMapUnique_t &prop_map_unique,
                   const ClauseSetUnique_t &clauses);

private:
    int Score(std::pair<int, int> prop_pair) const;

    const int alpha = 1;
    const int beta = 2;
};

class MomsBranching : public MaxMinClause
{
public:
    MomsBranching(long long num_vars,
                  PropMapUnique_t &prop_map_unique,
                  const ClauseSetUnique_t &clauses);

private:
    int Score(std::pair<int, int> prop_pair) const;

    const float k = 3.0;
};

#endif
