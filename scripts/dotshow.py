#!/usr/bin/env python3
import sys

if len(sys.argv) > 1:  we = open(sys.argv[1])
else:                  we = sys.stdin

we.readline()
we.readline()  
print('digraph {')
for l in we:
  l = l.strip()
  if len(l) == 0: continue
  l = l.split()
  print('  {} -> {} [label="{}"]'.format(*l))
print('}')
