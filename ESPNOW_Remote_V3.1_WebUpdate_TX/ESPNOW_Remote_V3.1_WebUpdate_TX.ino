/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------DS2 CONTROLLER WITH ESP32 & ESPNOW COMMUNICATION PROTOCOL---------------------*/
/*--------------------------------------Source Code by LEXARGA-24 TEAM-------------------------------------*/
/*-----------------------------------Modified & Adapted by LEXARGA-24 TEAM---------------------------------*/
/*---------------------------------------------------V3.0--------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

#include "A_GlobalVariables.h"
#include "B_ESPNOW_Functions.h"
#include "C_Joystick_Functions.h"
#include "D_Oled_Functions.h"
#include "E_WebServer_Update.h"


//==========================================MAIN PROGRAM (SETUP & LOOP)=====================================//
void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);  
  
  u8g2.setI2CAddress(0x78);
  u8g2.begin(); // start the u8g2 library

  for(uint8_t i = 0; i < 15; i++){
    pinMode(button[i], INPUT_PULLUP);
  }
  pinMode(18, OUTPUT);
  pinMode(5, OUTPUT);
  
  calibrateJoysticks();
  dispSplashLogo();
  
  if (digitalRead(33)) { 
    settingMode = false; 
    
    startSend();
    OledDisplaySend();
  }else{
    settingMode = true;
    StartOTA();
    displaySetPage();
  }  
}
 
void loop() {
  if (settingMode){
    ledBlink(500); 
  }else{
    dataSent();
    joystickRead();
    OledDisplaySend();
    ledBlink(1000);
    delay(100);
  }
}


//=============================================Blink ISR FUNC===============================================//
unsigned int blinkMillis = 0;
void ledBlink(int wait){
  if(millis() - blinkMillis>= wait){
    blinkMillis=millis();
    digitalWrite(18, !digitalRead(18));
    digitalWrite(5, !digitalRead(18));    
  }
}
