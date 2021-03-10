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
Clone the repository using "git clone repository https://github.com/PersieB/Parallel-and-Distributed-Computing.git"
Navigate to the Lab2 folder in your unix environment.
To compile the program type:
gcc -o Transpose Transpose.c -fopenmp -lgomp

To run, type:
./Transpose

