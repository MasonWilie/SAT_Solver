#include "BranchingHeuristics.h"
#include "Clause.h"

/**
 * @brief Return the maximum value between two ints
 * 
 * @param a 
 * @param b 
 * @return int Maximum of a and b
 */
int max(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

/**
 * @brief Return the minimum value between two ints
 * 
 * @param a 
 * @param b 
 * @return int Minimum of a and b
 */
int min(int a, int b)
{
    if (a < b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

RandomBranching::RandomBranching()
{
    srand(time(NULL));
}

/**
 * @brief Branching algorithm that chooses a variable at random, excluding
 * variables which have their inverse already set.
 * 
 * Function is currently extreamly inefficient and needs refactoring
 * 
 * @param clauses All clauses (not used in this algorithm)
 * @param unset_props Propositions that are not currently set
 * @param set_props Propositions that are currently set
 * @return AtomicProposition* Next proposition to set
 */
AtomicProposition *RandomBranching::NextProposition(const ClauseSetUnique_t &clauses,
                                                    const PropSetRaw_t &unset_props,
                                                    const PropSetRaw_t &set_props) const
{
    // TODO: Make this more efficient
    PropSetRaw_t::iterator iter;
    do
    {
        iter = std::begin(unset_props);
        std::advance(iter, rand() % unset_props.size());
    } while (std::find(std::begin(set_props), std::end(set_props), (*iter)->GetInverse()) != std::end(set_props));

    return *iter;
}

MaxMinClause::MaxMinClause(long long num_vars,
                           PropMapUnique_t &prop_map_unique,
                           const ClauseSetUnique_t &clauses)
    : num_vars(num_vars),
      random_brancher(new RandomBranching)
{
    // Create a map from proposition names to their pointers
    for (auto iter = std::begin(prop_map_unique); iter != std::end(prop_map_unique); std::advance(iter, 1))
    {
        raw_prop_map.insert(std::pair<long long, AtomicProposition *>(iter->first, iter->second.get()));
    }
}

AtomicProposition *MaxMinClause::NextProposition(const ClauseSetUnique_t &clauses,
                                                 const PropSetRaw_t &unset_props,
                                                 const PropSetRaw_t &set_props) const
{
    // Get the current maximum clause size (could change when adding clauses)
    size_t max_clause_size = 0;
    for (auto clause_iter = std::begin(clauses); clause_iter != std::end(clauses); std::advance(clause_iter, 1))
    {
        size_t size = (*clause_iter)->Size();
        if (size > max_clause_size)
        {
            max_clause_size = size;
        }
    }

    // Initially, you consider all props
    std::set<long long> props_to_consider;
    for (int i = 0; i < num_vars + 1; i++)
    {
        props_to_consider.insert(i);
        props_to_consider.insert(-i);
    }

    std::set<long long> ties;
    // Start with clauses of size 1, and go all the way up to the maximum clause size if needed
    for (size_t clause_size = 1; clause_size < max_clause_size + 1; clause_size++)
    {
        std::map<long long, std::pair<int, int>> prop_map;
        // Iterate through all clauses
        for (auto clause_iter = std::begin(clauses); clause_iter != std::end(clauses); std::advance(clause_iter, 1))
        {

            // Check to see if this clause is present, and of the desired size
            if ((*clause_iter)->Satisfied() || (*clause_iter)->Size() != clause_size)
            {
                continue;
            }

            std::set<long long> props = (*clause_iter)->GetPropositionsLongLong();

            // Iterate through each proposition in the clause
            for (long long prop_num : props)
            {
                // If the proposition is not being considered or is not present in the clause, continue
                if (props_to_consider.find(prop_num) == std::end(props_to_consider) || !raw_prop_map.at(prop_num)->PresentInClause())
                {
                    continue;
                }

                // Convert notted props to regular, but remember it was notted
                bool neg = prop_num < 0;
                if (neg)
                {
                    prop_num = (-1) * prop_num;
                }

                // Create an entry in the map for the prop if none exists
                if (prop_map.find(prop_num) == std::end(prop_map))
                {
                    prop_map.insert(std::pair<long long, std::pair<int, int>>(prop_num, std::pair<int, int>(0, 0)));
                }

                // Add add one to the corresponding prop (first = not notted, second = notted)
                if (neg)
                {
                    prop_map.at(prop_num).second++;
                }
                else
                {
                    prop_map.at(prop_num).first++;
                }
            }
        }

        // If there were no propositions that fit the requirements, move on
        if (prop_map.empty())
        {
            continue;
        }

        long long best_score_prop;
        int best_score = -1;
        bool tie = false;

        // Try to find the maximum score for each proposition
        for (auto map_iter = std::begin(prop_map); map_iter != std::end(prop_map); std::advance(map_iter, 1))
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
                    best_score_prop = map_iter->first;
                    tie = false;
                }
            }
        }

        // If there was a maximum score, return the prop.
        if (!tie)
        {
            return raw_prop_map.at(best_score_prop);
        }

        // If there was no max score, narrow the search space and try again
        props_to_consider.clear();
        for (long long p : ties)
        {
            props_to_consider.insert(p);
        }
    }

    // If there were ties up until the last clause number, choose the first one. Otherwise choose random.
    if (!ties.empty())
    {
        return raw_prop_map.at(*std::begin(ties));
    }
    else
    {
        return random_brancher->NextProposition(clauses, unset_props, set_props);
    }
}

BohmsBranching::BohmsBranching(long long num_vars,
                               PropMapUnique_t &prop_map_unique,
                               const ClauseSetUnique_t &clauses)
    : MaxMinClause(num_vars, prop_map_unique, clauses)
{
}

int BohmsBranching::Score(std::pair<int, int> prop_pair) const
{
    return alpha * max(prop_pair.first, prop_pair.second) + beta * min(prop_pair.first, prop_pair.second);
}

MomsBranching::MomsBranching(long long num_vars,
                             PropMapUnique_t &prop_map_unique,
                             const ClauseSetUnique_t &clauses)
    : MaxMinClause(num_vars, prop_map_unique, clauses)
{
}

int MomsBranching::Score(std::pair<int, int> prop_pair) const
{
    return (prop_pair.first + prop_pair.second) * (int)pow(2.0, k) + (prop_pair.first * prop_pair.second);
}