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
#include "DebugMacros.h"
#include "Config.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "Passwords.h"

Ticker SensorTicker;
AsyncWebServer server(80);



/*
This is the code for the deevic that looks at the task tracking.




*/


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
    uint8_t _CurrentTask;
  private:
    unsigned long _PreviousTaskTime;
    Task _TaskList[4] = {(65) ,(66) , (67) , (68)};
};

TaskChanger::TaskChanger(){
  //TODO have someway to see the tasks names
  _PreviousTaskTime = millis();

}

bool TaskChanger::ChangeTask(){
  unsigned long DT = millis() - _PreviousTaskTime; //ok I am unsure exactly how long a unsigned long variable is so it is the one non uintx_t variable because of this.
  DT = DT / (60000); //should floor the value. Genreates the time in mineuts.
  _TaskList[_CurrentTask].IncreaseTaskTime(DT);
  _PreviousTaskTime = millis();
  digitalWrite(LedPins[_CurrentTask], LOW);
  _CurrentTask = (_CurrentTask + 1)%4 ;
  digitalWrite(_CurrentTask , HIGH);
  TaskChangingPrint(_CurrentTask);
  TaskChangingPrint(DT);
  return true;
}

TaskChanger TaskTracker;

/* The part of the code for the sensor reading and data logging


*/

bool ReadFromLog(String Location){
  File file = SPIFFS.open(Location);
  while(file.available()){
 
      Serial.write(file.read());
  }
  file.close();
}

bool WriteToLog(String Location , String Text){
  //Would be more efficient wih char arrays.
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
  String Bigstring = String("[" + String(LDR,HEX) + "," + String(Temp,HEX) + "," + String(TaskTracker._CurrentTask , DEC) + "]" );
  TickerPrint(Bigstring);
  WriteToLog(DataLog , Bigstring);
}

void setup() {
  Serial.begin(115200);

  pinMode(ButtonPin,INPUT);

  for(uint8_t i; i<4 ;i++){
    pinMode(LedPins[i] , OUTPUT);
  }

  if (!SPIFFS.begin(true)) {
    MainDebugPrint("SPIFFS wont load");
  }

  WiFi.begin(SSID, Password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());

  ReadFromLog(DataLog);
  SensorTicker.attach(SamplingFrequency , SampleSensors);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "It is good to be alive , Lets see how long that lasts");
  });

  server.on("/DataLog", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/log/LDRandTemp.log", String(), false);
  });

  server.on("/CT" , HTTP_GET, [](AsyncWebServerRequest *request){
    uint8_t a = TaskTracker._CurrentTask;
    String ReturnString = String(a);
    request->send(200, "text/plain",ReturnString);
  }); 

  server.begin();
  
}

bool Latch = true;


void loop() {
  if(digitalRead(ButtonPin)){
    TaskChangingPrint("Button Pressed");
    TaskTracker.ChangeTask();
    delay(1000);
  }
  
  // put your main code here, to run repeatedly:

}
