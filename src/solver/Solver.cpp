#include "Solver.h"

Solver::Solver(std::string filename)
{
    ReadFile(filename);
}

std::vector<std::string> Solver::SpaceDelimit(std::string s)
{
    std::vector<std::string> strings;
    std::string element = "";

    bool saw_char = false;

    for (char c : s)
    {
        if (c == ' ' || c == '\t')
        {
            if (!saw_char)
            {
                continue;
            }
            saw_char = false;
            strings.push_back(element);
            element = "";
        }
        else
        {
            saw_char = true;
            element += c;
        }
    }
    if (element.size() != 0)
    {
        strings.push_back(element);
    }
    return strings;
}

inline void ThrowFormatErrorWithUrl()
{
    throw std::runtime_error("File format error. See http://www.satcompetition.org/2009/format-benchmarks2009.html");
}

std::unique_ptr<BacktrackingHeuristic> Solver::CreateBacktrackingHeuristic(BacktrackingHeuristic::BacktrackingType type)
{
    std::unique_ptr<BacktrackingHeuristic> heuristic;
    switch (type)
    {
    case BacktrackingHeuristic::BacktrackingType::STANDARD:
        heuristic = std::unique_ptr<BacktrackingHeuristic>(new StandardBacktracking);
        break;
    default:
        heuristic = nullptr;
    }

    return std::move(heuristic);
}

std::unique_ptr<BranchingHeuristic> Solver::CreateBranchingHeuristic(BranchingHeuristic::BranchingType type)
{
    std::unique_ptr<BranchingHeuristic> heuristic;
    switch (type)
    {
    case BranchingHeuristic::BranchingType::RANDOM:
        heuristic = std::unique_ptr<BranchingHeuristic>(new RandomBranching);
        break;
    case BranchingHeuristic::BranchingType::BOHM:
        heuristic = std::unique_ptr<BranchingHeuristic>(new BohmsBranching(num_vars, prop_map));
        break;
    case BranchingHeuristic::BranchingType::MOMS:
        heuristic = std::unique_ptr<BranchingHeuristic>(new MomsBranching(num_vars, prop_map));
        break;
    case BranchingHeuristic::BranchingType::JW1:
        heuristic = std::unique_ptr<BranchingHeuristic>(new JeroslowWang(JeroslowWang::Version::ONE_SIDED, num_vars, prop_map));
        break;
    case BranchingHeuristic::BranchingType::JW2:
        heuristic = std::unique_ptr<BranchingHeuristic>(new JeroslowWang(JeroslowWang::Version::TWO_SIDED, num_vars, prop_map));
        break;
    default:
        heuristic = nullptr;
    }

    return std::move(heuristic);
}

Solver::FormulaStatus Solver::Status() const
{
    Solver::FormulaStatus status;

    bool SAT{true}, UNSAT{false};

    for (auto clause_iter = std::begin(clauses); clause_iter != std::end(clauses); clause_iter++)
    {
        SAT &= clause_iter->get()->Satisfied();
        UNSAT |= clause_iter->get()->False();
    }

    if (SAT)
    {
        status = Solver::FormulaStatus::SAT;
    }
    else if (UNSAT)
    {
        status = Solver::FormulaStatus::UNSTAT;
    }
    else
    {
        status = Solver::FormulaStatus::CONTINUE;
    }

    return status;
}

Solver::Solution Solver::Solve(const BranchingHeuristic::BranchingType branching_type,
                               const BacktrackingHeuristic::BacktrackingType backtracking_type)
{
    Solver::Solution solution;

    std::unique_ptr<BranchingHeuristic> branching_heuristic(CreateBranchingHeuristic(branching_type));
    std::unique_ptr<BacktrackingHeuristic> backtracking_heuristic(CreateBacktrackingHeuristic(backtracking_type));

    bool prop_from_backtrack{false};
    AtomicProposition *next_prop = nullptr;

    const double max_iterations = pow(2, (prop_map.size() / 2));

    double iteration{0.0};

    do
    {
        if (!prop_from_backtrack)
        {
            next_prop = branching_heuristic->NextProposition(clauses, unset_props, set_props);
        }

        iteration += 1.0;

        unset_props.erase(next_prop);
        set_props.insert(next_prop);
        next_prop->Assert();
        backtracking_heuristic->Update(next_prop);

        switch (Status())
        {
        case Solver::FormulaStatus::CONTINUE:
            prop_from_backtrack = false;
            break;
        case Solver::FormulaStatus::SAT:
            solution.SAT = true;
            for (auto iter = std::begin(set_props); iter != std::end(set_props); std::advance(iter, 1))
            {
                solution.assertions.insert((*iter)->GetName());
            }
            return solution;
            break;
        case Solver::FormulaStatus::UNSTAT:
            next_prop = backtracking_heuristic->Backtrack(clauses, unset_props, set_props);
            prop_from_backtrack = true;
            break;
        default:
            break;
        }

    } while (next_prop);

    solution.SAT = false;
    return solution;
}

std::set<std::set<long long>> Solver::GetClausesAsLongs() const
{
    std::set<std::set<long long>> clauses_long_long;
    for (ClauseSetUnique_t::iterator iter = std::begin(clauses); iter != std::end(clauses); std::advance(iter, 1))
    {
        clauses_long_long.insert((*iter)->GetPropositionsLongLong());
    }
    return clauses_long_long;
}

//TODO: Refactor this to make it less of a mess. Break up into more functions.
void Solver::ReadFile(std::string filename)
{
    std::ifstream in(filename);

    if (!in.is_open())
    {
        printf("Error opening file '%s'\n", filename.c_str());
        throw std::runtime_error("Error opening file");
    }

    int num_clauses;
    bool p_found = false;

    size_t line_count{0};
    size_t clause_count{0};
    ClauseSetUnique_t::iterator clause_iterator;
    while (!in.eof())
    {
        std::string line;
        std::getline(in, line);
        line_count++;

        if (line.size() == 0 || line[0] == 'c')
        {
            continue;
        }

        if (line[0] == 'p')
        {
            if (p_found)
            {
                printf("Encountered second line of metadata on line %lu\n", line_count);
                ThrowFormatErrorWithUrl();
            }

            std::vector<std::string> line_elements = SpaceDelimit(line);
            if (line_elements.size() != 4)
            {
                printf("Incorrectly formatted metadata info on line %lu. Not enough arguments.\n", line_count);
                ThrowFormatErrorWithUrl();
            }

            if (line_elements[1] != "cnf" && line_elements[1] != "CNF")
            {
                printf("Error in metadata on line %lu\n", line_count);
                printf("File says it is in '%s' format. Currently, CNF is the only format supported by this solver\n", line_elements[1].c_str());
                ThrowFormatErrorWithUrl();
            }

            try
            {
                num_vars = std::stoi(line_elements[2]);
                num_clauses = std::stoi(line_elements[3]);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
                printf("Error in metadata on line %lu\n", line_count);
                printf("Either the number of variables (read in as '%s') or the number of clauses (read in as '%s') could not be converted into an int.\n", line_elements[2].c_str(), line_elements[3].c_str());
                ThrowFormatErrorWithUrl();
            }

            for (long long i = 1; i <= num_vars; i++)
            {
                AtomicProposition::PropPair pair = AtomicProposition::CreatePropPair(i);
                prop_map[i] = std::unique_ptr<AtomicProposition>(pair.regular);
                prop_map[-i] = std::unique_ptr<AtomicProposition>(pair.notted);
            }

            for (int i = 0; i < num_clauses; i++)
            {
                clauses.insert(std::make_unique<Clause>());
            }

            clause_iterator = std::begin(clauses);

            p_found = true;
        }
        else
        {
            if (!p_found)
            {
                // printf("Encountered non-comment or metadata line prior to reading needed metadata information at line %lu\n", line_count);
                ThrowFormatErrorWithUrl();
            }

            std::vector<std::string> line_elements = SpaceDelimit(line);

            if (line_elements[line_elements.size() - 1] != "0")
            {
                // printf("Error in line %lu. All clause lines should end in a '0'.\n", line_count);
                ThrowFormatErrorWithUrl();
            }

            for (size_t i = 0; i < line_elements.size() - 1; i++)
            {
                long long prop_num;
                try
                {
                    prop_num = (long long)std::stoi(line_elements[i]);
                }
                catch (const std::exception &e)
                {
                    // printf("Error reading variable in line %lu. Encountered string '%s', which needs to be an int between -%ll and %ll (excluding 0).", line_count, line_elements[i].c_str(), num_vars, num_vars);
                    ThrowFormatErrorWithUrl();
                }

                if (prop_num == 0 || prop_num > num_vars || prop_num < -num_vars)
                {
                    // printf("Invalid variable at line %lu. Needs to be between -%ll and %ll excluding 0", line_count, num_vars, num_vars);
                    ThrowFormatErrorWithUrl();
                }

                if (prop_map.find(prop_num) == std::end(prop_map))
                {
                    // printf("Proposition given by the string '%s' not found.\n", line_elements[i].c_str());
                    ThrowFormatErrorWithUrl();
                }

                if ((*clause_iterator)->ContainsProposition(prop_map[-prop_num].get()))
                {
                    // printf("Cannot have oposite propositions in the same line. Clause at line %lu is a tautology.\n", line_count);
                    ThrowFormatErrorWithUrl();
                }

                (*clause_iterator)->AddProposition(prop_map[prop_num].get());
            }

            std::advance(clause_iterator, 1);
            clause_count++;
            if (clause_count > num_clauses)
            {
                printf("Number of clauses encountered (%lu) does not match number of clauses expected (%d).\n", clause_count, num_clauses);
                ThrowFormatErrorWithUrl();
            }
        }
    }

    for (PropMapUnique_t::iterator iter = std::begin(prop_map); iter != std::end(prop_map); iter++)
    {
        unset_props.insert(iter->second.get());
        set_props.clear();
    }
}