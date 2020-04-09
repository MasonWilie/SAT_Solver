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
AtomicProposition *RandomBranching::NextProposition(const ClauseSetUnique_t &clauses, const PropSetRaw_t &unset_props, const PropSetRaw_t &set_props) const
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

BohmsBranching::BohmsBranching(long long num_vars, PropMapUnique_t &prop_map_unique, const ClauseSetUnique_t &clauses) : num_vars(num_vars),
                                                                                    random_brancher(new RandomBranching)
{
    for (auto iter = std::begin(prop_map_unique); iter != std::end(prop_map_unique); std::advance(iter, 1))
    {
        raw_prop_map.insert(std::pair<long long, AtomicProposition*>(iter->first, iter->second.get()));
    }

    for (auto clause_iter = std::begin(clauses); clause_iter != std::end(clauses); std::advance(clause_iter, 1))
    {
        size_t size = (*clause_iter)->Size();
        if ( size > max_clause_size)
        {
            max_clause_size = size;
        }
    }


}

/**
 * @brief Branching algorithm that chooses a variable based on
 * maximizing the cost function proposed in the paper below.
 * 
 * "The Impact of Branching Heuristics in Propositional
 * Satisfiability Algorithms" by Joao Marques-Silva, Page 66
 * 
 * @param clauses All clauses (not used in this algorithm)
 * @param unset_props Propositions that are not currently set
 * @param set_props Propositions that are currently set
 * @return AtomicProposition* Next proposition to set
 */
AtomicProposition *BohmsBranching::NextProposition(const ClauseSetUnique_t &clauses, const PropSetRaw_t &unset_props, const PropSetRaw_t &set_props) const
{
    

    std::set<long long> props_to_consider;
    for (int i = 0; i < num_vars+1; i++)
    {
        props_to_consider.insert(i);
        props_to_consider.insert(-i);
    }

    // Start with clauses of size 1, and go all the way up to the maximum clause size if needed
    for (size_t clause_size = 1; clause_size < max_clause_size+1; clause_size++)
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

            for (long long prop_num : props)
            {
                if (props_to_consider.find(prop_num) == std::end(props_to_consider))
                {
                    continue;
                }

                if (!raw_prop_map.at(prop_num)->PresentInClause())
                {
                    continue;
                }


                if (prop_num < 0)
                {
                    if (prop_map.find(-prop_num) == std::end(prop_map))
                    {
                        prop_map.insert(std::pair<long long, std::pair<int, int>>(-prop_num, std::pair<int, int>(0, 1)));
                    }
                    else
                    {
                        prop_map.at(-prop_num).second++;
                    }
                }
                else
                {
                    if (prop_map.find(prop_num) == std::end(prop_map))
                    {
                        prop_map.insert(std::pair<long long, std::pair<int, int>>(prop_num, std::pair<int, int>(1, 0)));
                    }
                    else
                    {
                        prop_map.at(prop_num).first++;
                    }
                }
            }
        }

        if (prop_map.empty())
        {
            continue;
        }

        long long best_score_prop;
        int best_score = -1;
        bool tie = false;

        std::set<long long> ties;

        for (auto map_iter = std::begin(prop_map); map_iter != std::end(prop_map); std::advance(map_iter, 1))
        {
            
            int score = alpha * max(map_iter->second.first, map_iter->second.second) + beta * min(map_iter->second.first, map_iter->second.second);
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
                    best_score = score;
                    best_score_prop = map_iter->first;
                    tie = false;
                }
            }
        }
        
        if (!tie)
        {
            return raw_prop_map.at(best_score_prop);
        }

        props_to_consider.clear();
        for (long long p : ties)
        {
            props_to_consider.insert(p);
        }
    }
    return random_brancher->NextProposition(clauses, unset_props, set_props);
}