#Tamer Rafidi - rafidit - 400333527

from cmath import cos, sin
from os import truncate
from turtle import color
import numpy
import math
import serial
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

step = 0                                                                           #Initializes all variables
line_x=0
line_y=0
line_z=0
z = 0

delete = input("Delete saved graph data? (yes or no)")
if(delete == 'yes'):
    f = open('data_graph.txt','w')                                               #opens an I/O file in write mode
    truncate                                                                     #deletes all data in the file if user chooses to delete the file
    f.close()                                                                    #Closes the I/O file once it deletes everything

file = serial.Serial('COM4', baudrate = 115200, timeout = 4)                       #Connects the microcontroller to the computer so that the program can receive data
print("Opening: " + file.name)                                                       

file.reset_output_buffer()
file.reset_input_buffer()

fig = plt.figure(figsize=(11,11))                                                 #Initializes plot size
graph = fig.add_subplot(111, projection='3d')                                         
graph.set_xlabel('x')                                                              #Labels x label "x"
graph.set_ylabel('y')                                                              #Labels y label "y"
graph.set_zlabel('z')                                                              #Labels z label "z"

file.write('s'.encode())                                                           #Begins to encode the data into the file I/O
  
x = [0]*64                                                                      #array to complete one rotation
f = open('data_graph.txt','a')                                                  #open data_graph.txt in append mode
while (step != 64):                                                             #While loop that iterates until one full rotation is complete
    x[step] = float(file.readline().decode())                                   #decode the data and put it in a list
    print(x[step], step+1)                                                      #Prints data point as it is received
    f.write(str(x[step]))                                                       #turn data to string and put in txt file
    f.write('\n')                                                               #moves the next line
    step+=1                                                                     #Increments step by 1 each iteration
f.close()

data = open("data_graph.txt","r")                                                #open text file in read only mode
data_Str = data.read()                                                           #all of txt file put in string
data_table = data_Str.split("\n")                                                #Adds a new line after each data point and stores it in data_table
for i in range(0,len(data_table),1):                                              
    data_table[i] = str(data_table[i])                                           #put into list
data.close()                                                                        
data_table.pop(len(data_table)-1)                                                

Data = [float(i) for i in data_table]                                            #turned into type float
measure_num = len(Data)                                                          #number of measurements is stored in measure_num

plot = input("Do you want to plot the data?(yes or no)")                         #requires input from user to see if the user wants to plot the data 
if(plot == 'yes'):

    step = 0
    while(step != measure_num):                                                   #While loop that iterates if theres data points to graph 

        if(step%64==0 and step != 0): z+=0.1                        #If statement to check if steps is a multiple of 64, increments z by 0.1 to create depth after each iteration
        
        line_x = [(Data[step-1])*cos((step-1)*0.09817477),(Data[step])*cos(step*0.09817477)]    
        line_y = [(Data[step-1])*sin((step-1)*0.09817477),(Data[step])*sin(step*0.09817477)]
        if(step < 64): line_z = [0,0]
        else: line_z = [z,z] 
                                                                               
        graph.scatter( (Data[step])*cos(step*0.09817477) ,(Data[step])*sin(step*0.09817477),z, c = "black", s = 1)  #plot points in cartesian

        if(step<64): graph.plot(line_x ,line_y,0, color = 'black')                         
        else: graph.plot(line_x ,line_y,line_z, color = 'black')
        step+=1

    step = 0
    while(step < measure_num-64):                                                              #While loop to iterate through 10 loops 
        if(step<64):
            z = 0
            line_x = [(Data[step])*cos((step)*0.09817477),(Data[step+64])*cos(step*0.09817477)] #creates an x-component from the data measurement achieved
            line_y = [(Data[step])*sin((step)*0.09817477),(Data[step+64])*sin(step*0.09817477)] #creates an y-component from the data measurement achieved
            line_z = [z,z+0.1]                                                                  #creates depth in the graph 
            graph.plot(line_x ,line_y,line_z, color = 'black')
        if(64<=step<128):
            z = 0.1
            line_x = [(Data[step])*cos((step)*0.09817477),(Data[step+64])*cos(step*0.09817477)]
            line_y = [(Data[step])*sin((step)*0.09817477),(Data[step+64])*sin(step*0.09817477)]
            line_z = [z,z+0.1]
            graph.plot(line_x ,line_y,line_z, color = 'black')
        if(128<=step<192):
            z = 0.2
            line_x = [(Data[step])*cos((step)*0.09817477),(Data[step+64])*cos(step*0.09817477)]
            line_y = [(Data[step])*sin((step)*0.09817477),(Data[step+64])*sin(step*0.09817477)]
            line_z = [z,z+0.1]
            graph.plot(line_x ,line_y,line_z, color = 'black')
        if(192<=step<256):
            z = 0.3
            line_x = [(Data[step])*cos((step)*0.09817477),(Data[step+64])*cos(step*0.09817477)]
            line_y = [(Data[step])*sin((step)*0.09817477),(Data[step+64])*sin(step*0.09817477)]
            line_z = [z,z+0.1]
            graph.plot(line_x ,line_y,line_z, color = 'black')
        if(256<=step<320):
            z = 0.4
            line_x = [(Data[step])*cos((step)*0.09817477),(Data[step+64])*cos(step*0.09817477)]
            line_y = [(Data[step])*sin((step)*0.09817477),(Data[step+64])*sin(step*0.09817477)]
            line_z = [z,z+0.1]
            graph.plot(line_x ,line_y,line_z, color = 'black')
        if(320<=step<384):
            z = 0.5
            line_x = [(Data[step])*cos((step)*0.09817477),(Data[step+64])*cos(step*0.09817477)]
            line_y = [(Data[step])*sin((step)*0.09817477),(Data[step+64])*sin(step*0.09817477)]
            line_z = [z,z+0.1]
            graph.plot(line_x ,line_y,line_z, color = 'black')
        if(384<=step<448):
            z = 0.6
            line_x = [(Data[step])*cos((step)*0.09817477),(Data[step+64])*cos(step*0.09817477)]
            line_y = [(Data[step])*sin((step)*0.09817477),(Data[step+64])*sin(step*0.09817477)]
            line_z = [z,z+0.1]
            graph.plot(line_x ,line_y,line_z, color = 'black')
        if(448<=step<512):
            z = 0.7
            line_x = [(Data[step])*cos((step)*0.09817477),(Data[step+64])*cos(step*0.09817477)]
            line_y = [(Data[step])*sin((step)*0.09817477),(Data[step+64])*sin(step*0.09817477)]
            line_z = [z,z+0.1]
            graph.plot(line_x ,line_y,line_z, color = 'black')
        if(512<=step<576):
            z = 0.8
            line_x = [(Data[step])*cos((step)*0.09817477),(Data[step+64])*cos(step*0.09817477)]
            line_y = [(Data[step])*sin((step)*0.09817477),(Data[step+64])*sin(step*0.09817477)]
            line_z = [z,z+0.1]
            graph.plot(line_x ,line_y,line_z, color = 'black')
        if(576<=step<640):
            z = 0.9
            line_x = [(Data[step])*cos((step)*0.09817477),(Data[step+64])*cos(step*0.09817477)]
            line_y = [(Data[step])*sin((step)*0.09817477),(Data[step+64])*sin(step*0.09817477)]
            line_z = [z,z+0.1]
            graph.plot(line_x ,line_y,line_z, color = 'black')
        step+=1
    plt.show()                                          #plots the graph and outputs it to the user

print("Closing: " + file.name )                            #close COM4
file.close()
