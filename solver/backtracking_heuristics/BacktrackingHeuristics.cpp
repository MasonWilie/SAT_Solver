#include "BacktrackingHeuristics.h"

/**
 * @brief Feeds the literal that was chosen to be asserted into the backtracking
 * algorithm so that the algorithm can keep track. If the chosen variable is the
 * inverse of the previously chosen variable (one that is still on the top of the
 * stack), then it will be added to the same node one the same level of the stack.
 * Otherwise it will create a new node and increase the size of the stack by 1.
 * 
 * @param decision literal that was previously chosen
 */
void DpllBacktracking::Update(Literal* decision)
{
    if (history.size() != 0 && history.top().regular == decision->GetInverse())
    {
        history.top().notted = decision;
    }else if (history.size() != 0 &&  history.top().notted == decision->GetInverse())
    {
        history.top().regular = decision;
    }else
    {
        BacktrackingNode node;
        if (decision->IsNot())
        {
            node.notted = decision;
            node.regular = nullptr;
        }else
        {
            node.regular = decision;
            node.notted = nullptr;
        }
        history.push(node);
    }

    history.top().last_set = decision;
}


/**
 * @brief Standard backtracking algorithm, which chooses the next variable
 * in the basic way that DPLL does. Just move one decision backwards and see
 * if you can proceed from there.
 * 
 * @param clauses All clauses (not used in this algorithm)
 * @param unset_lits literals that are not currently set (asserted)
 * @param set_lits literals that are currently set (asserted)
 * @return Literal* Suggestion for the next literal to set. If nullptr, then UNSAT.
 */
Literal* DpllBacktracking::Backtrack(ClauseSetUnique_t &clauses, LitSetRaw_t &unset_lits, LitSetRaw_t &set_lits)
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
            unset_lits.insert(history.top().last_set);
            set_lits.erase(history.top().last_set);
            history.pop();
        }else if (history.top().regular && !history.top().notted) // Regular set, notted not
        {
            history.top().regular->UnAssert();
            unset_lits.insert(history.top().regular);
            set_lits.erase(history.top().regular);
            return history.top().regular->GetInverse();
        }else // Notted set, regular not
        {
            history.top().notted->UnAssert();
            unset_lits.insert(history.top().notted);
            set_lits.erase(history.top().notted);
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

