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
    switch (type)
    {
    case BacktrackingHeuristic::BacktrackingType::DPLL:
        return std::make_unique<DpllBacktracking>();
        break;
    default:
        return nullptr;
    }

    return nullptr;
}

std::unique_ptr<BranchingHeuristic> Solver::CreateBranchingHeuristic(BranchingHeuristic::BranchingType type)
{
    std::unique_ptr<BranchingHeuristic> heuristic;
    switch (type)
    {
    case BranchingHeuristic::BranchingType::RANDOM:
        heuristic = std::unique_ptr<BranchingHeuristic>(new RandomBranching(prop_map));
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
    case BranchingHeuristic::BranchingType::VSIDS:
        heuristic = std::unique_ptr<BranchingHeuristic>(new VsidsBranching(prop_map, clauses));
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
    AtomicProposition *next_prop;

    next_prop = nullptr;

    do
    {
        if (!prop_from_backtrack)
        {
            next_prop = branching_heuristic->NextProposition(clauses, unset_props, set_props);
        }

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

std::set<std::set<int>> Solver::GetClausesAsInts() const
{
    std::set<std::set<int>> clauses_long_long;
    for (ClauseSetUnique_t::iterator iter = std::begin(clauses); iter != std::end(clauses); std::advance(iter, 1))
    {
        clauses_long_long.insert((*iter)->GetPropsAsInts());
    }
    return clauses_long_long;
}

void Solver::ReadFile(std::string filename)
{
    std::ifstream in(filename);

    if (!in.is_open())
    {
        printf("Error opening file '%s'\n", filename.c_str());
        throw std::runtime_error("Error opening file");
    }

    int num_clauses = 0;
    bool p_found = false;

    std::vector<std::string> element_strings;

    size_t line_count{0};
    int clause_count{0};
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

            for (int i = 1; i <= num_vars; i++)
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
                printf("Encountered non-comment or metadata line prior to reading needed metadata information at line %lu\n", line_count);
                ThrowFormatErrorWithUrl();
            }

            std::vector<std::string> line_elements = SpaceDelimit(line);

            for (std::string s : line_elements)
            {
                element_strings.push_back(s);
            }
        }
    }


    // Parsing the strings found to seperate the clauses
    for (std::string s : element_strings)
    {
        // Convert the string to an integer
        int prop_num;
        try
        {
            prop_num = std::stoi(s);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            std::cout << "Error converting '" << s << "' into an integer." << std::endl;
            exit(1);
        }

        // Division between clauses found
        if (prop_num == 0)
        {
            if (clause_count == num_clauses)
            {
                std::cout << "More clauses encountered than expected" << std::endl;
                exit(1);
            }

            if ((*clause_iterator)->Size() == 0)
            {
                std::cout << "Trying to add an empty clause to the set of clauses" << std::endl;
            }

            std::advance(clause_iterator, 1);
            clause_count++;
        }else
        {
            if (prop_num > num_vars || prop_num < -num_vars)
            {
                std::cout << "Variable " << prop_num << " is out of range" << std::endl;
                exit(1);
            }

            (*clause_iterator)->AddProposition(prop_map[prop_num].get());
        }
    }

    for (PropMapUnique_t::iterator iter = std::begin(prop_map); iter != std::end(prop_map); iter++)
    {
        unset_props.insert(iter->second.get());
        set_props.clear();
    }
}