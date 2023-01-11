
import random 
import math
import sys


n = int(sys.argv[1])
x = int(sys.argv[2])

# n random floats 
rand_n = [ random.random() for i in range(n) ]
# extend the floats so the sum is approximately x (might be up to 3 less, because of flooring) 
result = [ math.floor(i * x / sum(rand_n)) for i in rand_n ] 
# randomly add missing numbers 
for i in range(x - sum(result)): 
    result[random.randint(0,3)] += 1
print(result)
# print("The sum is %d" % sum(result))
