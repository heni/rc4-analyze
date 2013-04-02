#!/usr/bin/env python2.7
import operator
import numpy as np

class StatTable(object):
    
    def __init__(self):
        self.Internal = np.zeros(255 * 256**2).reshape(255, 256**2)

    def mergeWith(self, idx, row):
        self.Internal[idx] += row

    def checkConsistency(self):
        smValues = sorted(map(sum, self.Internal))
        return smValues[0] == smValues[-1]

    def printToStream(self, pr):
        for i in xrange(len(self.Internal)):
            expected = np.average(self.Internal[i])
            outItems = self.Internal[i] / expected
            sigma = np.std(outItems)
            outIndices = outItems.argsort()
            for i in outIndices:
                if outItems[i] - 1.0 < -3 * sigma or outItems[i] - 1.0 > 3 * sigma:
                    pr.write("{0:3},{1:3}[{2:11.8f}] ".format(i / 256, i % 256, outItems[i]))
            print >>pr
            
if __name__ == "__main__":
    st = StatTable()
    for i, ln in enumerate(open("mr-bigrams.1m.log")):
        idx, items = ln.strip("\n").split("\t")
        st.mergeWith(int(idx), map(int, items.split()))
    assert st.checkConsistency()
    st.printToStream(open("rc4-bidistribution-1m.diff.log", "w"))
