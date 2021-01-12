import requests
from math import log
Ip = "198.168.1.5"
Port = "80"

def GetCurrentTask():
    r = requests.get(UriCreator(Task))
    if r.status_code != 200:
        return False
    else:
        return int(r.content)

def ClearInfo(Uri):
    r = requests.delete(UriCreator(Task))
    if r.status_code != 200:
        return False
    else:
        return True

def ParserForHex(data):
    """
    Data comes in formated as HexTemp,HexLight,Task
    """
    data = data[:-1] #removes the fun comma.
    a = []
    extraCheaky = "a = [" + str(data) + "]" #this is illegally cheaky do not copy this line.
    extraCheaky.eval() #as the file is very similar to a python list , it treats it as a python list
    #this is also a massive security risk , but it is so much fun that I have left it in.
    Temprature , Light , Task  = [] ,[] , []
    for Row in a:
        Te , L , Ta = Row
        Te = int(Te , 16)
        L = int(L , 16)
        Light.append(L)
        Temprature.append(Te)
        Task.append(Ta)
    return Temprature , Light , Task


def TrueTempCalc(Input):
    Beta = 3977
    Temprature0 = 298
    AbsoluteZero = 273
    TheOtherResistance = 9000
    R0 = 10000
    MainResistance = ((Input/4096) * TheOtherResistance) /((Input/4096) - 1)
    Output = (Temprature0*Beta) / (Temprature0 * log(MainResistance/R0) + Beta)
    return Output


def UriCreator(Uri):
    return Ip + ":" + Port + "/" Uri