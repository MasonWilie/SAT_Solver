#ifndef PROGRAM_OPTIONS_H
#define PROGRAM_OPTIONS_H

#include <boost/program_options.hpp>
#include <iostream>

#include "Solver.h"


struct ProgramOptions
{
    std::string input_filename;
    std::string output_filename;

    bool help{false};
    bool output_to_file{false};

    BacktrackingHeuristic::BacktrackingType backtrack_type;
    BranchingHeuristic::BranchingType branching_type;
};

ProgramOptions GetProgramOptions(int argc, char** argv)
{

    std::string input_filename, output_filename;
    boost::program_options::options_description desc("SAT Solver Usage");

    ProgramOptions opts;

    desc.add_options()
        ("help,h", "print usage message")
        ("in,i", boost::program_options::value<std::string>(), "input filename (required)")
        ("out,o", boost::program_options::value<std::string>(), "output filename")
        ("random-branch", "Use the random backtracking algorithm (default)")
        ("bohm-branch", "Use Bohm's branching algorithm")
        ("mom-branch", "Use MOMs branching algorithm")
        ("jw-one-branch", "Use the one sided Jeroslow-Wang branching algorithm")
        ("jw-two-branch", "Use the two sided Jeroslow-Wang branching algorithm")
        ("dpll-backtrack", "Use the DPLL backtracking algorithm (default)")
        ("cdcl-backtrack", "Use the CDCL backtracking algorithm");

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help") || !vm.count("in")) {  
        std::cout << desc << "\n";
        opts.help = true;
        exit(0);
    }

    if (vm.count("out"))
    {
        opts.output_to_file = true;
        opts.output_filename = vm["out"].as<std::string>();
    }
    opts.input_filename = vm["in"].as<std::string>();

    std::cout << "Branching type: ";
    if (vm.count("bohm-branch"))
    {
        opts.branching_type = BranchingHeuristic::BranchingType::BOHM;
        std::cout << "Bohm's";
    }else if (vm.count("random-branch"))
    {
        opts.branching_type = BranchingHeuristic::BranchingType::RANDOM;
        std::cout << "Random";
    }else if (vm.count("mom-branch"))
    {
        opts.branching_type = BranchingHeuristic::BranchingType::MOMS;
        std::cout << "MOM";
    }else if (vm.count("jw-one-branch"))
    {
        opts.branching_type = BranchingHeuristic::BranchingType::JW1;
        std::cout << "One sided Jeroslow-Wang";
    }else if (vm.count("jw-two-branch"))
    {
        opts.branching_type = BranchingHeuristic::BranchingType::JW1;
        std::cout << "Two sided Jeroslow-Wang";
    }else
    {
        std::cout << "ERROR - None specified\n";
        std::cout << desc << "\n";
        exit(0);
    }
    std::cout << std::endl;

    std::cout << "Backtrack type: ";
    if (vm.count("dpll-backtrack"))
    {
        opts.backtrack_type = BacktrackingHeuristic::BacktrackingType::DPLL;
        std::cout << "DPLL";
    }else if (vm.count("cdcl-backtrack"))
    {
        opts.backtrack_type = BacktrackingHeuristic::BacktrackingType::CDCL;
        std::cout << "CDCL";
    }else
    {
        std::cout << "ERROR - None specified\n";
        std::cout << desc << "\n";
        exit(0);
    }
    std::cout << std::endl;

    return opts;
}

#endif