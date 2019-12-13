#!/usr/bin/env python3

import numpy as np
import pymc3 as pm
 
with pm.Model() as model:
    mu = pm.Uniform('mu',lower=-2,upper=2,transform='interval')
