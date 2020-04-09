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

size_t GetMaxClauseSize(const ClauseSetUnique_t &clauses)
{
    size_t max_clause_size = 0;
    for (auto clause_iter = std::begin(clauses); clause_iter != std::end(clauses); std::advance(clause_iter, 1))
    {
        size_t size = (*clause_iter)->Size();
        if (size > max_clause_size)
        {
            max_clause_size = size;
        }
    }
    return max_clause_size;
}

RandomBranching::RandomBranching(PropMapUnique_t &prop_map_unique)
{
    srand(time(NULL));

    for (auto iter = std::begin(prop_map_unique); iter != std::end(prop_map_unique); std::advance(iter, 1))
    {
        raw_prop_map.insert(std::pair<long long, AtomicProposition *>(iter->first, iter->second.get()));
    }
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
PropDecision RandomBranching::NextProposition(const ClauseSetUnique_t &clauses,
                                                    const PropSetRaw_t &unset_props,
                                                    const PropSetRaw_t &set_props) const
{
    PropDecision decision;

    for (auto iter = std::begin(clauses); iter != std::end(clauses); std::advance(iter, 1))
    {
        if ((*iter)->Size() == 1)
        {
            std::set<long long> props = (*iter)->GetPropositionsLongLong();

            for (long long p : props)
            {
                AtomicProposition* prop_ptr = raw_prop_map.at(p);
                decision.clause_buddies.insert(prop_ptr);
                if (prop_ptr->PresentInClause())
                {
                    decision.prop = prop_ptr;
                    decision.was_unit_clause = true;
                }
            }
            return decision;
        }
    }

    // TODO: Build unit propagation into this
    // TODO: Make this more efficient
    PropSetRaw_t::iterator iter;
    do
    {
        iter = std::begin(unset_props);
        std::advance(iter, rand() % unset_props.size());
    } while (std::find(std::begin(set_props), std::end(set_props), (*iter)->GetInverse()) != std::end(set_props));

    decision.prop = *iter;
    decision.was_unit_clause = false;

    return decision;
}

MaxMinClauseHeuristic::MaxMinClauseHeuristic(long long num_vars,
                                             PropMapUnique_t &prop_map_unique)
    : num_vars(num_vars),
      random_brancher(new RandomBranching(prop_map_unique))
{
    // Create a map from proposition names to their pointers
    for (auto iter = std::begin(prop_map_unique); iter != std::end(prop_map_unique); std::advance(iter, 1))
    {
        raw_prop_map.insert(std::pair<long long, AtomicProposition *>(iter->first, iter->second.get()));
    }
}

PropDecision MaxMinClauseHeuristic::NextProposition(const ClauseSetUnique_t &clauses,
                                                          const PropSetRaw_t &unset_props,
                                                          const PropSetRaw_t &set_props) const
{
    PropDecision decision;

    // Get the current maximum clause size (could change when adding clauses)
    size_t max_clause_size = GetMaxClauseSize(clauses);

    std::map<long long, std::set<long long>> unit_clauses_found;

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
            decision.was_unit_clause = clause_size == 1;
            decision.prop = raw_prop_map.at(best_score_prop);      
            return decision;
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
        if (!unit_clauses_found.empty())
        {
            // decision.prop = raw_prop_map.at(*std::begin(unit_clauses_found));
            decision.was_unit_clause = true;
        }else
        {
            decision.prop = raw_prop_map.at(*std::begin(ties));
            decision.was_unit_clause = false;
        }
    }
    else
    {
        decision =  random_brancher->NextProposition(clauses, unset_props, set_props);
    }

    return decision;
}

BohmsBranching::BohmsBranching(long long num_vars,
                               PropMapUnique_t &prop_map_unique)
    : MaxMinClauseHeuristic(num_vars, prop_map_unique)
{
}

int BohmsBranching::Score(std::pair<int, int> prop_pair) const
{
    return alpha * max(prop_pair.first, prop_pair.second) + beta * min(prop_pair.first, prop_pair.second);
}

MomsBranching::MomsBranching(long long num_vars,
                             PropMapUnique_t &prop_map_unique)
    : MaxMinClauseHeuristic(num_vars, prop_map_unique)
{
}

int MomsBranching::Score(std::pair<int, int> prop_pair) const
{
    return (prop_pair.first + prop_pair.second) * (int)pow(2.0, k) + (prop_pair.first * prop_pair.second);
}

JeroslowWang::JeroslowWang(Version version, long long num_vars, PropMapUnique_t &prop_map_unique)
    : random_brancher(new RandomBranching(prop_map_unique)),
      version(version),
      num_vars(num_vars)
{

    srand(time(NULL));

    for (auto iter = std::begin(prop_map_unique); iter != std::end(prop_map_unique); std::advance(iter, 1))
    {
        raw_prop_map.insert(std::pair<long long, AtomicProposition *>(iter->first, iter->second.get()));
    }
}

PropDecision JeroslowWang::NextProposition(const ClauseSetUnique_t &clauses,
                                                 const PropSetRaw_t &unset_props,
                                                 const PropSetRaw_t &set_props) const
{
    PropDecision decision;

    for (auto iter = std::begin(clauses); iter != std::end(clauses); std::advance(iter, 1))
    {
        if ((*iter)->Size() == 1)
        {
            std::set<long long> props = (*iter)->GetPropositionsLongLong();

            for (long long p : props)
            {
                AtomicProposition* prop_ptr = raw_prop_map.at(p);
                decision.clause_buddies.insert(prop_ptr);
                if (prop_ptr->PresentInClause())
                {
                    decision.prop = prop_ptr;
                    decision.was_unit_clause = true;
                }
            }
            return decision;
        }
    }

    decision.was_unit_clause = false;

    size_t max_clause_size = GetMaxClauseSize(clauses);
    std::map<long long, std::vector<int>> prop_count;

    // Setting up a matrix to hold the count of each occurence of a propositoin based on the size of the clause
    for (long long i = (version == Version::TWO_SIDED ? -num_vars : 1); i <= num_vars; i++)
    {
        if (i == 0)
        {
            continue;
        }
        prop_count.insert(std::pair<long long, std::vector<int>>(i, std::vector<int>(max_clause_size, 0)));
    }
    

    // Iterate through each clause
    for (auto clause_iter = std::begin(clauses); clause_iter != std::end(clauses); std::advance(clause_iter, 1))
    {
        // Check to see if this clause is present, and of the desired size
        if ((*clause_iter)->Satisfied())
        {
            continue;
        }

        std::set<long long> clause_longs = (*clause_iter)->GetPropositionsLongLong();

        size_t clause_size = clause_longs.size();

        for (long long prop : clause_longs)
        {
            if (!raw_prop_map.at(prop)->PresentInClause())
            {
                continue;
            }

            if (version == Version::ONE_SIDED && prop < 0)
            {
                continue;
            }

            prop_count[prop][clause_size-1]++;
        }
    }

    

    std::map<long long, float> prop_scores;

    for (long long prop = (version == Version::TWO_SIDED ? -num_vars : 1); prop <= num_vars; prop++)
    {
        if (prop == 0)
        {
            continue;
        }

        float size = 1;
        float score = 0;
        for (int count : prop_count[prop])
        {   
            if (count != 0)
            {
                score += pow(2.0, -size);
            }
            size++;
        }

        prop_scores[prop] = score;
    }

    long long max_prop = 1;
    float max_score = -1.0;

    std::set<long long> ties;
    bool is_tie = false;

    for (long long prop = 1; prop <= num_vars; prop++)
    {
        float score = prop_scores[prop] + (version == Version::TWO_SIDED ? prop_scores[-prop] : 0);
        if (score >= max_score)
        {
            if (abs(score-max_score) < float_error)
            {
                is_tie = true;
                ties.insert(prop);
            }else
            {
                ties.clear();
                is_tie = false;
                ties.insert(prop);
                
                max_score = score;
                max_prop = prop;
            }
        }
    }

    // Randomly choose a proposition if there was a tie
    if (is_tie)
    {
        auto iter = std::begin(ties);
        std::advance(iter, rand() % ties.size());
        max_prop = *iter;
    }

    if (version == Version::TWO_SIDED)
    {
        float reg_score = prop_scores[max_prop];
        float not_score = prop_scores[-max_prop];

        decision.prop = raw_prop_map.at((reg_score >= not_score ? max_prop : -max_prop));
    }else
    {
        decision.prop = raw_prop_map.at(max_prop);
    }

    return decision;
}