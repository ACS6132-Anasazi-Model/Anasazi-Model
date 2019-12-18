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
from six.moves import input


def simulator(mu, sigma, batch_size=1, random_state=None):
    mu, sigma = np.atleast_1d(mu, sigma)
    return scipy.stats.norm.rvs(mu[:, None], sigma[:, None], size=(batch_size, 100), random_state=random_state)

def mean(y):
    return np.mean(y, axis=1)

def var(y):
    return np.var(y, axis=1)

print("Input 1 for calibration of the first 50 years of the actual model")
print("Input 2 for calibration of a simple gaussian model")
UserInput= input('Type the input here: ')
# print(UserInput)
# UserInput = (UserInput.split())[0]
UserInput = int(UserInput)

if UserInput == 1:
    print("Running calibration of the first 50 years of the actual model")
    os.system("./ELFI_test.py")
elif UserInput == 2:
    print("Running calibration of a simple gaussian model")
    true_mu = float(input("Select mean, mu for the model: "))
    true_sigma = float(input("Select standard deviation, sigma for the model: "))

    true_model_output = simulator(true_mu,true_sigma)

    mu = elfi.Prior('uniform', true_mu-5, 5)
    sigma = elfi.Prior('uniform', true_sigma-3, 3)

    # Add the simulator node and observed data to the model
    sim = elfi.Simulator(simulator, mu, sigma, observed=true_model_output)

    # Add summary statistics to the model
    S1 = elfi.Summary(mean, sim)
    S2 = elfi.Summary(var, sim)

    # Specify distance as euclidean between summary vectors (S1, S2) from simulated and
    # observed data
    d = elfi.Distance('euclidean', S1, S2)

    elfi.set_client('multiprocessing')
    elfi.set_client(elfi.clients.multiprocessing.Client(num_processes=3))

    # Plot the complete model (requires graphviz)
    elfi.draw(d)

    N = 50

    smc = elfi.SMC(d,batch_size=10000)

    schedule = [5,1,.01]
    result_smc = smc.sample(N, schedule)

    print("True mu = {:f}, True sigma = {:f}".format(true_mu,true_sigma))
    result_smc.sample_means_summary(all=True)
    result_smc.plot_pairs(all=True,figsize=(8, 2), fontsize=12)
    result_smc.save("results/result_smc.json")
    plt.show()
