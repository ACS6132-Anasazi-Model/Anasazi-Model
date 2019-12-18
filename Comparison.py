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

def autocov(x,hist_data):
    x = np.array(x)
    if(len(x) == len(hist_data)):
        C = np.sqrt(np.mean((x-hist_data)**2))
        #print(C)
        return C
    else:
        return 999999

def readOutput(S,hist_data):
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
    if (autocov(y,hist_data) < 10):
        print(autocov(y,hist_data))
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
        readOutput('result{:d}.csv'.format(i),hist_data)
        i+=1
    else:
        OkToLoop = False


plt.xlabel('Year')
plt.ylabel('Number of Households')
plt.title('Anasazi model')
plt.legend()
plt.show()
