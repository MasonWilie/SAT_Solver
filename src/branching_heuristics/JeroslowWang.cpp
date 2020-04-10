#include "JeroslowWang.h"


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

AtomicProposition *JeroslowWang::NextProposition(const ClauseSetUnique_t &clauses,
                                                 const PropSetRaw_t &unset_props,
                                                 const PropSetRaw_t &set_props) const
{
    for (auto iter = std::begin(clauses); iter != std::end(clauses); std::advance(iter, 1))
    {
        if ((*iter)->Size() == 1)
        {
            std::set<long long> props = (*iter)->GetPropositionsLongLong();

            for (long long p : props)
            {
                AtomicProposition *prop_ptr = raw_prop_map.at(p);
                if (prop_ptr->PresentInClause())
                {
                    return prop_ptr;
                }
            }
        }
    }

    size_t max_clause_size = BranchingHeuristic::GetMaxClauseSize(clauses);
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

            prop_count[prop][clause_size - 1]++;
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
            if (abs(score - max_score) < float_error)
            {
                is_tie = true;
                ties.insert(prop);
            }
            else
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

        return raw_prop_map.at((reg_score >= not_score ? max_prop : -max_prop));
    }
    return raw_prop_map.at(max_prop);

}