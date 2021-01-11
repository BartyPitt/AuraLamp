import tkinter as tk
from tkinter import ttk
from matplotlib.backends.backend_tkagg import (
    FigureCanvasTkAgg, NavigationToolbar2Tk)
# Implement the default Matplotlib key bindings.
from matplotlib.backend_bases import key_press_handler
from matplotlib.figure import Figure
from GraphPlotters import GenExample
import numpy as np




ExampleTestData = [3,4,5,6]
ExampleTargets = [10,10,20,20]
ExampleDataForTemp = []

LARGE_FONT= ("Verdana", 24)
SMALL_FONT= ("Verdana", 12)

class MainWindow(tk.Tk):
    def __init__(self ,*args , **kwargs):
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





    def ShowFrame(self,PageNumber):
        frame = self.Screens[PageNumber]
        frame.tkraise()
        


    def GenerateScreenOne(self,MainFrame):
        MainFrame.grid()
        RightSubframe = tk.Frame(MainFrame)
        LeftSubframe = tk.Frame(MainFrame)
        BottomSubFrame= tk.Frame(MainFrame)

        RightSubframe.grid(row = 0 , column = 1)
        LeftSubframe.grid(row = 0 , column = 0)
        BottomSubFrame.grid(row = 1 ,columnspan=2 )
        self.GenerateProgressBars2(RightSubframe)

        self.PlotPieChart(LeftSubframe,ExampleTestData)
        self.GenerateBottomGraphs(BottomSubFrame)
        return MainFrame

    def GenerateProgressBars2(self,frame):

        self.GenerateProgressBars(frame , "green",0)
        self.GenerateProgressBars(frame, "yellow",1)
        self.GenerateProgressBars(frame, "red",2)
        self.GenerateProgressBars(frame, "blue",3)



    def GenerateBottomGraphs(self , frame):
        Title = ttk.Label(frame, text='Today , Temperature and Tasks worked on ' , font = LARGE_FONT)
        Title.pack()
        fig = Figure(figsize=(10, 2), dpi=100)
        GenExample(fig)
        canvas = FigureCanvasTkAgg(fig, master=frame)  # A tk.DrawingArea.
        canvas.draw()
        canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=1)


    def GenerateOptions(self, frame):
        return None

    def GenerateSecondGraph(self,frame):
        return None



    def PlotPieChart(self,frame,Data):
        Title = ttk.Label(frame, text='Weekly Task Distribution' , font = LARGE_FONT)
        Title.pack()

        fig = Figure(figsize=(5, 4), dpi=100)
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
        Label1.grid(row = 2 * number)
        ttk.Progressbar(frame, style=colourName, orient="horizontal", length=600, mode="determinate", maximum=4, value=1).grid(row = 2*number + 1, pady = 10)





app = MainWindow()
app.mainloop()
