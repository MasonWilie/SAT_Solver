#include "gtest/gtest.h"
#include "Solver.h"

TEST(Solver, SpaceDelimit)
{
    std::string test_string_1 = "Hello this is the test string";
    std::vector<std::string> test_string_1_individual = {"Hello", "this", "is", "the", "test", "string"};

    std::string test_string_2 = "Extra space oops ";
    std::vector<std::string> test_string_2_individual = {"Extra", "space", "oops"};

    std::vector<std::string> test_1_results = Solver::SpaceDelimit(test_string_1);
    std::vector<std::string> test_2_results = Solver::SpaceDelimit(test_string_2);

    EXPECT_EQ(test_string_1_individual.size(), test_1_results.size());
    EXPECT_EQ(test_string_2_individual.size(), test_2_results.size());

    for (size_t i = 0; i < test_string_1_individual.size(); i++)
    {
        EXPECT_EQ(test_string_1_individual[i], test_1_results[i]);
    }

    for (size_t i = 0; i < test_string_2_individual.size(); i++)
    {
        EXPECT_EQ(test_string_2_individual[i], test_2_results[i]);
    }
}

TEST(Solver, ReadFile)
{
    Solver solver("test_data/simple_sat.cnf");

    std::set<long long> c1 = {-2, 4, 1};
    std::set<long long> c2 = {-3, 1, 2};
    std::set<long long> c3 = {4, -3, 2};

    std::set<std::set<long long>> clauses_long_long = solver.GetClausesAsLongs();

    EXPECT_TRUE(std::find(std::begin(clauses_long_long), std::end(clauses_long_long), c1) != std::end(clauses_long_long));
    EXPECT_TRUE(std::find(std::begin(clauses_long_long), std::end(clauses_long_long), c2) != std::end(clauses_long_long));
    EXPECT_TRUE(std::find(std::begin(clauses_long_long), std::end(clauses_long_long), c3) != std::end(clauses_long_long));
}

TEST(Solver, Solve)
{
    {
        Solver solver("test_data/simple_sat.cnf");
        Solver::Solution solution = solver.Solve(BranchingHeuristic::BranchingType::RANDOM, BacktrackingHeuristic::BacktrackingType::DPLL);
        EXPECT_TRUE(solution.SAT);
    }
    
    {
        Solver solver("test_data/simple_unsat.cnf");
        Solver::Solution solution = solver.Solve(BranchingHeuristic::BranchingType::RANDOM, BacktrackingHeuristic::BacktrackingType::DPLL);
        EXPECT_TRUE(!solution.SAT);
    }
}