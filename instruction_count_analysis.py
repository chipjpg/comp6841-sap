from idautils import *
from idaapi import *
from idc import *
import re

instruction_counts = {}

for segea in Segments():
    for funcea in Functions(segea, SegEnd(segea)):
        functionName = GetFunctionName(funcea)
        for (startea, endea) in Chunks(funcea):
            for head in Heads(startea, endea):
                #print functionName, ":", "0x%08x"%(head), ":", GetDisasm(head)
                
                match = re.search("^[a-z]+( [a-z]+)?" ,GetDisasm(head))
                if match:
                    if match.group() in instruction_counts:
                        instruction_counts[match.group()] += 1
                    else:
                        instruction_counts[match.group()] = 1

d_view = [ (v,k) for k,v in instruction_counts.iteritems() ]
d_view.sort(reverse=True)
for v,k in d_view:
    print "%s: %d" % (k,v)
