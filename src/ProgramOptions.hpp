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
};

ProgramOptions GetProgramOptions(int argc, char** argv)
{

    std::string input_filename, output_filename;
    boost::program_options::options_description desc("SAT Solver Usage");

    ProgramOptions opts;

    desc.add_options()
        ("help,h", "print usage message")
        ("in,i", boost::program_options::value<std::string>(), "input filename (required)")
        ("out,o", boost::program_options::value<std::string>(), "output filename");

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

    return opts;
}

#endif