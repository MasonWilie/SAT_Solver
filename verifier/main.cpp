// This is a simple verifier and is by no means efficient
#include <string>
#include <set>
#include <boost/program_options.hpp>
#include <iostream>
#include <vector>
#include <fstream>

struct ProgramOptions
{
    std::string input_filename;
    std::string output_filename;
};

ProgramOptions ReadProgramOptions(int argc, char **argv)
{
    ProgramOptions opts;

    boost::program_options::options_description desc("Verifier Usage");

    desc.add_options()("help,h", "print usage message")("in,i", boost::program_options::value<std::string>(), "Input file given to SAT solver (required)")("out,o", boost::program_options::value<std::string>(), "Output file given by SAT solver (required)");

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help") || !vm.count("in") || !vm.count("out"))
    {
        std::cout << desc << "\n";
        exit(0);
    }

    opts.output_filename = vm["out"].as<std::string>();
    opts.input_filename = vm["in"].as<std::string>();

    return opts;
}

std::vector<std::string> SpaceDelimit(std::string s)
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

std::set<int> ReadOutput(const std::string filename)
{
    std::ifstream in(filename);
    if (!in.is_open())
    {
        std::cout << "Error opening file '" << filename << "'" << std::endl;
        exit(0);
    }

    std::string line;
    std::getline(in, line);

    std::vector<std::string> assertions_strings = SpaceDelimit(line);

    if (assertions_strings.back() == " ")
    {
        assertions_strings.pop_back();
    }

    std::set<int> assertions;

    for (std::string s : assertions_strings)
    {
        int assert = std::stoi(s);
        if (assertions.find(assert) != std::end(assertions))
        {
            std::cout << "Duplicate values in the output file. "
                      << assert
                      << " is repeated more than once."
                      << std::endl;
            exit(0);
        }

        if (assertions.find(-assert) != std::end(assertions))
        {
            std::cout << "Both the negated and regular version of "
                      << (assert < 0 ? -assert : assert)
                      << " apear in the output file. This is impossible."
                      << std::endl;
            exit(0);
        }

        assertions.insert(assert);
    }

    return assertions;
}

std::set<std::set<int>> ReadInputFile(const std::string filename)
{
    std::ifstream in(filename);
    if (!in.is_open())
    {
        std::cout << "Error opening input file "
                  << filename
                  << std::endl;

        exit(0);
    }

    std::string line;
    std::set<std::set<int>> clauses;
    std::set<int> clause;
    while(!in.eof())
    {
        std::getline(in, line);
        if (line[0] == 'p' || line[0] == 'c')
        {
            continue;
        }

        std::vector<std::string> clause_strings = SpaceDelimit(line);

        for (std::string s : clause_strings)
        {
            int prop_num = std::stoi(s);

            if (prop_num == 0)
            {
                clauses.insert(clause);
                clause.clear();
            }else
            {
                clause.insert(prop_num);
            }
            
        }
    }

    return clauses;

}

int main(int argc, char **argv)
{

    ProgramOptions opts = ReadProgramOptions(argc, argv);

    std::set<int> assertions = ReadOutput(opts.output_filename);
    std::set<std::set<int>> clauses = ReadInputFile(opts.input_filename);

    for (std::set<int> clause : clauses)
    {
        bool clause_satisfied = false;
        for (int prop : clause)
        {
            if (assertions.find(prop) != std::end(assertions))
            {
                clause_satisfied = true;
                break;
            }
        }

        if (!clause_satisfied)
        {
            std::cout << "This solution does not satisfy the problem." << std::endl;
            std::cout << "Clause {";
            for (int prop : clause)
            {
                std::cout << prop << " ";
            }
            std::cout << "} is not satisfied." << std::endl;
            return 0;
        }
    }

    std::cout << "This solution works." << std::endl;

    return 0;
}