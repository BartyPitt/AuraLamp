import tkinter as tk
import ApiInteractions as Api
from tkinter import ttk
from matplotlib.backends.backend_tkagg import (
    FigureCanvasTkAgg, NavigationToolbar2Tk)
# Implement the default Matplotlib key bindings.
from matplotlib.backend_bases import key_press_handler
from matplotlib.figure import Figure
from GraphPlotters import GenExample
import numpy as np

import ApiInteractions as api





ExampleTestData = [3,4,5,6]
ExampleTargets = [10,10,20,20]

LARGE_FONT= ("Verdana", 24)
SMALL_FONT= ("Verdana", 12)

class MainWindow(tk.Tk):
    def __init__(self ,*args , **kwargs):
            self.TargetBarValues = [40,40,40,40]
            self.BarsList = []
            self.BarEntries = []

            tk.Tk.__init__(self, *args, **kwargs)
            MainContainer = tk.Frame(self)
            MainContainer.pack(side = "top" , fill = "both" , expand = True)
            MainContainer.grid_rowconfigure(0, weight=1)
            MainContainer.grid_columnconfigure(0, weight=1)

            self.Screens = []
            self.title("Aura Productivity APP")

            self.Screens.append(tk.Frame(MainContainer))
            MainFrame = self.Screens[0]
            MainFrame.grid(row=0, column=0, sticky="nsew")
            self.GenerateScreenOne(MainFrame)
            self.UpdateConection()





    def ShowFrame(self,PageNumber):
        frame = self.Screens[PageNumber]
        frame.tkraise()
        


    def GenerateScreenOne(self,MainFrame):
        MainFrame.grid()
        RightSubframe = tk.Frame(MainFrame)
        LeftSubframe = tk.Frame(MainFrame)
        BottomSubFrame= tk.Frame(MainFrame)
        BaseSection = tk.Frame(MainFrame)


        RightSubframe.grid(row = 0 , column = 1)
        LeftSubframe.grid(row = 0 , column = 0)
        BottomSubFrame.grid(row = 1 ,columnspan=2 )
        BaseSection.grid(row = 2, columnspan = 2)
        self.GenerateProgressBars2(RightSubframe)

        self.PlotPieChart(LeftSubframe,ExampleTestData)
        self.GenerateBottomGraphs(BottomSubFrame)
        self.GenerateStatus(BaseSection)
        return MainFrame

    def GenerateProgressBars2(self,frame):
        self.tasklabel = tk.Label(master = frame ,text = "Current Task = 0" , font = LARGE_FONT)
        self.tasklabel.grid(row = 0)
        self.GenerateProgressBars(frame , "green",0)
        self.GenerateProgressBars(frame, "orange",1)
        self.GenerateProgressBars(frame, "red",2)
        self.GenerateProgressBars(frame, "blue",3)
        tk.Button(frame , text = "Update",command = self.UpdateProgressBars).grid(row = 10)


    def GenerateBottomGraphs(self , frame):
        Title = ttk.Label(frame, text='Today , Temperature and Tasks worked on ' , font = LARGE_FONT)
        Title.pack()
        fig = Figure(figsize=(10, 2), dpi=100)
        GenExample(fig)
        canvas = FigureCanvasTkAgg(fig, master=frame)  # A tk.DrawingArea.
        canvas.draw()
        canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=1)


    def GenerateStatus(self, frame):
        frame.grid()
        BottomText = tk.Label(frame , text = "Current Status :", font = LARGE_FONT)
        self.Status = tk.Label(frame,text = "Disconnected" , font = LARGE_FONT , foreground="red")
        BottomText.grid(row = 0)
        self.Status.grid(row = 1)



    def PlotPieChart(self,frame,Data):
        Title = ttk.Label(frame, text='Weekly Task Distribution' , font = LARGE_FONT)
        Title.pack()

        fig = Figure(figsize=(5, 3), dpi=100)
        plot =fig.add_subplot(111)
        plot.pie(ExampleTestData)

        canvas = FigureCanvasTkAgg(fig, master=frame)  # A tk.DrawingArea.
        canvas.draw()
        canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=1)


    def GenerateProgressBars(self,frame,colour,number):
        s= ttk.Style()
        colourName = colour + ".Horizontal.TProgressbar"
        s.theme_use('clam')
        s.configure(colourName, foreground=colour, background=colour)
        Label1 = ttk.Label(frame, text='Task' + str(number) , font = SMALL_FONT)
        Label1.grid(row = 2 * number + 1)
        Bar = ttk.Progressbar(frame, style=colourName, orient="horizontal", length=400, mode="determinate", maximum=self.TargetBarValues[number], value=15)
        Bar.grid(row = 2*number + 2, pady = 10)
        self.BarsList.append(Bar)

        ent = tk.Entry(frame)
        ent.grid(row = 2*number + 1 , column = 1)
        ent.insert(0 , "40")
        self.BarEntries.append(ent)

    def UpdateConection(self):
        Responce = Api.GetCurrentTask()
        if Responce:
            self.tasklabel["text"] = Responce
            self.Status["text"] = "Connected"
            self.Status.configure(fg ="green")
        else:
            self.Status["text"] = "Disconnected"
            self.Status.configure(fg ="red")

        self.after(5000 , self.UpdateConection)
        return
        


    def UpdateProgressBars(self):
        print(self.BarEntries)
        for i,Bar in enumerate(self.BarsList):
            Bar["maximum"] = int(self.BarEntries[i].get())


app = MainWindow()
app.mainloop()
