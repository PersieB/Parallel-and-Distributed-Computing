# Lab Assignment 2

## Outcome
The main purpose of this laboratory exercise is to refresh and learn the use of C and then explore,
learn and apply shared memory programming libraries, i.e., Pthread and OpenMP, for simple
scientific applications of manipulating very large matrices that are maintained in memory. The
main outcome will be:

i.) learning to use the Pthread and OpenMP parallel programming libraries.
ii.) learning to compile and run parallel shared memory programs in Linux environment.
iii.) learning to evaluate the performance of your parallel programs by timing.

## Task
Write algorithms to compute the transpose of a matrix using OpenMP and Pthreads Library

For OpenMP:
1. A Naive-OpenMP Algorithm
2. Diagonal-Threads Algorithm

For PThreads:
1. Diagonal-Threads Algorithm

# OpenMP Naive-Threaded Algorithm
A simple non-threaded approach, or serial algorithm, is to transpose the given matrix with a simple
nested loop by swapping the elements Ahi; ji with Ahj; ii. Call this method the Basic-Algorithm.
A naive OpenMP threading will instrument the serial algorithm by inserting #PRAGMA before
for loops, etc., to automatically parallelize the code if compiled as an OpenMP application.

# Diagonal-Threading
In this algorithm, threads are generated for each diagonal elements. For each diagonal position,
the corresponding row elements to the right and the column elements below are interchanged by the threads assigned to the position.

## CLONING AND RUNNING
Clone the repository using `git clone repository https://github.com/PersieB/Parallel-and-Distributed-Computing.git` in a folder

# Repository Branches
The github repository has 2 branches, namely "master" and "documentation".

# Master branch
The master branch contains the Lab 2 (or any lab) folder that should be cloned to access the source codes. 
NB: A README.md is also attached in each lab folder.

# Documentation branch
The documentation branch contains the report for each lab.

# Accessing source code
Since the main branch is the default, you can simply to the Lab folder in your unix environment.

# Accessing report from documentation branch
Simply use `git checkout documentation` to move from the master to the documentation branch in your terminal.
The folder will be updated and show only the reports. To switch back to master branch, use the `git checkout master`.
The folder will be updated to reflect only files in the master branch i.e lab folders containing source codes.

# HEAR FROM YOU
Kindly feel free to reach out with any feedback. Thank you!




