'''
Created on 6 jul 2019

@author: ernestoalvarado
'''
import random
from math import gcd,sqrt
# XXX: https://djangocentral.com/python-program-to-check-if-a-number-is-perfect-square/
def square(n):
	return int(sqrt(n) + 0.5) ** 2 == n
if __name__ == '__main__':
	i=0
	N=600
	s=2
	maxn=int(1E9)
	f=open("in{:02d}.txt".format(s+i),'w')
	f.write("{}\n".format(N))
	while i<N:
		# XXX: https://stackoverflow.com/questions/3996904/generate-random-integers-between-0-and-9
		x=random.randint(1,maxn+1)
		z=random.randint(1,maxn+1)
		k=random.randint(1,maxn+1)
		f.write("{} {} {}\n".format(x,z,k))
		i+=1
	f.close()
