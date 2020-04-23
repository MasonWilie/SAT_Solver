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

    virtual void Update(Literal* decision)=0;
    virtual Literal* Backtrack(ClauseSetUnique_t &clauses, LitSetRaw_t &unset_lits, LitSetRaw_t &set_lits)=0;
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

    DpllBacktracking()=default;
    ~DpllBacktracking()=default;

    struct BacktrackingNode
    {
        Literal* regular{nullptr};
        Literal* notted{nullptr};
        Literal* last_set{nullptr};
    };

    void Update(Literal* decision);
    Literal* Backtrack(ClauseSetUnique_t &clauses, LitSetRaw_t &unset_lits, LitSetRaw_t &set_lits);
    void Reset();
private:
    std::stack<BacktrackingNode> history;
};

#endif