#include "Random.h"

RandomBranching::RandomBranching(LitMapUnique_t &lit_map_unique)
{
    srand(time(NULL));

    for (auto iter = std::begin(lit_map_unique); iter != std::end(lit_map_unique); std::advance(iter, 1))
    {
        raw_lit_map.insert(std::pair<int, Literal *>(iter->first, iter->second.get()));
    }
}

/**
 * @brief Branching algorithm that chooses a variable at random, excluding
 * variables which have their inverse already set.
 * 
 * Function is currently extreamly inefficient and needs refactoring
 * 
 * @param clauses All clauses (not used in this algorithm)
 * @param unset_lits literals that are not currently set
 * @param set_lits literals that are currently set
 * @return Literal* Next literal to set
 */
Literal *RandomBranching::NextLiteral(const ClauseSetUnique_t &clauses,
                                                    const LitSetRaw_t &unset_lits,
                                                    const LitSetRaw_t &set_lits) const
{
    for (auto iter = std::begin(clauses); iter != std::end(clauses); std::advance(iter, 1))
    {
        if ((*iter)->Size() == 1)
        {
            std::set<int> lits = (*iter)->GetLitsAsInts();

            for (int p : lits)
            {
                Literal *lit_ptr = raw_lit_map.at(p);
                if (lit_ptr->PresentInClause())
                {
                    return lit_ptr;
                }
            }
        }
    }

    LitSetRaw_t::iterator iter;
    do
    {
        iter = std::begin(unset_lits);
        std::advance(iter, rand() % unset_lits.size());
    } while ((*iter)->GetInverse()->IsAsserted());

    return *iter;
}