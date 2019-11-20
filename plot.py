import matplotlib.pyplot as plt
import csv

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
        	
plt.plot(x,y, label='Population')
plt.xlabel('Year')
plt.ylabel('Number of Households')
plt.title('Anasazi model')
plt.legend()
plt.show()