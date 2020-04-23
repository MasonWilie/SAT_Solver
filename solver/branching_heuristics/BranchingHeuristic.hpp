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

size_t GetMaxClauseSize(const ClauseSetUnique_t &clauses);

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
        MOMS,
        JW1, // Two sided Jeroslow-Wang
        JW2, // One sided Jeroslow-Wang
        VSIDS
    };

    BranchingHeuristic() = default;
    ~BranchingHeuristic() = default;
    virtual Literal *NextLiteral(const ClauseSetUnique_t &clauses,
                                               const LitSetRaw_t &unset_lits,
                                               const LitSetRaw_t &set_lits) const = 0;
    
    static size_t GetMaxClauseSize(const ClauseSetUnique_t &clauses)
    {
        size_t max_clause_size = 0;
        for (auto clause_iter = std::begin(clauses); clause_iter != std::end(clauses); std::advance(clause_iter, 1))
        {
            size_t size = (*clause_iter)->Size();
            if (size > max_clause_size)
            {
                max_clause_size = size;
            }
        }
        return max_clause_size;
    }
};
#endif
