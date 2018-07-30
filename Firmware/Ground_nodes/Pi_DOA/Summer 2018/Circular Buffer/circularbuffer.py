#!/usr/bin/python
import sys
import threading
import numpy
import time

class RingBuffer:
    """ class that implements a not-yet-full buffer """
    def __init__(self,size_max):
        self.max = size_max
        self.data = numpy.zeros(size_max)
	self.cur = 0

    def append(self, x):
        """ Append an element overwriting the oldest one. """
        self.data[self.cur] = x
        self.cur = (self.cur+1) % self.max
  
    def get(self, file):
        """ return list of elements in correct order """
	if self.cur == (self.max - 1):
            print str(self.data[self.cur:]+self.data[:self.cur])

    def run(self, stream, file):
    	while True:
	    time.sleep(0.1)
    	    x.append(stream)
	    x.get(file)

if __name__=='__main__':
	print "hello"
	file = open("test.dat", "w")
    	x=RingBuffer(10)
	x.run(sys.stdin.read(), file)