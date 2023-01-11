
import random 
import math
import sys


n = int(sys.argv[1])
# n random floats 
rand_n = [ random.random() % 15 for i in range(n) ]

print(rand_n)
# print("The sum is %d" % sum(result))
