# -*- coding: utf-8 -*-
#  The MIT License (MIT)
#  Copyright (c) 2016 Pinaky Bhattacharyya
#  [See LICENSE.txt in project root folder for more details.]

"""
Created on Sat Jul  9 02:02:59 2016

@author: Pinaky
"""

import json
import matplotlib.pyplot as plt
import numpy as np

filepath = 'outfile.json'

with open(filepath) as file:
	data = json.load(file)

# You can run up to here and play with 'data' in IPython

nstages = len(data['stages'])

fig = plt.figure()
bb = np.zeros(nstages)
ll = np.zeros(nstages)

for i in range(nstages):
	print('i={0}'.format(i))
	plt.cla()
	bb[i] = data['stages'][i]['beta']
	ll[i] = np.mean(np.array(data['stages'][i]['loglike']))
	x = np.array(data['stages'][i]['x'])
	plt.plot(x[:,0], x[:,1], 'ob')
	plt.xlabel('Mean parameter')
	plt.ylabel('Log of standard deviation parameter')
	plt.title('beta = {0:.4f}'.format(bb[i]))
	plt.xlim([-5.0, 5.0])
	plt.ylim([-5.0, 5.0])
	plt.savefig('gauss{0}.png'.format(str(i).zfill(2)))

pmean = np.mean(data['stages'][-1]['x'], axis=0)
logevd = np.trapz(ll, x=bb)
print('Posterior mean = ')
print(pmean)
print('Log evidence = {0}'.format(logevd))

