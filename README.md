# SAT Solver With Changeable Branching Algorithm

This project is a [satisfiability](https://en.wikipedia.org/wiki/Boolean_satisfiability_problem) solver that allows you to change the branching algorithm used for choosing new literals to assert. Available algorithms are listed below. Currently, the backtracking algorithm used is the one used in [DPLL](https://en.wikipedia.org/wiki/DPLL_algorithm).

### Algorithms Available

|Algorithm  | Flag          |
|:---------:|:-------------:|
|Bohm       | --bohm |  
|Maximum Occurrences in clauses of Minimum Size | --mom|
|Random | --random|
|Variable State Independent Decaying Sum | --vsids|
|One-sided Jeroslow-Wang | --jw-one|
|Two-sided Jeroslow-Wang | --jw-two|

### Building the Project
Dependencies: Boost, GTest, CMake

The following commands should clone and build the project in a Linux envoronment.
```
$ git clone https://github.com/MasonWilie/SAT_Solver/
$ cd SAT_Solver
$ cmake -H. -Bbuild
$ cd build
$ make
```
This will result in 3 binary files: Solve, Verify, and Test. Solve and Verify do not rely on any other data in the build folder, and therefore are not required to stay there.

### Running the Solver
The solver requires 2 flags, the input filename ("--in" or "-i") which must be followed by the input filename and the branching algorithm to use (from above). It then has an optional argument for an output file ("--out" or "-o") wich needs to be followed by a filename, and will allow the assertions to be saved to a text file in their integer form. If the output filename argument is left off, the satisfying assignments will be fed to standard out. If the input is UNSAT, it will simply print UNSAT with no other output. SAT or UNSAT, the computational time will also be printed to standard out (never to file).

An example run with input filename "test_data/aim-50-1_6-yes1-4.cnf" and output filename "output.txt" using MOM's branching algorithm is shown below.

```
$ ./Solve -i test_data/aim-50-1_6-yes1-4.cnf -o output.txt --mom
Branching type: MOM
SAT
Outputting solution to file
Time to solution: 0.156626 s
```
### Running the Verifier
The verifier is used to verify that a solution given by the solver actually satisfies the problem. It has two required arguments, the filename of the file that was given to the solver ("--in" or "-i") and output filename that the solver spit out ("--out" or "-o"). The verifier will comfirm that the given output actually works (and makes sense). An example run can be seen below using the same filenames as above.

```
$ ./Verify -i test_data/aim-50-1_6-yes1-4.cnf -o output.txt
This solution works.
```
