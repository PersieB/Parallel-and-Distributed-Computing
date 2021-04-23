"""
Author: Percy Brown
Date: 20th April 2021
Topic: Generating matrices
"""


"""
Paramater: size - with the assumption that the matrices under consideration are square
Fills in initialised matrix with appropriate values, based on given size.
Returns the matrix
"""
def create_matrix(size):
    # initialize matrix with zeros
    matrix = [[0 for i in range(int(size))] for j in range(int(size))]
    value = 1
    for i in range(size):
        for j in range(size):
            matrix[i][j] = value
            value+=1

    matrix_file = open("matrix.txt", 'w+')
    with open('matrix.txt', 'w') as testfile:
        for row in matrix:
            testfile.write('\t'.join([str(a) for a in row]) + '\n')
    matrix_file.close()

    return matrix
