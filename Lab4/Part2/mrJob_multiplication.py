"""
Author: Percy Brown
Date: 20th April 2020.
Topic: Matrix multiplication using MapReduce Job (MrJob)
"""

import mrjob
from mrjob.job import MRJob
import math
import time
import partitions as part

class MrJobMultiply(MRJob):
    file3 = open('mrjob_output.txt', 'w+')

    #get dimensions of matrices
    file1 = open("matrix.txt","r+")
    line = file1.readline()
    size = sum(1 for line in open('matrix.txt')) #get matrix size N
    
    INPUT_PROTOCOL = mrjob.protocol.RawProtocol

    """
    configures command line arguments for the program.
    allows a user to specify the number of processors to utilise for block partitioning
    """
    def configure_args(self):
        super(MrJobMultiply, self).configure_args()
        self.add_passthru_arg(
            "--P", 
            help = "specify the number of processors. "
        )

    
    """
    Maps partions into key value pairs.
    Parameters:
            input_file
            input_uri
    Yields:
            [int]: multiplication of matrix given
    """
    def mapper_raw(self,input_file, input_uri):
        matrix=[]
        with open(input_file,'r') as x: 
            for line in x.readlines():
                row  = [int(index) for index in line.split()]
                matrix.append(row)

        offset = int((self.size/math.sqrt(int(self.options.P))))
        bands = math.sqrt(int(self.options.P))
        count = 1
        offset = int((self.size/math.sqrt(int(self.options.P))))
        bands = math.sqrt(int(self.options.P))

        for i in part.row_order(matrix,bands,offset):
            for j in part.column_order(matrix,bands,offset):
                yield str(count), i
                yield str(count), j

                count += 1
    
    """
    Aggregates the results from the mapper and yields local multiplication
    Parameters: key, values
    """
    def reducer(self, key, values):
        count = 0
        local_array = []
        value = []
        for i in values:
            local_array +=[i]
        for row in local_array[:1][0]:
            for col in local_array[1:][0]:
                value.append(part.compute_value(row,col))
        yield key, value
        self.file3.write(str(key) + " ")
        self.file3.write(str(value) + "\n")

def main():
    start = time.time()
    MrJobMultiply.run()
    end = time.time()
    print("Time taken for MapReduce: " + str(round(end - start, 4)) + " seconds.")
main()


