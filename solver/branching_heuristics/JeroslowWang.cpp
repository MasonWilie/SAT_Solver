#include "JeroslowWang.h"


JeroslowWang::JeroslowWang(Version version, int num_vars, LitMapUnique_t &lit_map_unique)
    : random_brancher(new RandomBranching(lit_map_unique)),
      version(version),
      num_vars(num_vars)
{

    srand(time(NULL));

    for (auto iter = std::begin(lit_map_unique); iter != std::end(lit_map_unique); std::advance(iter, 1))
    {
        raw_lit_map.insert(std::pair<int, Literal *>(iter->first, iter->second.get()));
    }
}

Literal *JeroslowWang::NextLiteral(const ClauseSetUnique_t &clauses,
                                                 const LitSetRaw_t &unset_lits,
                                                 const LitSetRaw_t &set_lits) const
{

    size_t max_clause_size = BranchingHeuristic::GetMaxClauseSize(clauses);
    std::map<Literal *, std::vector<int>> lit_count;

    // Setting up a matrix to hold the count of each occurence of a literal based on the size of the clause
    for (auto iter = std::begin(raw_lit_map); iter != std::end(raw_lit_map); std::advance(iter, 1))
    {
        lit_count.insert(std::pair<Literal*, std::vector<int>>(iter->second, std::vector<int>(max_clause_size, 0)));
    }

    // Iterate through each clause and store counts in a vector where the index corresponds to the size of
    // the clause which the literal was found in
    for (auto clause_iter = std::begin(clauses); clause_iter != std::end(clauses); std::advance(clause_iter, 1))
    {
        // Check to see if this clause is present, and of the desired size
        if ((*clause_iter)->Satisfied())
        {
            continue;
        }

        // Count every literal in the clause and store it based on its size
        LitSetRaw_t clause_lits = (*clause_iter)->GetLitsAsPtrs();
        size_t clause_size = clause_lits.size();
        for (Literal* lit : clause_lits)
        {
            if (version == Version::ONE_SIDED && lit->IsNot())
            {
                continue;
            }
            lit_count[lit][clause_size - 1]++;
        }
    }


    // Calculate the scores over the literals
    std::map<Literal*, float> lit_scores;
    for (auto iter = std::begin(lit_count); iter != std::end(lit_count); std::advance(iter, 1))
    {
        if (version == Version::ONE_SIDED && iter->first->IsNot())
        {
            continue;
        }

        float size = 1;
        float score = 0;
        for (int count : lit_count[iter->first])
        {
            if (count != 0)
            {
                score += pow(2.0, -size);
            }
            size += 1.0;
        }

        lit_scores[iter->first] = score;
    }

    Literal* max_lit;
    float max_score = -1.0;

    std::set<Literal*> ties;
    bool is_tie = false;

    for (auto iter = std::begin(lit_scores); iter != std::end(lit_scores); std::advance(iter, 1))
    {
        if (iter->first->IsAsserted() || iter->first->GetInverse()->IsAsserted())
        {
            continue;
        }

        float score = lit_scores[iter->first] + (version == Version::TWO_SIDED ? lit_scores[iter->first->GetInverse()] : 0);
        if (score >= max_score)
        {
            if (abs(score - max_score) < float_error)
            {
                is_tie = true;
                ties.insert(iter->first);
            }
            else
            {
                ties.clear();
                is_tie = false;
                ties.insert(iter->first);

                max_score = score;
                max_lit = iter->first;
            }
        }
    }

    // Randomly choose a literal if there was a tie
    if (is_tie)
    {
        auto iter = std::begin(ties);
        std::advance(iter, rand() % ties.size());
        max_lit = *iter;
    }

    if (version == Version::TWO_SIDED)
    {
        float reg_score = lit_scores[max_lit];
        float not_score = lit_scores[max_lit->GetInverse()];

        max_lit = reg_score >= not_score ? max_lit : max_lit->GetInverse();
    }

    if (set_lits.find(max_lit) != std::end(set_lits) || set_lits.find(max_lit->GetInverse()) != std::end(set_lits))
    {
        std::cout << "Choosing a lit that is already set" << std::endl;
    }

    return max_lit;

}