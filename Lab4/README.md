### Lab 4

## Outcome
i.) Learning how to decompose a big data processing problem into sub-tasks executed by
workers on a network of computers but coordinated by a master task. This phase is
referred to the mapping phase. We will make do with simple shared memory computer.
ii.) Learning how the results from workers performing the sub-tasks are eventually merged into
a final result. This phase is referred to as the reduction phase.
iii.) Learning to use the fundamental principles of how MapReduce works using an alternative
to Hadoop

## CLONING AND RUNNING

Clone the repository using git clone repository https://github.com/PersieB/Parallel-and-Distributed-Computing.git in a folder
## Repository Branches

The github repository has 2 branches, namely "master" and "documentation".
## Master branch

The master branch contains the Lab4 folder that should be cloned to access the source codes - Part 1 and Part2. NB: A README.md is also attached in each lab folder.
## Documentation branch

The documentation branch contains the report for each lab including Lab4 report called "Lab4 Report".
## Accessing source code

Since the main branch is the default, you can simply to the Lab folder in your unix environment.

## Accessing report from documentation branch

Simply use 'git checkout documentation' to move from the master to the documentation branch in your terminal. The folder will be updated and show only the reports. To switch back to master branch, use the git checkout master. The folder will be updated to reflect only files in the master branch i.e lab folders containing source codes.

## How to run the files
Part 1:
Run: 'python3 mrJob_wordCount.py < input.txt

Part 2:
For sequential, run: 'time python3 serial_multiplication.py matrix_multiplication N'
For MapReduce, run: 'time python3 mrJob_multiplication.py matrix.txt --P=p', where p is an even square number denoting the numbers of processors.

More details on how the programs and how to run can be found in the Lab4 report under the documentation branch.
HEAR FROM YOU

Kindly feel free to reach out with any feedback. Thank you!