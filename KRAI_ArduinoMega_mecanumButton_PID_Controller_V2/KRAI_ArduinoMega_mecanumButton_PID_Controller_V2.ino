/*---------------------------------------------------------------------------------------------------------*/
/*--------------------ARDUINO MEGA PARSING DATA FROM ESP32, MECANUM WHEEL WITH BUTTON----------------------*/
/*-------------------INCLUDING INTERNAL ENCODER FEEDBACK WITH PID CLOSED LOOP FEEDBACK--------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*--------------------------------------Source Code by LEXARGA-24 TEAM-------------------------------------*/
/*-----------------------------------Modified & Adapted by LEXARGA-24 TEAM---------------------------------*/
/*----------------------------------------------------V2.2-------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*------------------------------------LAST UPDATE AT 12:20:00, 15 JAN 25-----------------------------------*/

// Define DEBUG to enable debugging; comment it out to disable
//#define DEBUG

#ifdef DEBUG
  #define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
  #define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
  #define DEBUG_BEGIN(baud) Serial.begin(baud)
#else
  #define DEBUG_PRINT(...)    
  #define DEBUG_PRINTLN(...)  
  #define DEBUG_BEGIN(baud)   
#endif

#include "mecanumControl.h"
#include "dataReadFunc.h"

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
  PIDSetup();
  stopMotor();
}

void loop(){
  checkData();
  delay(10);
  if(!recvData.stat[0]){ stopMotor(); }
  else if(!recvData.stat[3]){ counterClockwise(); }
  else if(!recvData.stat[4]){ forward(); }
  else if(!recvData.stat[6]){ backward(); }
  else if(!recvData.stat[5]){ left(); }
  else if(!recvData.stat[7]){ right(); }
  else if(!recvData.stat[8]){ forwardLeft(); }
  else if(!recvData.stat[11]){ forwardRight(); }
  else if(!recvData.stat[9]){ backwardLeft(); }
  else if(!recvData.stat[10]){ backwardRight(); }
  else if(!recvData.stat[13]){ clockwise(); }
  else{ stopMotor(); }
 
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
