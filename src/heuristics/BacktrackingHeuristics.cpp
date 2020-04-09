#include "BacktrackingHeuristics.h"

/**
 * @brief Feeds the proposition that was chosen to be asserted into the backtracking
 * algorithm so that the algorithm can keep track. If the chosen variable is the
 * inverse of the previously chosen variable (one that is still on the top of the
 * stack), then it will be added to the same node one the same level of the stack.
 * Otherwise it will create a new node and increase the size of the stack by 1.
 * 
 * @param decision Proposition that was previously chosen
 */
void DpllBacktracking::Update(PropDecision decision)
{
    if (history.size() != 0 && history.top().regular == decision.prop->GetInverse())
    {
        history.top().notted = decision.prop;
    }else if (history.size() != 0 &&  history.top().notted == decision.prop->GetInverse())
    {
        history.top().regular = decision.prop;
    }else
    {
        BacktrackingNode node;
        if (decision.prop->IsNot())
        {
            node.notted = decision.prop;
            node.regular = nullptr;
        }else
        {
            node.regular = decision.prop;
            node.notted = nullptr;
        }
        history.push(node);
    }

    history.top().last_set = decision.prop;
}

/**
 * @brief Standard backtracking algorithm, which chooses the next variable
 * in the basic way that DPLL does. Just move one decision backwards and see
 * if you can proceed from there.
 * 
 * @param clauses All clauses (not used in this algorithm)
 * @param unset_props Propositions that are not currently set (asserted)
 * @param set_props Propositions that are currently set (asserted)
 * @return AtomicProposition* Suggestion for the next proposition to set. If nullptr, then UNSAT.
 */
AtomicProposition* DpllBacktracking::Backtrack(ClauseSetUnique_t &clauses, PropSetRaw_t &unset_props, PropSetRaw_t &set_props)
{
    if (history.size() == 0)
    {
        std::cout << "[ERROR  ] Attempting to pop the history with no elements in it\n";
    }

    while(true)
    {
        // No more paths to go -> Unsat
        if (history.size() == 0)
        {
            return nullptr;
        }

        // If both paths have been tried, go back up one step
        if (history.top().regular && history.top().notted)
        {
            history.top().last_set->UnAssert();
            unset_props.insert(history.top().last_set);
            set_props.erase(history.top().last_set);
            history.pop();
        }else if (history.top().regular && !history.top().notted) // Regular set, notted not
        {
            history.top().regular->UnAssert();
            unset_props.insert(history.top().regular);
            set_props.erase(history.top().regular);
            return history.top().regular->GetInverse();
        }else // Notted set, regular not
        {
            history.top().notted->UnAssert();
            unset_props.insert(history.top().notted);
            set_props.erase(history.top().notted);
            return history.top().notted->GetInverse();
        }
    }

    return nullptr;
}

/**
 * @brief Resets the algorithm so that it forgets its history.
 * 
 */
void DpllBacktracking::Reset()
{
    while (history.size() != 0)
    {
        history.pop();
    }
}



/**
 * @brief Feeds the proposition that was chosen to be asserted into the backtracking
 * algorithm so that the algorithm can keep track. If the chosen variable is the
 * inverse of the previously chosen variable (one that is still on the top of the
 * stack), then it will be added to the same node one the same level of the stack.
 * Otherwise it will create a new node and increase the size of the stack by 1.
 * 
 * @param decision Proposition that was previously chosen
 */
void CdclBacktracking::Update(PropDecision decision)
{
    graph.AddNode(decision.prop);
    if (decision.was_unit_clause)
    {  
        for (auto p : decision.clause_buddies)
        {
            graph.AddEdge(p, decision.prop);
        }
    }

    last_set = decision.prop;
}


AtomicProposition* CdclBacktracking::Backtrack(ClauseSetUnique_t &clauses, PropSetRaw_t &unset_props, PropSetRaw_t &set_props)
{

}


void CdclBacktracking::Reset()
{

}


