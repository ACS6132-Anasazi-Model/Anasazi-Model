#!/usr/bin/env python3

import matplotlib.pyplot as plt
import csv
import numpy as np


def autocov(x,hist_data):
    x = np.array(x)
    C = np.sqrt(np.mean((x-hist_data)**2))
    print(C)
    return C

x = []
y = []
line_num = 0
with open('NumberOfHousehold.csv','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
    	line_num += 1
    	if (line_num != 1):
        	x.append(int(row[0]))
        	y.append(int(row[1]))

simulation_years = len(x)
Sim = y
plt.plot(x,y, label='Simulation')
x = []
y = []
line_num = 0
with open('target_data.csv','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        line_num += 1
        if (line_num < simulation_years+1):
        	x.append(int(row[0]))
        	y.append(int(row[1]))
hist = y
autocov(Sim,hist)
plt.plot(x,y, label='Historical')
plt.xlabel('Year')
plt.ylabel('Number of Households')
plt.title('Anasazi model')
plt.legend()
plt.show()
