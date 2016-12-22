/*
 Created by Igor Jarc
 See http://iot-playground.com for details
 Please use community fourum on website do not contact author directly
 
 Code based on https://github.com/DennisSc/easyIoT-ESPduino/blob/master/sketches/ds18b20.ino
 
 External libraries:
 - https://github.com/milesburton/Arduino-Temperature-Control-Library
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
ADC_MODE(ADC_VCC); 

//AP definitions - update this
#define AP_SSID     "BTHub6-HWTS"
#define AP_PASSWORD "Ng6niWYXktnQ"

// EasyIoT Cloud definitions - change EIOT_CLOUD_INSTANCE_PARAM_ID
#define EIOT_CLOUD_INSTANCE_PARAM_ID    "58557de5c943a02af236ca35/kR16iIsGzSI6KPJ3"
#define EIOT_CLOUD_INSTANCE_PARAM_ID_2    "58557de5c943a02af236ca35/eHMgx30oSxoEWWS8"
#define REPORT_INTERVAL 60 // in sec


#define EIOT_CLOUD_ADDRESS     "cloud.iot-playground.com"
#define EIOT_CLOUD_PORT        40404




#define ONE_WIRE_BUS 2  // DS18B20 pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);


float oldTemp;


void setup() {
  Serial.begin(115200);
  
  wifiConnect();
    
  oldTemp = -1;
}

void loop() {
  float temp;
  float getVcc = ESP.getVcc();
  getVcc = getVcc*1.042e-3;
  
  do {
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
    Serial.print("Temperature: ");
    Serial.println(temp);
  } while (temp == 85.0 || temp == (-127.0));
  
  if (temp != oldTemp)
  {
    sendTeperature(temp,getVcc);
    oldTemp = temp;
  }
  
  int cnt = REPORT_INTERVAL;
  
  while(cnt--)
    delay(1000);
}

void wifiConnect()
{
    Serial.print("Connecting to AP");
    WiFi.begin(AP_SSID, AP_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");  
}

void sendTeperature(float temp, float volt)
{  
   WiFiClient client;
   
   while(!client.connect(EIOT_CLOUD_ADDRESS, EIOT_CLOUD_PORT)) {
    Serial.println("connection failed");
    wifiConnect(); 
  }

  String url = "";
  String url2="";
  // URL: "/RestApi/SetParameter/[insatnce id]/[parameter id]/[value]
  url += "/RestApi/SetParameter/"+ String(EIOT_CLOUD_INSTANCE_PARAM_ID) + "/"+String(temp); // generate EasIoT cloud update parameter URL
  url2 += "/RestApi/SetParameter/"+ String(EIOT_CLOUD_INSTANCE_PARAM_ID_2) + "/"+String(volt);

  Serial.print("POST data to URL: ");
  Serial.println(url);
  
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + String(EIOT_CLOUD_ADDRESS) + "\r\n" + 
               "Connection: close\r\n" + 
               "Content-Length: 0\r\n" + 
               "\r\n");

  delay(100);
    while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.println(line);
  }
  delay(200);
  Serial.print("POST data to URL: ");
  Serial.println(url2);

    client.print(String("POST ") + url2 + " HTTP/1.1\r\n" +
               "Host: " + String(EIOT_CLOUD_ADDRESS) + "\r\n" + 
               "Connection: close\r\n" + 
               "Content-Length: 0\r\n" + 
               "\r\n");

  delay(100);
    while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("Connection closed");
}
