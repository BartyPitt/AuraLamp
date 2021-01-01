/*
Code written by Barty Pitt from Imperial Collage London.
in December 2020.
Designed to record the daily activity along with the temprature and light level.
The code is writen for ESP32 and tested on esp32 Wroom. with 4mb of Flash.
Should work with many other boards although this is not guaranteed.
Not sure if 

*/

#include <Arduino.h>
#include <SPIFFS.h>
#include <Ticker.h>
//Extra headers files that I made to help myself.
//Passwords.h Should not exist for someone else.......
//#include "../Passwords.h"
#include "DebugMacros.h"
#include "Config.h"

Ticker SensorTicker;

class Task{
  public:
    Task(int Name);
    bool Reset();
    bool IncreaseTaskTime(unsigned long Time);
    bool FormatReturn(char *ReturnString);
  private:
    uint8_t _CurrentTaskTime;
    char _Name;
};

Task::Task(int Name){
  _Name = Name;
  _CurrentTaskTime = 0;
}

bool Task::IncreaseTaskTime(unsigned long Time){
  _CurrentTaskTime = _CurrentTaskTime + Time;
  return true;
}

bool Task::FormatReturn(char *ReturnString){
  String Output = String(String(_Name) +"," + String(_CurrentTaskTime , HEX) );
  Output.toCharArray(ReturnString , 50);
}



class TaskChanger{
  //At some point this system needs o bee changed to allow creation and deleating of tasks.
  //At the moment there are 4 predefined tasks.
  public:
    TaskChanger();
    bool ChangeTask();
  private:
    unsigned long _PreviousTaskTime;
    uint8_t _CurrentTask;
    Task _TaskList[4] = {(65) ,(66) , (67) , (68)};
};

TaskChanger::TaskChanger(){
  //TODO have someway to see the tasks names
  _PreviousTaskTime = millis();

}

bool TaskChanger::ChangeTask(){
  unsigned long DT = _PreviousTaskTime - millis(); //ok I am unsure exactly how long a unsigned long variable is so it is the one non uintx_t variable because of this.
  DT = DT / (60000); //should floor the value. Genreates the time in mineuts.
  _TaskList[_CurrentTask].IncreaseTaskTime(DT);
  _PreviousTaskTime = millis();
  _CurrentTask = (_CurrentTask + 1)%4 
  TaskChangingPrint(_CurrentTask);
  TaskChangingPrint(DT);
  return true;
}



bool WriteToLog(String Location , String Text){
  File log =  SPIFFS.open(Location , FILE_APPEND);
  if(log.print(Text)){
      MainDebugPrint("Written to the file");
    }
  else{
    MainDebugPrint("SPIFFS broken when doing sensor readings");
  }
}



void SampleSensors(){
  uint32_t LDR = analogRead(LDRPin);
  uint32_t Temp = analogRead(ThermistorPin);
  String Bigstring = String("!" + String(LDR,HEX) + "," + String(Temp,HEX) + "!" );
  WriteToLog(DataLog , Bigstring);
}


TaskChanger TaskTracker;

void setup() {
  Serial.begin(115200);

  for(uint8_t i; i<3 ;i++){
    pinMode(LedPins[i] , OUTPUT);
  }

  if (!SPIFFS.begin(true)) {
    MainDebugPrint("SPIFFS wont load");
  }
  SensorTicker.attach(SamplingFrequency , SampleSensors);

  
}

bool Latch = true;


void loop() {
  if(digitalRead(ButtonPin)){
    TaskTracker.ChangeTask();
    delay(250);
  }
  
  // put your main code here, to run repeatedly:

}