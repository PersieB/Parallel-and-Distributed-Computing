"""
Author: Percy Brown
Date: 20th April 2021
Topic: Block-partioning of matrices
"""


"""
Partitions a given matrix into row-major order
Parameters:
        matrix : an n x n matrix
        p (int): sqrt(P) == number of processors
        offset (int): number of blocks=N/sqrt(p)
Yield: Array partioned in row-major order
"""
def row_order(matrix,p,offset):
    i=0
    while i < p:
        start = i * offset
        end = (i*offset) + offset
        yield matrix[start:end]
        i+=1

"""
Partitions a given matrix into column-major order
Parameters:
        matrix : an n x n matrix
        p (int): sqrt(P) == number of processors
        offset (int): number of blocks=N/sqrt(p)
Yield: Array partioned in column-major order
"""
def column_order(matrix, p, offset):
    i = 0
    while i < p:
        array = []
        start = i * offset
        end = (i*offset) + offset

        for j in range(len(matrix)):
            array += matrix[j][start:end]

        yield reorient_column(array,offset)
        i+=1

"""
Reorients a given column
Parameters:
    column : block of numbers
    offset : integer displacements
Returns:
    list: reoriented column
"""
def reorient_column(column,offset):
    col =[[] for i in range(offset)]

    for i in range(len(column)):
        col[i%offset].append(column[i])
    return col


"""
Performs local block multiplication
Parameters:
        row: partitioned
        column: partitioned
Returns:
        value: results
"""
def compute_value(row,column):
    value = 0
    for count in range(len(row)):
        value += (row[count]*column[count])
    return value