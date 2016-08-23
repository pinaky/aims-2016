The MIT License (MIT)
Copyright (c) 2016 Pinaky Bhattacharyya

Asymptotically Independent Markov Sampling
[An independent implentation in C++11]
[Based on the algorithm prescribed at: https://arxiv.org/abs/1110.1880]
[This is an UNOFFICIAL implementation of the algorithm]

=== TABLE OF CONTENTS ===

1. Introduction
2. Organization of the code
3. Description of the classes
4. Compiling the code
5. Running the tests
6. Running the example
7. Implementing your own instance
8. Extending the code

===

1. INTRODUCTION

This C++11 code is my own implementation of the AIMS algorithm. There are two 
reasons that I used C++:
- Performance: C++ is a compiled and allows fine control over memory 
allocation and threading. The most expensive part of any Markov chain Monte-
Carlo algorithm is usually the likelihood evaluation. This can be 
parallelized independently using OpenMP, for example.
- OOP/Inheritance: Different portions of the AIMS code are performed by 
member dfunctions of different abstract classes. This allows one to introduce
more sophisticated functionality without breaking the code. For instance, the
abstract class NumSamplesScheduler has a member function Solve() that returns
the number of samples according to the schedule desired by the coder. 
Currently, a specific implementation, NumSamplesScheduler_Constant, returns a
constant number of samples for each annealing stage through Solve(). One 
could extend NumSamplesScheduler NumSamplesScheduler_InverseSquare, if 
desired.

That said, this code could be incorporated into a higher-level language such
as R / Python for ease of use in the future.


2. ORGANIZATION OF THE CODE

./include contains all the header files.
./src contains source files for the AIMS library.
./src/example contains an sample implementation for a Gaussian forward model
	with a uniform prior.
./src/tests contains test code.


3. DESCRIPTION OF THE CLASSES
<TODO>


4. COMPILING THE CODE

Run the 'make' command from the shell.

'make' will generate 'libaims.a' and store it in ./lib
'make' will generate 'tests' and store it in ./bin
'make' will generate 'example' and store it in ./bin


5. RUNNING THE TESTS

Simply navigate to ./bin and run the tests' executable.


6. RUNNING THE EXAMPLE

Run the 'example' executable and redirect the sample input file 
provided in the ./extras folder. You could do this, for example,
using:

./bin/example < ./extras/input.txt

This should generate an output file, 'outfile.json', in the
root directory. This output file contains all the output in JSON
format. There is a list of 'stages' that contain all the data
from each AIMS annealing stage, such as:
- j: the stage number
- beta: the value of the annealing parameter
- acc: binary value indicating whether a candidate was accepted
- accprob: calculated acceptance probability for the candidate
- wcurr & wnext: importance weights defined in the paper
- Neff: number of effective samples
- x: the samples themselves
- k: the index of the random sample chosen from the previous chain
The JSON format is easily read using a simple Python script such
as:

import json
# Open the file and read the data
with open(filepath) as file:
	data = json.load(file)
# Then access the data from any stage. Example: Posterior mean
import numpy
posterior_mean = numpy.mean(data['stages'][-1]['x'])

You can run the sample post-processor provided in the extras
directory after running the example with 'outfile.json' in 
the extras directory. By default, 'postprocess.py' looks for
'outfile.json' in the same directory. You can change this in 
the script.


7. IMPLEMENTING YOUR OWN INSTANCE

If you would like to use this version of AIMS to draw samples from
your own posterior, you need to specify the following:
- Acceptance Probability Service
- Number of Samples Scheduler
- Beta Scheduler
- Proposal Scheduler
- Chain Printer
- Prior
- Forward Model

Thankfully, you can get away with specifying only the Prior and the 
Forward Model. You can use existing implementations of the remaining
services.

Even for the prior, if you are content with a uniform hyper-
rectangle, simply use the General Uniform Prior provided.

Finally, for the Forward Model, simply implement a class that 
has a public member named LogLikelihood that takes a vector<double>
const & and returns a double. The rest is up to you.

So basically, simply follow the example in ./src/example, but replace
the Gaussian Forward Model with your own version.


8. EXTENDING THE CODE
<TODO>

