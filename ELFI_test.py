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

currentDT = datetime.now()
currentDT = currentDT.strftime("/%Y-%m-%d %H:%M:%S")
shutil.rmtree("results")
os.mkdir("results")
shutil.rmtree("prop")
os.mkdir("prop")
# elfi.set_client('multiprocessing')
iter = multiprocessing.Value('i', 1)
global start
start = 800
global end
end = 850

global threshold
threshold = 0
#def readData1():
#    file = open("target_data.csv","r")
#    data = file.read()
#    print(data)

def readData():
    data = []

    file = open("target_data.csv","r")
    csv_read = csv.reader(file, delimiter=',')

    for row in csv_read:
        data.append(int(row[1]))

    file.close()
    #print(data[0:6])
    return data[0:end-start+1]
    #print(data[0][0])


def readOutput(iteration):
    output = []
    file = open("results/result%s.csv" %(iteration),"r")
    csv_read = csv.reader(file, delimiter=',')

    file.readline()
    # try:
    for row in csv_read:
        output.append(int(row[1]))
    file.close()
    # except:
    #     file.close()
    #     with open("results/result%s.csv" %(iteration), 'rb') as mycsv:
    #         reader = csv.reader( (line.replace('\0','') for line in mycsv) )
    #     readOutput(iteration)

    if(len(output) != end-start+1):
        print(iteration)
        print(len(output))
        readOutput(iteration)

    #print(output)
    return output


def writeParameterFile(iteration,start,end,storeyear,storage,need,minfission,maxfission,minDeathAge,maxDeathAge,distance,mincorn,maxcorn,annual,spatial,fertility,harvest,maize):
    file = open("prop/model%s.props" %(iteration),"w+")

    file.write("#Properties file\n\n")
    file.write("random.seed = 1\n")
    file.write("count.of.agents = 14\nboard.size.x = 80\nboard.size.y = 120\nproc.per.x = 1\nproc.per.y = 1\ngrid.buffer = 0\n\n")

    file.write("start.year = {:d}\n".format(start))
    file.write("end.year = {:d}\n".format(end))
    file.write("max.store.year = {:d}\n".format(storeyear))
    file.write("max.storage = {:d}\n".format(storage))
    file.write("household.need = {:d}\n".format(need))
    file.write("min.fission.age = {:d}\n".format(int(minfission[0])))
    file.write("max.fission.age = {:d}\n".format(int(maxfission[0])))
    file.write("min.death.age = {:d}\n".format(int(minDeathAge[0])))
    file.write("max.death.age = {:d}\n".format(int(maxDeathAge[0])))
    file.write("max.distance = {:d}\n".format(distance))
    file.write("initial.min.corn = {:d}\n".format(mincorn))
    file.write("initial.max.corn = {:d}\n".format(maxcorn))
    file.write("annual.variance = {:0.5f}\n".format(annual))
    file.write("spatial.variance = {:0.5f}\n".format(spatial))
    file.write("fertility.prop = {:0.5f}\n".format(fertility[0]))
    file.write("harvest.adj = {:0.5f}\n".format(harvest[0]))
    file.write("new.household.ini.maize = {:0.5f}\n".format(maize))
    file.close()

#readData()
#writeParameterFile(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16)
#readOutput()

def anasazi_model(minDeathAge, maxDeathAge,fertility,harvest,minfission,maxfission,n_obs=551, batch_size=1, random_state=None):
    global iter
    with iter.get_lock():
        iteration = iter.value
        iter.value += 1

    global start
    global end
    global threshold
    storeyear = 2
    storage = 1600
    need = 800
    #minfission = 16
    #maxfission = 25
    distance = 1000
    mincorn = 1000
    maxcorn = 1600
    annual = 0.1
    spatial = 0.1
    #fertility = 0.125
    #harvest = 1
    maize = 0.33
    writeParameterFile(iteration,start,end,storeyear,storage,need,minfission,maxfission,minDeathAge,maxDeathAge,distance,mincorn,maxcorn,annual,spatial,fertility,harvest,maize)
    random_state = random_state or np.random

    w = random_state.randn(batch_size, n_obs+2)  # i.i.d. sequence ~ N(0,1)
    Simulation = check_output("mpirun bin/main.exe props/config.props prop/model%s.props results/result%s.csv" %(iteration,iteration),shell=True)
    #Simulation.wait()
    output = readOutput(iteration)
    #print(output)
    # if(smc.current_population_threshold != threshold):
    #     threshold = smc.current_population_threshold
    #     print("Population wth threshold {:d}\n".format(threshold))

    return output

def autocov(x,hist_data):
    x = np.array(x)
    C = np.sqrt(np.mean((x-hist_data)**2))
    #print(C)
    return C

minDeathAge = elfi.Prior('uniform',25,6)#32-39
maxDeathAge = elfi.Prior('uniform',32,8)
fertility = elfi.Prior('normal',0.125,0.03)
harvest = elfi.Prior('uniform',0.8,0.2)
minfission = elfi.Prior('uniform',15,6)
maxfission = elfi.Prior('uniform',22,8)

hist_data = readData()
Y = elfi.Simulator(anasazi_model, minDeathAge, maxDeathAge,fertility,harvest,minfission,maxfission, observed=hist_data)

S1 = elfi.Summary(autocov, Y,hist_data)
d = elfi.Distance('euclidean', S1)
elfi.draw(d)

elfi.set_client('multiprocessing')
# rej = elfi.Rejection(d,batch_size=1)

smc = elfi.SMC(d,batch_size=1)

N = 50

# vis = dict(xlim=[-2,2], ylim=[-1,1])

# You can give the sample method a `vis` keyword to see an animation how the prior transforms towards the
# posterior with a decreasing threshold.
schedule = [100,25,5]
result_smc = smc.sample(N, schedule)

# result.summary()
# result.plot_pairs();
# plt.show()
# # result.plot_marginals()
# plt.savefig('posterior.png')
result_smc.sample_means_summary(all=True)
result_smc.plot_pairs(all=True, figsize=(8, 2), fontsize=12)
result_smc.save("results/result_smc.json")
plt.savefig('results/population2.png')
plt.close()
plt.savefig('results/population1.png')
plt.close()
plt.savefig('results/population0.png')
plt.close()

path = os.getcwd() + currentDT
shutil.copytree("results", path + "/results")
shutil.rmtree("results")
os.mkdir("results")
shutil.copytree("prop", path + "/prop")
shutil.rmtree("prop")
os.mkdir("prop")
