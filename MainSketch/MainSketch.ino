/*
Code written by Barty Pitt from Imperial Collage London.
in December 2020.
Designed to record the daily activity along with the temprature and light level.
The code is writen for ESP32 and tested on esp32 Wroom. with 4mb of Flash.
Should work with many other boards although this is not guaranteed.
So should change to *responce by defult.
*/

#include <Arduino.h>
#include <SPIFFS.h>
#include <Ticker.h>
//Extra headers files that I made to help myself.
//Passwords.h Should not exist for someone else.......
#include "DebugMacros.h"
//#include "Config.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "Passwords.h"
#include "Config.h"

Ticker SensorTicker;
AsyncWebServer server(80);

/*
This is the code for the deevic that looks at the task tracking.

Turns out that it is very easy to store byte data in the spiffs file system. 
Ehh I will know this for next time.


*/

class Task
{
public:
  Task(int ID);
  void UpdateTime();
  void UpdateTime(uint8_t Up);
  void ClearData();
  uint32_t RecallTime();

private:
  String _FileLocation;
};

Task::Task(int ID)
{
  _FileLocation = String(StorageLocation + String(ID) + "CurrentTime.Log");
}

void Task::UpdateTime(uint8_t Up)
{

  uint32_t CurrentTime = Up + RecallTime();

  File WriteFile = SPIFFS.open(_FileLocation, FILE_WRITE);
  WriteFile.print(String(CurrentTime));
  WriteFile.close();
}

void Task::UpdateTime()
{
  UpdateTime(1);
}

uint32_t Task::RecallTime()
{
  File ReadFile = SPIFFS.open(_FileLocation, FILE_READ);
  String ReadData = ReadFile.readString();
  uint64_t CurrentTime = ReadData.toInt();
  ReadFile.close();
  return CurrentTime;
}

void Task::ClearData()
{
  SPIFFS.remove(_FileLocation);
}

class TaskChanger
{
  //At some point this system needs o bee changed to allow creation and deleating of tasks.
  //At the moment there are 4 predefined tasks.
public:
  TaskChanger();
  bool ChangeTask();
  uint8_t _CurrentTask;
  void UpdateLog();
  void CreateReturnString(String *ReturnString,AsyncWebServerRequest *request);

private:
  Task _TaskList[4] = {(0), (1), (2), (3)};
};

TaskChanger::TaskChanger()
{
  //TODO have someway to see the tasks names
}

bool TaskChanger::ChangeTask()
{
  digitalWrite(LedPins[_CurrentTask], LOW);

  _CurrentTask = (_CurrentTask + 1) % 4;
  digitalWrite(LedPins[_CurrentTask], HIGH);
  TaskChangingPrint(_CurrentTask);
  return true;
}

void TaskChanger::UpdateLog()
{
  _TaskList[_CurrentTask].UpdateTime();
}

void TaskChanger::CreateReturnString(String *ReturnString , AsyncWebServerRequest *request)
{
  Serial.println("Arrived in carnage");
  String BigString = *ReturnString;
  BigString = "<html> <body>";
  uint32_t temp;
  for (uint8_t i = 0; i < 4; i++)
  {
    temp = _TaskList[i].RecallTime();
    BigString = String(BigString + " Task" + String(i) + (" ") + String(temp) + " <br>");
  }
  BigString = BigString + "</body> , </html>";
  request->send(200, "text/html", BigString);
  return;
}

TaskChanger TaskTracker;

/* The part of the code for the sensor reading and data logging


*/

void StoreData()
{
  Serial.println("Updating log");
  TaskTracker.UpdateLog();
}

bool ReadFromLog(String Location)
{
  File file = SPIFFS.open(Location);
  while (file.available())
  {

    Serial.write(file.read());
  }
  file.close();
}

void setup()
{
  Serial.begin(115200);
  Serial.println("working");

  pinMode(ButtonPin, INPUT);

  for (uint8_t i = 0; i < 4; i++)
  {
    pinMode(LedPins[i], OUTPUT);
    digitalWrite(LedPins[i] , HIGH);
    delay(1000);
    digitalWrite(LedPins[i] , LOW);
  }


  if (!SPIFFS.begin(true))
  {
    MainDebugPrint("SPIFFS wont load");
  }
  WiFi.softAP(SSID, Password);
  Serial.println(WiFi.softAPIP());

  SensorTicker.attach(SamplingFrequency, StoreData);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String StringyBoi; 
    TaskTracker.CreateReturnString(&StringyBoi , request);
    Serial.println("Welpio");
  });
  server.begin();
}

bool Latch = true;

void loop()
{
  if (digitalRead(ButtonPin))
  {
    TaskChangingPrint("Button Pressed");
    TaskTracker.ChangeTask();
    delay(1000);
  }

  // put your main code here, to run repeatedly:
}
