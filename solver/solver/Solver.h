#ifndef SOLVER_H
#define SOLVER_H

#include "Clause.h"
#include "BacktrackingHeuristics.h"
#include <fstream>
#include <cmath>


struct Problem
{
    std::set<std::set<long long>> clauses;
};

class Solver
{
public:

    struct Solution
    {
        bool SAT;
        std::set<long long> assertions;

        void PrintAssertions()
        {
            for (auto iter = std::begin(assertions); iter != std::end(assertions); std::advance(iter, 1))
            {
                std::cout << (*iter) << " ";
            }
            std::cout << std::endl;
        }

        void WriteToFile(const std::string filename) const
        {
            std::ofstream out(filename);

            if (!out.is_open())
            {
                std::cout << "Error opening file '" << filename <<"'" << std::endl;
                return;
            }

            for (auto iter = std::begin(assertions); iter != std::end(assertions); std::advance(iter, 1))
            {
                out << *iter << " ";
            }
        }
    };

    enum class FormulaStatus
    {
        SAT,
        UNSTAT,
        CONTINUE
    };

    Solver(std::string filename);

    Solution Solve();
    static std::vector<std::string> SpaceDelimit(std::string s);
    void ReadFile(std::string filename);

    Solution Solve(const BranchingHeuristic::BranchingType branching_type,
                   const BacktrackingHeuristic::BacktrackingType backtracking_type);

    std::set<std::set<long long>> GetClausesAsLongs() const;

private:
    bool Step();
    FormulaStatus Status() const;

    std::unique_ptr<BranchingHeuristic> CreateBranchingHeuristic(BranchingHeuristic::BranchingType type);
    std::unique_ptr<BacktrackingHeuristic> CreateBacktrackingHeuristic(BacktrackingHeuristic::BacktrackingType type);

    long long num_vars;

    ClauseSetUnique_t clauses;
    PropMapUnique_t prop_map;
    PropSetRaw_t set_props;
    PropSetRaw_t unset_props;
};

#endif