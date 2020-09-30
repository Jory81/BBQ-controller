/*************************************************** 
  This is a library for the Adafruit PT100/P1000 RTD Sensor w/MAX31865

  Designed specifically to work with the Adafruit RTD Sensor
  ----> https://www.adafruit.com/products/3328

  This sensor uses SPI to communicate, 4 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Arduino.h>

#ifdef ESP8266
 #include <ESP8266WiFi.h>
 #include "FS.h" 
#else
 #include <WiFi.h>
 #include <SPIFFS.h>
#endif

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>

#include <ESPAsyncWebServer.h>

#include <Thermocouple.h>
#include <MAX6675_Thermocouple.h>
#include <Adafruit_MAX31865.h>

#if (defined(__AVR__))
#include <avr\pgmspace.h>
#else
#include <pgmspace.h>
#endif

#include <pt100rtd.h>

// OLED SD1306 properties
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// THERMOCOUPLE PINS
#define SCK_PIN 18
#define CS_PIN 5
//#define SO_PIN 19
//#define CS2_PIN 22
#define SO_PIN 21
#define CS2_PIN 26
// NOT FULLY TESTED AND OPERATIONAL FOR MULTIPLE SENSORS - FYI

#define RELAY 4  

Thermocouple* thermocouple[5];
//Thermocouple* thermocouple2;

#define LED 2
#define updateTimeTemp 1000
#define updateTimeGraph 10000

// Use software SPI: CS, DI, DO, CLK
//Adafruit_MAX31865 maxthermo = Adafruit_MAX31865(10, 11, 12, 13);
// use hardware SPI, just pass in the CS pin
Adafruit_MAX31865 maxthermo[5] = {Adafruit_MAX31865(5), Adafruit_MAX31865(26), Adafruit_MAX31865(5), Adafruit_MAX31865(5), Adafruit_MAX31865(5)} ;

// The value of the Rref resistor. Use 430.0!
#define RREF 430.0

// Like, duh.
#define C2F(c) ((9 * c / 5) + 32)

pt100rtd PT100 = pt100rtd();

/*
 * functions used in this script
 */
float processRTD(uint16_t rtd);
void initializeEEPROMvariables();
void sendAllTempToClient();

void updateTimeAndGraph();
void sendTimeToClient1 (uint16_t Time);
void sendCounterToClient1 (uint16_t Time);
void sendCounterToClient2 (uint16_t Time);
void updateGraph (float temp);
void updateGraph2 (float temp);
void displayOledScreen(float temp1, float temp2);
void fanControl();

void onRootRequest(AsyncWebServerRequest *request);
void initWebServer();
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void initWebSocket();
void notifyClients(); 
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void processWebSocketMessageS(String str, int stringLength, String dataString);
void processWebSocketMessage(String str, int dataVar);

void setupESP32();
void setupOledScreen();
void setupSPIFFS();
void setupEEPROM();
void setupWIFI();
void setupTempSensors();

#include "globalVariables.h"
#include "readTemperature.h"
#include "setupFunctions.h"
#include "websocketMessages.h"
#include "oledDisplay.h"

void setup()
{
setupESP32();
setupOledScreen();
setupSPIFFS();
setupEEPROM();
setupWIFI();
setupTempSensors();
}

void loop()
{
     if (millis() - previousMillis1 >= updateTimeTemp){
        switch (sensorType){
          case 1: {
            for (int sensor = 0; sensor <sensorAmount; sensor++){
              uint16_t rtd = maxthermo[sensor].readRTD();
              temp[sensor] = processRTD(rtd);
              oldtemp[sensor] = temp[sensor];
            }
            sendAllTempToClient ();
          }

          break;
          case 2: {
            for (int sensor = 0; sensor <sensorAmount; sensor++){
              temp[sensor] = thermocouple[sensor]->readCelsius();
                 if ((temp[sensor] < 300 && temp[sensor] > oldtemp[sensor]-50 && temp[sensor] < oldtemp[sensor]+50 ) || oldtemp[sensor] == 0){
                  oldtemp[sensor] = temp[sensor];             
                  }
            }
            sendAllTempToClient ();
          }
          break;
        }
  
    displayOledScreen(temp[0], temp[1]);
    fanControl();
    updateTimeAndGraph();
  
    previousMillis1 = millis();
    }
}
