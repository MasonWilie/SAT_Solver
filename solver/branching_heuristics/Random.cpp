#include "Random.h"

RandomBranching::RandomBranching(PropMapUnique_t &prop_map_unique)
{
    srand(time(NULL));

    for (auto iter = std::begin(prop_map_unique); iter != std::end(prop_map_unique); std::advance(iter, 1))
    {
        raw_prop_map.insert(std::pair<int, AtomicProposition *>(iter->first, iter->second.get()));
    }
}

/**
 * @brief Branching algorithm that chooses a variable at random, excluding
 * variables which have their inverse already set.
 * 
 * Function is currently extreamly inefficient and needs refactoring
 * 
 * @param clauses All clauses (not used in this algorithm)
 * @param unset_props Propositions that are not currently set
 * @param set_props Propositions that are currently set
 * @return AtomicProposition* Next proposition to set
 */
AtomicProposition *RandomBranching::NextProposition(const ClauseSetUnique_t &clauses,
                                                    const PropSetRaw_t &unset_props,
                                                    const PropSetRaw_t &set_props) const
{
    for (auto iter = std::begin(clauses); iter != std::end(clauses); std::advance(iter, 1))
    {
        if ((*iter)->Size() == 1)
        {
            std::set<int> props = (*iter)->GetPropsAsInts();

            for (int p : props)
            {
                AtomicProposition *prop_ptr = raw_prop_map.at(p);
                if (prop_ptr->PresentInClause())
                {
                    return prop_ptr;
                }
            }
        }
    }

    PropSetRaw_t::iterator iter;
    do
    {
        iter = std::begin(unset_props);
        std::advance(iter, rand() % unset_props.size());
    } while ((*iter)->GetInverse()->IsAsserted());

    return *iter;
}