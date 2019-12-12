import pandas as pd
import os

PI = []
path = r'C:/Users/Rancho/Desktop/6132 agent based/anasazi/'
historicalpeople=pd.read_csv('target_data.csv',header = None,usecols=[1])
#print(historicalpeople)
def getfile():     #get csvfiles list
    files = os.listdir(path)
    files.sort()
    list = []
    for file in files:
        if not os.path.isdir(path + file):
            filename = str(file)
            list.append(filename)
    return list

list = getfile()

for i in range(0,8):   #calculate sse of each result
    csvdata = []
    rdata = pd.read_csv(list[i],usecols=[1])
#print(rdata)
#rdata = pd.read_csv('NumberOfHousehold.csv',header = 0,usecols=[1])
#print(rdata)
    sse = historicalpeople.iloc[:, 0].sum() - rdata.iloc[:, 0].sum()
    PI.append(sse)
idx = PI.index(min(PI))
PImin = PI[idx]
print('Best PI is',PI[idx])
print('the number of best PI is',idx)




