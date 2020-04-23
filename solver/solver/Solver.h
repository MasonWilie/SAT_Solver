#ifndef SOLVER_H
#define SOLVER_H

#include "Clause.h"
#include "BacktrackingHeuristics.h"

#include <fstream>
#include <cmath>


struct Problem
{
    std::set<std::set<int>> clauses;
};

class Solver
{
public:

    struct Solution
    {
        bool SAT{false};
        std::set<int> assertions;
        uint64_t iterations{0};

        void PrintAssertions()
        {
            for (auto iter = std::begin(assertions); iter != std::end(assertions); std::advance(iter, 1))
            {
                std::cout << (*iter) << " ";
            }
            std::cout << std::endl;
        }

        void WriteToFile(const std::string filename, double time) const
        {
            std::ofstream out(filename);

            if (!out.is_open())
            {
                std::cout << "Error opening file '" << filename <<"'" << std::endl;
                return;
            }

            if (SAT)
            {
                for (auto iter = std::begin(assertions); iter != std::end(assertions); std::advance(iter, 1))
                {
                    out << *iter << " ";
                }
                out << "\n";
            }
            out << time << "\n";
            out << iterations;
        }
    };

    enum class FormulaStatus
    {
        SAT,
        UNSTAT,
        CONTINUE
    };

    Solver(std::string filename);
    ~Solver()=default;

    Solution Solve();
    static std::vector<std::string> SpaceDelimit(std::string s);
    void ReadFile(std::string filename);

    Solution Solve(const BranchingHeuristic::BranchingType branching_type,
                   const BacktrackingHeuristic::BacktrackingType backtracking_type);

    std::set<std::set<int>> GetClausesAsInts() const;

private:
    bool Step();
    FormulaStatus Status() const;

    std::unique_ptr<BranchingHeuristic> CreateBranchingHeuristic(BranchingHeuristic::BranchingType type);
    std::unique_ptr<BacktrackingHeuristic> CreateBacktrackingHeuristic(BacktrackingHeuristic::BacktrackingType type);

    int num_vars;

    ClauseSetUnique_t clauses;
    LitMapUnique_t prop_map;
    LitSetRaw_t set_props;
    LitSetRaw_t unset_props;
};

#endif