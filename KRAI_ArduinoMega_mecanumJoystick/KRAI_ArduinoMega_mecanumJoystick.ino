/*---------------------------------------------------------------------------------------------------------*/
/*--------------------ARDUINO MEGA PARSING DATA FROM ESP32, MECANUM WHEEL WITH JOYSTICK--------------------*/
/*--------------------------------------Source Code by LEXARGA-24 TEAM-------------------------------------*/
/*-----------------------------------Modified & Adapted by LEXARGA-24 TEAM---------------------------------*/
/*----------------------------------------------------V3.0-------------------------------------------------*/
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

#include "dataReadFunc.h"
#include "mecanumControl.h"

void setup() {
  recvStart();
  mecanumSetup();
}

void loop(){
  checkData();
  delay(30);
  calcMecanum();
}
