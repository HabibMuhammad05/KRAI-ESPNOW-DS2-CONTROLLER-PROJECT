#include <WiFi.h>
#include <esp_now.h>

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#include <U8g2lib.h>

bool settingMode;

//============================EDIT THIS TO CHANGE THE RX MAC ADDRESS==================================//
//uint8_t broadcastAddress[] = {0xE8, 0x6B, 0xEA, 0xD4, 0xA5, 0x84}; //esp32 ext antenna
uint8_t broadcastAddress[] = {0x8C, 0x4F, 0x00, 0x3C, 0x91, 0x7C}; // esp32 v1 microUSB

//=========================VARS TO SENT BY ESPNOW - MUST MATCH WITH RX================================//
typedef struct struct_message {
  bool stat[15];
  int joyData[4];
} struct_message;
struct_message sendData; //struct for sending data.

//==============================VARS FOR DS2 CONTROLLER BUTTONS=======================================//
//uint8_t button[15] = {33,   18,   23,   19,   4,     15,     13,     12,       25,      26,      27,        14,      16,   17,  2}; 
uint8_t button[15] = {33,   18,   16,   17,   4,     15,     13,     12,       25,      26,      27,        14,      19,   23,  2}; 
                  // "L3", "R3", "L2", "L1", "UP", "LEFT", "DOWN", "RIGHT", "SQUARE", "CROSS", "ROUND", "TRIANGLE", "R2", "R1", SW
                  //   0    1     2     3     4       5       6       7         8         9       10        11       12    13   14
uint8_t joy1XPin = 39; 
uint8_t joy1YPin = 36; 
uint8_t joy2XPin = 35; 
uint8_t joy2YPin = 34; 

const int NUM_SAMPLES = 3;
static int joy1XBuffer[NUM_SAMPLES] = {0}, joy1YBuffer[NUM_SAMPLES] = {0};
static int joy2XBuffer[NUM_SAMPLES] = {0}, joy2YBuffer[NUM_SAMPLES] = {0};
static int joy1XSum = 0, joy1YSum = 0, joy2XSum = 0, joy2YSum = 0;
static int joy1XIndex = 0, joy1YIndex = 0, joy2XIndex = 0, joy2YIndex = 0;

int joy1XCenter, joy1YCenter, joy2XCenter, joy2YCenter;
int joy1XPos, joy1YPos, joy2XPos, joy2YPos;

const int boxSize = 30;
const int joy1XBox = 48;
const int joy1YBox = 47;
const int joy2XBox = 80;
const int joy2YBox = 47;

const int centerThreshold = 45;

//==============================VARS FOR BATT VOLTAGE MEASUREMENT=======================================//
uint8_t voltPin = 32; 
float batteryVoltage, batteryPercentage;
const float voltageDividerFactor = 47.0 / (100.0 + 47.0);
const float calibrationFactor = 1.066; 
const float minVoltage = 6.0; 
const float maxVoltage = 8.4; 
