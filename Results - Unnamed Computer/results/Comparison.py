#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
import scipy.stats
import elfi
import os
from datetime import datetime
import csv
import shutil
from subprocess import *
import multiprocessing

global start
start = 800
global end
end = 1250

#global count
#count = 0

global best_50
best_50 = []

def autocov(x,hist_data):
    x = np.array(x)
    if(len(x) == len(hist_data)):
        C = np.sqrt(np.mean((x-hist_data)**2))
        #print(C)
        return C
    else:
        return 999999

def readProps(Path):
    data = []

    file = open(Path, "r")
    #data = file.readlines()
    for row in file:
        row = row.replace("\n","")
        row = row.split("=")
        #print(row)
        if len(row) > 1:
            data.append(row[1])
    file.close()
    return data


def readOutput(S,hist_data,i):
    #global count
    x = []
    y = []
    line_num = 0
    with open(S,'r') as csvfile:
        plots = csv.reader(csvfile, delimiter=',')
        for row in plots:
            line_num += 1
            if (line_num != 1):
                x.append(int(row[0]))
                y.append(int(row[1]))
    if (autocov(y,hist_data) < 35.2):
        print(autocov(y,hist_data))
        #print(S)
        #count += 1
        best_50.append(i)
        plt.plot(x,y, label=S)

def readData():
    data1 = []
    data2 = []
    file = open("target_data.csv","r")
    csv_read = csv.reader(file, delimiter=',')

    for row in csv_read:
        data2.append(int(row[0]))
        data1.append(int(row[1]))

    file.close()
    plt.plot(data2,data1, label='Historical')
    #print(data[0:6])
    return data1[0:end-start+1]
    #print(data[0][0])

hist_data = readData()
OkToLoop = True
i=1
while(OkToLoop):
    if os.path.isfile('result{:d}.csv'.format(i)):
        readOutput('result{:d}.csv'.format(i),hist_data,i)
        i+=1
    else:
        OkToLoop = False

#print(count)
plt.xlabel('Year')
plt.ylabel('Number of Households')
plt.title('Anasazi model')
plt.legend()
plt.show()


BASE_DIR = os.path.dirname(os.getcwd())
minfission = []
maxfission = []
minDeathAge = []
maxDeathAge = []
fertility = []
harvest = []

for i in best_50:
    data = readProps('{:s}/prop/model{:d}.props'.format(BASE_DIR,i))
    minfission.append(int(data[12]))
    maxfission.append(int(data[13]))
    minDeathAge.append(int(data[14]))
    maxDeathAge.append(int(data[15]))
    fertility.append(float(data[21]))
    harvest.append(float(data[22]))

minfission = np.array(minfission)
maxfission = np.array(maxfission)
minDeathAge = np.array(minDeathAge)
maxDeathAge = np.array(maxDeathAge)
fertility = np.array(fertility)
harvest = np.array(harvest)

print("Min. Fission Age: {:f}, Max. Fission Age: {:f}, Min. Death Age: {:f}, Max. Death Age: {:f}, Fertility: {:f} and harvest: {:f}".format(np.mean(minfission),np.mean(maxfission),np.mean(minDeathAge),np.mean(maxDeathAge),np.mean(fertility),np.mean(harvest)))


plt.subplot(1,6, 1)
plt.hist(minfission, bins=7)
plt.title('Min. Fission Age')
plt.xlabel('Age')

plt.subplot(1,6, 2)
plt.hist(maxfission, bins=9)
plt.title('Max. Fission Age')
plt.xlabel('Age')

plt.subplot(1,6, 3)
plt.hist(minDeathAge, bins=7)
plt.title('Min. Death Age')
plt.xlabel('Age')

plt.subplot(1,6, 4)
plt.hist(maxDeathAge, bins=9)
plt.title('Max. Death Age')
plt.xlabel('Age')

plt.subplot(1,6, 5)
plt.hist(fertility, bins="auto")
plt.title('Fertility')
plt.xlabel('Probability of fission')

plt.subplot(1,6, 6)
plt.hist(harvest, bins="auto")
plt.title('Harvest')
plt.xlabel('Harvest Adjust rate')
plt.show()
