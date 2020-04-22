import subprocess
import enum
import os
import glob
import threading
import time
import sys

dirname = os.path.dirname(os.path.realpath(__file__)) + "/"

class Algorithm(enum.Enum):
    MOM = 0
    BOHM = 1
    RAND = 2
    JONE = 3
    JTWO = 4
    SIDS = 5 


def CreateCommand(input_filename, output_filename, algorithm):
    if algorithm == Algorithm.BOHM:
        alg_s = "bohm"
    elif algorithm == Algorithm.MOM:
        alg_s = "mom"
    elif algorithm == Algorithm.JONE:
        alg_s = "jw-one"
    elif algorithm == Algorithm.JTWO:
        alg_s = "jw-two"
    elif algorithm == Algorithm.SIDS:
        alg_s = "vsids"
    elif algorithm == Algorithm.RAND:
        alg_s = "random"

    return ["./Solve", "-i", input_filename, "-o", output_filename, "--{}".format(alg_s)]


def GetFileInfo(filename):
    with open(filename, "r") as file:
        lines = file.readlines()
        for l in lines:
            elements = l.split()
            if elements[0] == 'p':
                num_vars = int(elements[2])
                num_clauses = int(elements[3])
                break
            
    return num_vars, num_clauses

class RunCmd(threading.Thread):
    def __init__(self, cmd, timeout):
        threading.Thread.__init__(self)
        self.cmd = cmd
        self.timeout = timeout

    def run(self):
        self.p = subprocess.Popen(self.cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        self.p.wait()

    def Run(self):
        self.start()
        self.join(self.timeout)

        if self.is_alive():
            self.p.terminate()
            self.join()

class Runtime:
    def __init__(self, filename):
        self.filename = filename
        self.num_vars, self.num_clauses = GetFileInfo(filename)
        self.times = [-1, -1, -1, -1, -1, -1]
        self.iterations = [-1, -1, -1, -1, -1, -1]

    def SetTime(self, time, alg):
        self.times[alg.value] = time
    def SetIteration(self, iterations, alg):
        self.iterations[alg.value] = iterations

class OutputFiles:
    def __init__(self, time_filename, iter_filename):
        self.time_file = open(time_filename, "w")
        self.iter_file = open(iter_filename, "w")
        self.time_file.write("Filename,Num Vars,Num Clauses,MOM,Bohm,random,onesided jw,twosided jw,SIDS\n")
        self.iter_file.write("Filename,Num Vars,Num Clauses,MOM,Bohm,random,onesided jw,twosided jw,SIDS\n")

    def WriteRuntime(self, runtime):
        self.time_file.write(runtime.filename)
        self.time_file.write(",")
        self.time_file.write(str(runtime.num_vars))
        self.time_file.write(",")
        self.time_file.write(str(runtime.num_clauses))
        for t in runtime.times:
            self.time_file.write(",")
            if t != -1:
                self.time_file.write(str(t))
        self.time_file.write("\n")

        self.iter_file.write(runtime.filename)
        self.iter_file.write(",")
        self.iter_file.write(str(runtime.num_vars))
        self.iter_file.write(",")
        self.iter_file.write(str(runtime.num_clauses))
        for t in runtime.iterations:
            self.iter_file.write(",")
            if t != -1:
                self.iter_file.write(str(t))
        self.iter_file.write("\n")




def GetTimeAndIterations(filename):
    with open(filename, "r") as file:
        lines = file.readlines()
        if len(lines) == 2:
            return float(lines[0]), float(lines[1])
        elif len(lines) == 3:
            return float(lines[1]), float(lines[2])
        else:
            return float(-1), float(-1)

def main():

    if not os.path.exists(dirname + "Solve"):
        print("No 'Solve' binary found in directory.")
        print("You must build the 'Solve' binary and move it to this directory.")
        return

    if len(sys.argv) != 2:
        print("Too many arguments passed in")
        return

    if sys.argv[1] in ["-h", "--help"]:
        print("Benchmarking suit interface:")
        print("This benchmarking suite will take example .cnf files from the folder 'formulas'")
        print("and save the time and number of iterations each file took for different branching")
        print("algorithms to the files 'results_time.csv' and 'results_iter.csv' respectively.")
        print("The program taks one argument, the maximum amount of time each algorithm is given")
        print("for an output file before giving up. Entries which did not complete in that time")
        print("are marked with an empty cell in the output file. An example run is shown below,")
        print("with the maximum time allowed as 14.5 seconds.")
        print("")
        print("\t $ python3 benchmark.py 14.5")
        print("")
        print("The .cnf file format is descibed at the source below, with many example files\n found online.\n")
        print("\t https://people.sc.fsu.edu/~jburkardt/data/cnf/cnf.html\n")
        return
    
    try:
        time = float(sys.argv[1])
    except:
        print("Error converting the time {} into a floating point number".format(sys.argv[1]))
        return

    filenames = glob.glob(dirname + "formulas/*.cnf")

    output_filename = dirname + "out.txt"

    all_algs = [Algorithm.MOM,
                Algorithm.BOHM,
                Algorithm.RAND,
                Algorithm.JONE,
                Algorithm.JTWO,
                Algorithm.SIDS]

    master_output = OutputFiles("results_time.csv", "results_iter.csv")

    for input_filename in filenames:
        try:
            runtime = Runtime(input_filename)
        except:
            continue

        for alg in all_algs:
            command = CreateCommand(input_filename, output_filename, alg)
            RunCmd(command, time).Run()

            if os.path.exists(output_filename):
                run_time, run_iter = GetTimeAndIterations(output_filename)
                runtime.SetTime(run_time, alg)
                runtime.SetIteration(run_iter, alg)
                RunCmd(["rm", output_filename], 10).Run()
        master_output.WriteRuntime(runtime)
        print("Finished {}.".format(input_filename))
            
if __name__ == "__main__":
    main()
