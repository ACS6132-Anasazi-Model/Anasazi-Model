#!/usr/bin/env python3

import os
import csv

os.system("mpirun bin/main.exe props/config.props props/model.props NumberOfHousehold.csv")
os.system("./plot.py")
