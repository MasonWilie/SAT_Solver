#include "gtest/gtest.h"
#include "BranchingHeuristics.h"
#include <map>

TEST(RandomHeuristic, Randomness)
{
    
    ClauseSetUnique_t clauses;
    PropMapUnique_t props_master;
    PropSetRaw_t props, set_props;

    for (long long i = 1; i < 5; i++)
    {
        AtomicProposition::PropPair pair = AtomicProposition::CreatePropPair(i);
        props_master.insert(std::make_pair(i, std::unique_ptr<AtomicProposition>(pair.regular)));
        props_master.insert(std::make_pair(-i, std::unique_ptr<AtomicProposition>(pair.notted)));
    }

    RandomBranching rb(props_master);

    for (auto iter = std::begin(props_master); iter != std::end(props_master); iter++)
    {
        props.insert(iter->second.get());
    }

    PropSetRaw_t chosen_props;

    for (int i = 0; i < 10000; i++)
    {
        chosen_props.insert(rb.NextProposition(clauses, props, set_props));
    }

    EXPECT_EQ(chosen_props.size(), props.size());
}