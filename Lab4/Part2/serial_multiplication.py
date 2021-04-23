"""
Author: Percy Brown
Date: 20th April 2021
Topic: Matrix Multiplication in Serial Mode
"""

import numpy as np
import time
import create_matrices as mat
import sys

"""
Creates matrix A and B with values, and intialises matrix C with zeros with a given size
Executes serial matrix multiplications
Writes the resulting matrix C into a text file 'serial_result.txt'
"""
def matrix_multiplication(size):
    A = mat.create_matrix(int(size))
    B = mat.create_matrix(int(size))
    C = np.zeros((int(size), int(size)))

    for i in range(len(A)):
        for j in range(len(B[0])):
            for k in range(len(B)):
                C[i][j] += A[i][k] * B[k][j]
    result_file = open('serial_result.txt', 'w+')
    with open('serial_result.txt', 'w') as x:
        for i in C:
            x.write('\t'.join([str(int(a)) for a in i]) + '\n')

"""
The main function executes the "matrix_multiplication" method
Calculates the time taken to execute the serial matrix multiplication
"""
def main():
    start_time = time.time()
    args = sys.argv
    globals()[args[1]](*args[2:])
    end_time = time.time()
    print("Time taken for serial: " + str(round(end_time - start_time, 4)) + " seconds.")
main()

