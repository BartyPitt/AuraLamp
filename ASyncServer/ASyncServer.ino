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
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "Passwords.h"

AsyncWebServer server(80);

void setup() {
  
  // put your setup code here, to run once:
  Serial.begin(115200);
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
  }
  WiFi.begin(SSID, Password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());

  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "It is good to be alive , Lets see how long that lasts");
  });

  server.on("/DataLog", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/log/LDRandTemp.log", String(), false);
  });

  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
