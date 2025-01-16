/*---------------------------------------------------------------------------------------------------------*/
/*--------------------ARDUINO MEGA PARSING DATA FROM ESP32, MECANUM WHEEL WITH JOYSTICK--------------------*/
/*--------------------------------------Source Code by LEXARGA-24 TEAM-------------------------------------*/
/*-----------------------------------Modified & Adapted by LEXARGA-24 TEAM---------------------------------*/
/*----------------------------------------------------V3.0-------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*------------------------------------LAST UPDATE AT 09:41:00, 16 JAN 25-----------------------------------*/

// Define DEBUG to enable debugging; comment it out to disable
#define DEBUG

#ifdef DEBUG
  #define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
  #define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
  #define DEBUG_BEGIN(baud) Serial.begin(baud)
#else
  #define DEBUG_PRINT(...)    
  #define DEBUG_PRINTLN(...)  
  #define DEBUG_BEGIN(baud)   
#endif

#include "dataReadFunc.h"
#include "mecanumControl.h"

bool R2lastState = true;
bool L2lastState = true;

uint8_t relay1 = 30;
uint8_t relay2 = 32;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  recvStart();
  mecanumSetup();
}

void loop(){
  checkData();
  delay(30);
  calcMecanum();
 
//=============================== turn on/off relay at D30 when R2 is pressed ===============================//
  if (!recvData.stat[12] && R2lastState) {
    digitalWrite(relay1, !digitalRead(relay1)); 
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); 
  }
  R2lastState = recvData.stat[12];
  
//=============================== turn on/off relay at D32 when R2 is pressed ===============================//
  if (!recvData.stat[2] && L2lastState) { 
    digitalWrite(relay2, !digitalRead(relay2));
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  L2lastState = recvData.stat[2];
}
