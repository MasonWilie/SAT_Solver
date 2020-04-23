#include "gtest/gtest.h"
#include "Random.h"
#include <map>

TEST(RandomHeuristic, Randomness)
{
    
    ClauseSetUnique_t clauses;
    LitMapUnique_t lits_master;
    LitSetRaw_t lits, set_lits;

    for (int i = 1; i < 5; i++)
    {
        Literal::LitPair pair = Literal::CreateLitPair(i);
        lits_master.insert(std::make_pair(i, std::unique_ptr<Literal>(pair.regular)));
        lits_master.insert(std::make_pair(-i, std::unique_ptr<Literal>(pair.notted)));
    }

    RandomBranching rb(lits_master);

    for (auto iter = std::begin(lits_master); iter != std::end(lits_master); iter++)
    {
        lits.insert(iter->second.get());
    }

    LitSetRaw_t chosen_lits;

    for (int i = 0; i < 10000; i++)
    {
        chosen_lits.insert(rb.NextLiteral(clauses, lits, set_lits));
    }

    EXPECT_EQ(chosen_lits.size(), lits.size());
}