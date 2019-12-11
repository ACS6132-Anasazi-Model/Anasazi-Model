import csv

#def readData1():
#    file = open("target_data.csv","r")
#    data = file.read()
#    print(data)

def readData():
    data = []
    
    file = open("target_data.csv","r")
    csv_read = csv.reader(file, delimiter=',')
    
    for row in csv_read:
        data.append(row)
        
    #print(data[0][0])


def readOutput():
    output = []
    
    file = open("NumberOfHousehold.csv","r")
    csv_read = csv.reader(file, delimiter=',')
    
    file.readline()
    
    for row in csv_read:
        output.append(row)

    #print(output[0][0])


def writeParameterFile(start,end,storeyear,storage,need,minfission,maxfission,age,distance,mincorn,maxcorn,annual,spatial,fertility,harvest,maize):
    file = open("parameters.txt","w+")

    file.write("#Properties file\n\nrandom.seed = 1\ncount.of.agents = 1\nboard.size.x = 80\nboard.size.y = 120\nproc.per.x = 1\nproc.per.y = 1\ngrid.buffer = 0\n\n")
    
    file.write("start.year = " + str(start) + "\n")
    file.write("end.year = " + str(end) + "\n")
    file.write("max.store.year = " + str(storeyear) + "\n")
    file.write("max.storage = " + str(storage) + "\n")
    file.write("household.need = " + str(need) + "\n")
    file.write("min.fission.age = " + str(minfission) + "\n")
    file.write("max.fission.age = " + str(maxfission) + "\n")
    file.write("max.age = " + str(age) + "\n")
    file.write("max.distance = " + str(distance) + "\n")
    file.write("initial.min.corn = " + str(mincorn) + "\n")
    file.write("initial.max.corn = " + str(maxcorn) + "\n")
    file.write("annual.variance = " + str(annual) + "\n")
    file.write("spatial.variance = " + str(spatial) + "\n")
    file.write("fertility.prop = " + str(fertility) + "\n")
    file.write("harvest.adj = " + str(harvest) + "\n")
    file.write("new.household.ini.maize = " + str(maize) + "\n")
    

#readData()
#writeParameterFile(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16)
#readOutput()
