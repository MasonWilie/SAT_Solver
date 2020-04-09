#include "gtest/gtest.h"
#include "BacktrackingHeuristics.h"
#include "BranchingHeuristics.h"

TEST(Backtracking, Standard)
{
    ClauseSetUnique_t clauses;
    PropMapUnique_t prop_map;
    PropSetRaw_t unset_props;
    PropSetRaw_t set_props;

    PropDecision decision;

    long long var_names[] = {1, 2, 3};
    size_t num_vars{3};

    for (size_t i = 0; i < num_vars; i++)
    {
        AtomicProposition::PropPair pair = AtomicProposition::CreatePropPair(var_names[i]);
        prop_map[var_names[i]] = std::unique_ptr<AtomicProposition>(pair.regular);
        prop_map[var_names[i]*-1] = std::unique_ptr<AtomicProposition>(pair.notted);

        unset_props.insert(pair.regular);
        unset_props.insert(pair.notted);
    }

    DpllBacktracking backtracker;

    set_props.insert(prop_map[var_names[0]].get());
    unset_props.erase(prop_map[var_names[0]].get());

    EXPECT_EQ(set_props.size(), 1);
    EXPECT_EQ(unset_props.size(), prop_map.size() - 1);

    
    decision.prop = prop_map[var_names[0]].get();
    backtracker.Update(decision);

    AtomicProposition* suggested_prop = backtracker.Backtrack(clauses, unset_props, set_props);

    EXPECT_EQ(suggested_prop, prop_map[var_names[0]*-1].get());
    EXPECT_EQ(unset_props.size(), prop_map.size());
    EXPECT_EQ(set_props.size(), 0);

    decision.prop = suggested_prop;

    set_props.insert(suggested_prop);
    unset_props.erase(suggested_prop);
    backtracker.Update(decision);

    suggested_prop = backtracker.Backtrack(clauses, unset_props, set_props);

    EXPECT_EQ(suggested_prop, nullptr);

    backtracker.Reset();

    for (size_t i{0}; i < num_vars; i++)
    {
        decision.prop = prop_map[var_names[i]].get();
        backtracker.Update(decision);
    }

    for (size_t i{0}; i < num_vars; i++)
    {
        suggested_prop = backtracker.Backtrack(clauses, unset_props, set_props);
        EXPECT_EQ(suggested_prop, prop_map[var_names[num_vars - i - 1]*-1].get());

        decision.prop = suggested_prop;
        backtracker.Update(decision); 
    }

    suggested_prop = backtracker.Backtrack(clauses, unset_props, set_props);
    EXPECT_EQ(suggested_prop, nullptr);

    backtracker.Reset();

}