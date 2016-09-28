#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <ESPTelegramBot.h>
#include "gpio.h"
#include <string.h>

// Initialize Wifi connection to the router
const char *ssid = "PlusnetWireless2DA649";   // cannot be longer than 32 characters!
const char *pass = "Wan0$PIDB";   // your network key
int status = WL_IDLE_STATUS;


// Initialize Telegram BOT
#define BOTtoken "183104961:AAFOVTLmfQ0MDHdt2ZnLgtUZYkM_gbDFkLs"  //token of TestBOT
#define BOTname "esp_bot"
#define BOTusername "esp_e14_bot"
TelegramBOT bot(BOTtoken, BOTname, BOTusername);

void wokenup()
{
//wifi_fpm_close(); // disable force sleep function
//wifi_set_opmode(STATION_MODE); // set station mode
//wifi_station_connect(); // connect to AP
WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address: ");
IPAddress ip = WiFi.localIP();
//Serial.println(ip);
float v = ESP.getVcc()/65535*3.3;
//Serial.println(v);
char* str;
sprintf(str,"%.2g",v);
//if (v>=3.0) str = "OK";
//else str = "LOW";
String s = "Door is open, voltage is ";
s += str;
//strncat(s,str,1);
bot.begin();      // launch Bot functionalities
bot.sendMessage("181982455", s, "");
Serial.println("Door is open");
delay(1000);
//go_sleep();
}

void setup() {
  Serial.begin(115200);
  // check for the presence of the shield:

  //go_sleep();
 }

void loop() {
  // put your main code here, to run repeatedly:
wokenup();
Serial.println("go to sleep");
delay(500);

ESP.deepSleep(1E13); // Sleep
//ESP.restart();
delay(500);
//Serial.println("woken up");
//wokenup();

}




