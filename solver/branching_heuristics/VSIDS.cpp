#include "VSIDS.h"


VsidsBranching::VsidsBranching(PropMapUnique_t &prop_map_unique,
                               const ClauseSetUnique_t &clauses)
    : random_brancher(new RandomBranching(prop_map_unique))
{
    srand(time(NULL));

    for (auto iter = std::begin(prop_map_unique); iter != std::end(prop_map_unique); std::advance(iter, 1))
    {
        count.insert(std::make_pair(iter->second.get(), 0));
        raw_prop_map.insert(std::make_pair(iter->first, iter->second.get()));
    }

    for (auto iter = std::begin(clauses); iter != std::end(clauses); std::advance(iter, 1))
    {
        std::set<long long> props_long = (*iter)->GetPropositionsLongLong();
        for (long long p : props_long)
        {
            count.at(raw_prop_map.at(p))++;
        }
    }
}

AtomicProposition *VsidsBranching::NextProposition(const ClauseSetUnique_t &clauses,
                                                   const PropSetRaw_t &unset_props,
                                                   const PropSetRaw_t &set_props) const
{
    std::set<AtomicProposition *> ties;

    AtomicProposition* winner = nullptr;
    int max_count = -1;
    for (auto prop_and_count : count)
    {
        if (!prop_and_count.first->PresentInClause())
        {
            continue;
        }
        if (prop_and_count.second > max_count)
        {
            max_count = prop_and_count.second;
            winner = prop_and_count.first;

            ties.clear();
            ties.insert(prop_and_count.first);
        }
        else if (prop_and_count.second == max_count)
        {
            ties.insert(prop_and_count.first);
        }
    }

    if (ties.size() == 1)
    {
        return winner;
    }

    auto iter = std::begin(ties);
    std::advance(iter, rand() % ties.size());
    return *iter;
}