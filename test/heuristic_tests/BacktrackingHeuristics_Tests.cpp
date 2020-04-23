#include "gtest/gtest.h"
#include "BacktrackingHeuristics.h"

TEST(Backtracking, Standard)
{
    ClauseSetUnique_t clauses;
    LitMapUnique_t lit_map;
    LitSetRaw_t unset_lits;
    LitSetRaw_t set_lits;

    Literal* lit;

    int var_names[] = {1, 2, 3};
    size_t num_vars{3};

    for (size_t i = 0; i < num_vars; i++)
    {
        Literal::LitPair pair = Literal::CreateLitPair(var_names[i]);
        lit_map[var_names[i]] = std::unique_ptr<Literal>(pair.regular);
        lit_map[var_names[i]*-1] = std::unique_ptr<Literal>(pair.notted);

        unset_lits.insert(pair.regular);
        unset_lits.insert(pair.notted);
    }

    DpllBacktracking backtracker;

    set_lits.insert(lit_map[var_names[0]].get());
    unset_lits.erase(lit_map[var_names[0]].get());

    EXPECT_EQ(set_lits.size(), 1);
    EXPECT_EQ(unset_lits.size(), lit_map.size() - 1);

    
    lit = lit_map[var_names[0]].get();
    backtracker.Update(lit);

    Literal* suggested_lit = backtracker.Backtrack(clauses, unset_lits, set_lits);

    EXPECT_EQ(suggested_lit, lit_map[var_names[0]*-1].get());
    EXPECT_EQ(unset_lits.size(), lit_map.size());
    EXPECT_EQ(set_lits.size(), 0);

    lit = suggested_lit;

    set_lits.insert(suggested_lit);
    unset_lits.erase(suggested_lit);
    backtracker.Update(lit);

    suggested_lit = backtracker.Backtrack(clauses, unset_lits, set_lits);

    EXPECT_EQ(suggested_lit, nullptr);

    backtracker.Reset();

    for (size_t i{0}; i < num_vars; i++)
    {
        lit = lit_map[var_names[i]].get();
        backtracker.Update(lit);
    }

    for (size_t i{0}; i < num_vars; i++)
    {
        suggested_lit = backtracker.Backtrack(clauses, unset_lits, set_lits);
        EXPECT_EQ(suggested_lit, lit_map[var_names[num_vars - i - 1]*-1].get());

        lit = suggested_lit;
        backtracker.Update(lit); 
    }

    suggested_lit = backtracker.Backtrack(clauses, unset_lits, set_lits);
    EXPECT_EQ(suggested_lit, nullptr);

    backtracker.Reset();

}