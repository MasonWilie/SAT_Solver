#include "VSIDS.h"


VsidsBranching::VsidsBranching(LitMapUnique_t &lit_map_unique,
                               const ClauseSetUnique_t &clauses)
    : random_brancher(new RandomBranching(lit_map_unique))
{
    srand(time(NULL));

    for (auto iter = std::begin(lit_map_unique); iter != std::end(lit_map_unique); std::advance(iter, 1))
    {
        raw_lit_map.insert(std::make_pair(iter->first, iter->second.get()));
    }
}

Literal *VsidsBranching::NextLiteral(const ClauseSetUnique_t &clauses,
                                                   const LitSetRaw_t &unset_lits,
                                                   const LitSetRaw_t &set_lits) const
{
    std::map<Literal *, int> count;
    for (auto iter = std::begin(raw_lit_map); iter != std::end(raw_lit_map); std::advance(iter, 1))
    {
        count.insert(std::make_pair(iter->second, 0));
    }

    for (auto clause_iter = std::begin(clauses); clause_iter != std::end(clauses); std::advance(clause_iter, 1))
    {
        if ((*clause_iter)->Satisfied())
        {
            continue;
        }

        LitSetRaw_t lits = (*clause_iter)->GetLitsAsPtrs();

        for (Literal* p : lits)
        {
            if (p->IsAsserted() || p->GetInverse()->IsAsserted())
            {
                continue;
            }else
            {
                count[p]++;
            }
        }
    }


    std::set<Literal *> ties;
    Literal* winner = nullptr;
    int max_count = -1;
    for (auto lit_and_count : count)
    {
        if (lit_and_count.first->IsAsserted() || lit_and_count.first->GetInverse()->IsAsserted())
        {
            continue;
        }
        if (lit_and_count.second > max_count)
        {
            max_count = lit_and_count.second;
            winner = lit_and_count.first;

            ties.clear();
            ties.insert(lit_and_count.first);
        }
        else if (lit_and_count.second == max_count)
        {
            ties.insert(lit_and_count.first);
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