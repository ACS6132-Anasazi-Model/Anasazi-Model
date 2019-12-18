def readParameters():
    data = []
    
    file = open("parameters.txt", "r")
    #data = file.readlines()
    for row in file:
        row = row.replace("\n","")
        row = row.split("=")
        #print(row)
        if len(row) > 1:
            data.append(row[1])


    print(data)

readParameters()
