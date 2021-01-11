"""
Libary Containg all of the graph plotting functions.
"""
import numpy as np
import matplotlib.pyplot as plt
import random as r

def CrudeAverage(InputArray,Amount):
    Length = (len(InputArray)//Amount) * Amount
    n = np.reshape(InputArray[0:Length] ,(-1 , Amount))
    return np.average(n, axis = 1)


def GenerateTaskLog(Length):
    PossibleColours = ["tab:red","tab:green","tab:blue","tab:orange"]
    PreviousTask = 0
    OutputPos = []
    OutPutTasks = []
    while Length > 0:
        a = r.randint(1,5)
        task = r.randint(0,10)
        if task >= 4:
            task = PreviousTask
        PreviousTask = task
        Start = Length - a
        if Start < 0:
            Start = 0
        OutputPos.append([Start , a])
        Length = Start
        OutPutTasks.append(task)
    OutPutTasks = list(map(lambda x: PossibleColours[x],OutPutTasks ))
    return OutputPos , OutPutTasks

def GenerateSecondGraph(figure,TempratureData , taskList,TaskColours):
    Time = np.array([i for i in range(len(TempratureData))])
    ATemp = CrudeAverage(TempratureData , 20)
    ATime = CrudeAverage(Time , 20)
    plot = figure.add_subplot(2,1,1)
    plot.broken_barh(taskList ,(1,10), facecolors = TaskColours) 
    plot = figure.add_subplot(2,1,2)
    plot.plot(Time,TempratureData)
    plot.plot(ATime , ATemp , 'r')

def GenExample(plot):
    r.seed(6)
    TaskLogs = GenerateTaskLog(50)
    Temprature = np.genfromtxt('../Raw/0TrueTemp.csv',delimiter=',')
    GenerateSecondGraph(plot,Temprature , *TaskLogs)

if __name__ == "__main__":
    GenExample(plt)