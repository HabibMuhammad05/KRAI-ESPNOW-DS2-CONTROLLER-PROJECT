/*---------------------------------------------------------------------------------------------------------*/
/*--------------------ARDUINO MEGA PARSING DATA FROM ESP32, MECANUM WHEEL WITH BUTTON----------------------*/
/*--------------------------------------Source Code by LEXARGA-24 TEAM-------------------------------------*/
/*-----------------------------------Modified & Adapted by LEXARGA-24 TEAM---------------------------------*/
/*----------------------------------------------------V2.0-------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
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

#include "mecanumControl.h"
#include "dataReadFunc.h"

void setup() {
  recvStart();
  mecanumSetup();
  stopMotor();
}

void loop(){
  checkData();
  delay(20);
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
}
