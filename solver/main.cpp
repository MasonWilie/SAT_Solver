#include "ProgramOptions.hpp"
#include <chrono>

int main(int argc, char** argv)
{
    ProgramOptions options = GetProgramOptions(argc, argv);
    std::chrono::high_resolution_clock::time_point start, stop;

    Solver solver(options.input_filename);

    start = std::chrono::high_resolution_clock::now();
    Solver::Solution solution = solver.Solve(options.branching_type, options.backtrack_type);
    stop = std::chrono::high_resolution_clock::now();

    double time = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start).count();

    if (!solution.SAT)
    {
        std::cout << "UNSAT\n";
    }else if (!options.output_to_file)
    {
        std::cout << "SAT\n";
        std::cout << "Assertions: ";
        for (auto iter = std::begin(solution.assertions); iter != std::end(solution.assertions); std::advance(iter, 1))
        {
            std::cout << *iter << " ";
        }
        std::cout << "\n";
        return 0;
    }else
    {
        std::cout << "SAT\n";
        std::cout << "Outputting solution to file\n";
    }

    if (options.output_to_file)
    {
        solution.WriteToFile(options.output_filename, time);
    }

    std::cout << "Time to solution: " << std::chrono::duration_cast<std::chrono::duration<double>>(stop - start).count() << " s\n";
    
    return 0;
}