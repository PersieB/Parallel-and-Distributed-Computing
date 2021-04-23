"""
Author: Percy Brown
Date: 20th April 2021
Topic: The classic MapReduce job (MrJob): count the frequency of words.
"""


from mrjob.job import MRJob
import re

WORD_RE = re.compile(r"[\w']+")


class MRWordFreqCount(MRJob):
    """
    defines the mapper for a one-step job.
    Parameters:	
    key – A value parsed from input.
    value – A value parsed from input.
    """

    def mapper(self, _, line):
            # yield each word in the line
        for word in WORD_RE.findall(line):
            yield (word.lower(), 1)

            
    """
    define the combiner for a one-step job.
    Parameters:	
    key – A key which was yielded by the mapper
    value – A generator which yields all values yielded by one mapper task/node which correspond to key.

    """
    def combiner(self, word, counts):
        # sum the words we've seen so far
        yield (word, sum(counts))


    """
    define the reducer for a one-step job.
    Parameters:	
        key – A key which was yielded by the mapper
        value – A generator which yields all values yielded by the mapper which correspond to key.
    """
    def reducer(self, word, counts):
        yield (word, sum(counts))


if __name__ == '__main__':
     MRWordFreqCount.run()