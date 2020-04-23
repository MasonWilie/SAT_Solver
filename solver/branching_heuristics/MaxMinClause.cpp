#include "MaxMinClause.h"


MaxMinClauseHeuristic::MaxMinClauseHeuristic(int num_vars,
                                             LitMapUnique_t &lit_map_unique)
    : num_vars(num_vars),
      random_brancher(new RandomBranching(lit_map_unique))
{
    // Create a map from literal names to their pointers
    for (auto iter = std::begin(lit_map_unique); iter != std::end(lit_map_unique); std::advance(iter, 1))
    {
        raw_lit_map.insert(std::pair<int, Literal *>(iter->first, iter->second.get()));
    }
}

Literal *MaxMinClauseHeuristic::NextLiteral(const ClauseSetUnique_t &clauses,
                                                          const LitSetRaw_t &unset_lits,
                                                          const LitSetRaw_t &set_lits) const
{
    // Get the current maximum clause size (could change when adding clauses)
    size_t max_clause_size = BranchingHeuristic::GetMaxClauseSize(clauses);

    std::map<int, std::set<int>> unit_clauses_found;

    // Initially, you consider all lits
    std::set<int> lits_to_consider;
    for (int i = 0; i < num_vars + 1; i++)
    {
        lits_to_consider.insert(i);
        lits_to_consider.insert(-i);
    }

    std::set<int> ties;
    // Start with clauses of size 1, and go all the way up to the maximum clause size if needed
    for (size_t clause_size = 1; clause_size < max_clause_size + 1; clause_size++)
    {
        std::map<int, std::pair<int, int>> lit_map;
        // Iterate through all clauses
        for (auto clause_iter = std::begin(clauses); clause_iter != std::end(clauses); std::advance(clause_iter, 1))
        {

            // Check to see if this clause is present, and of the desired size
            if ((*clause_iter)->Satisfied() || (*clause_iter)->Size() != clause_size)
            {
                continue;
            }

            std::set<int> lits = (*clause_iter)->GetLitsAsInts();

            // Iterate through each literal in the clause
            for (int lit_num : lits)
            {
                // If the literal is not being considered or is not present in the clause, continue
                if (lits_to_consider.find(lit_num) == std::end(lits_to_consider) || !raw_lit_map.at(lit_num)->PresentInClause())
                {
                    continue;
                }

                // Convert notted lits to regular, but remember it was notted
                bool neg = lit_num < 0;
                if (neg)
                {
                    lit_num = (-1) * lit_num;
                }

                // Create an entry in the map for the lit if none exists
                if (lit_map.find(lit_num) == std::end(lit_map))
                {
                    lit_map.insert(std::pair<int, std::pair<int, int>>(lit_num, std::pair<int, int>(0, 0)));
                }

                // Add add one to the corresponding lit (first = not notted, second = notted)
                if (neg)
                {
                    lit_map.at(lit_num).second++;
                }
                else
                {
                    lit_map.at(lit_num).first++;
                }
            }
        }

        // If there were no literals that fit the requirements, move on
        if (lit_map.empty())
        {
            continue;
        }

        int best_score_lit = 0;
        int best_score = -1;
        std::pair<int, int> best_score_count = std::make_pair(0, 0);
        bool tie = false;

        // Try to find the maximum score for each literal
        for (auto map_iter = std::begin(lit_map); map_iter != std::end(lit_map); std::advance(map_iter, 1))
        {

            int score = Score(map_iter->second);
            if (score >= best_score)
            {

                if (score == best_score)
                {
                    ties.insert(map_iter->first);
                    tie = true;
                }
                else
                {
                    ties.clear();
                    ties.insert(map_iter->first);
                    best_score = score;
                    best_score_count = map_iter->second;
                    best_score_lit = map_iter->first;
                    tie = false;
                }
            }
        }

        // If there was a maximum score, return the lit.
        if (!tie)
        {
            return raw_lit_map.at((best_score_count.first >= best_score_count.second ? best_score_lit : -best_score_lit));
        }

        // If there was no max score, narrow the search space and try again
        lits_to_consider.clear();
        for (int p : ties)
        {
            lits_to_consider.insert(p);
        }
    }

    // If there were ties up until the last clause number, choose the first one. Otherwise choose random.
    if (!ties.empty())
    {
        return raw_lit_map.at(*std::begin(ties));
    }

    return random_brancher->NextLiteral(clauses, unset_lits, set_lits);
}
