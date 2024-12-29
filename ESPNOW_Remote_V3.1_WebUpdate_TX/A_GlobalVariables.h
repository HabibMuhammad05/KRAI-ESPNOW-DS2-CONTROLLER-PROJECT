#include <WiFi.h>
#include <esp_now.h>

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#include <U8g2lib.h>

bool settingMode;
//============================EDIT THIS TO CHANGE THE RX MAC ADDRESS==================================//
uint8_t broadcastAddress[] = {0xE8, 0x6B, 0xEA, 0xD4, 0xA5, 0x84};

//=========================VARS TO SENT BY ESPNOW - MUST MATCH WITH RX================================//
typedef struct struct_message {
  bool stat[15];
  int joyData[4];
//  char a[32];
//  int b;
//  float c;
//  bool d;
} struct_message;
struct_message sendData; //struct for sending data.

//==============================VARS FOR DS2 CONTROLLER BUTTONS=======================================//
uint8_t button[15] = {33, 32, 23, 19, 4, 15, 13, 12, 25, 26, 27, 14, 16, 17, 2}; // "L3", "R3", "L2", "L1", "UP", "LEFT", "DOWN", "RIGHT", "SQUARE", "CROSS", "ROUND", "TRIANGLE", "R2", "R1"

uint8_t joy1XPin = 39; // Joystick 1 X-axis
uint8_t joy1YPin = 36; // Joystick 1 Y-axis
uint8_t joy2XPin = 35; // Joystick 2 X-axis
uint8_t joy2YPin = 34; // Joystick 2 Y-axis

int joy1XCenter, joy1YCenter, joy2XCenter, joy2YCenter;
int joy1XPos, joy1YPos, joy2XPos, joy2YPos;

const int boxSize = 32;
const int joy1XBox = 48;
const int joy1YBox = 48;
const int joy2XBox = 80;
const int joy2YBox = 48;

const int centerThreshold = 10;
