#ifndef BACKTRACKING_HEURISTIC
#define BACKTRACKING_HEURISTIC

#include "BOHM.h"
#include "JeroslowWang.h"
#include "MOM.h"
#include "Random.h"
#include "VSIDS.h"


#include <bits/stdc++.h> 

/**
 * @brief Virtual class that represents a branching heuristic
 * 
 */
class BacktrackingHeuristic
{
public:
    
    enum class BacktrackingType
    {
        DPLL
    };

    BacktrackingHeuristic()=default;
    ~BacktrackingHeuristic()=default;

    virtual void Update(AtomicProposition* decision)=0;
    virtual AtomicProposition* Backtrack(ClauseSetUnique_t &clauses, PropSetRaw_t &unset_props, PropSetRaw_t &set_props)=0;
    virtual void Reset()=0;
};

/**
 * @brief Backtracking Heuristic that follows the standard
 * DPLL format, where you simply move back to the level of
 * the previous decision and to make the oposite decision if
 * it has not already been tried.
 * 
 */
class DpllBacktracking : public BacktrackingHeuristic
{
public:
    struct BacktrackingNode
    {
        AtomicProposition* regular{nullptr};
        AtomicProposition* notted{nullptr};
        AtomicProposition* last_set{nullptr};
    };

    void Update(AtomicProposition* decision);
    AtomicProposition* Backtrack(ClauseSetUnique_t &clauses, PropSetRaw_t &unset_props, PropSetRaw_t &set_props);
    void Reset();
private:
    std::stack<BacktrackingNode> history;
};

#endif