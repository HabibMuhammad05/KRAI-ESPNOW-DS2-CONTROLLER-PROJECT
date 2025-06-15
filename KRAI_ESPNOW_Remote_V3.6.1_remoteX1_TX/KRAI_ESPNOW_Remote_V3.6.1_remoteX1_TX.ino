/*---------------------------------------------------------------------------------------------------------*/
/*---------------------------DS2 CONTROLLER WITH ESP32 & ESPNOW COMMUNICATION PROTOCOL---------------------*/
/*--------------------------------------Source Code by LEXARGA-24 TEAM-------------------------------------*/
/*-----------------------------------Modified & Adapted by LEXARGA-24 TEAM---------------------------------*/
/*---------------------------------------------------V3.6.1------------------------------------------------*/
/*------------------------------------Specified for Remote 1(gen 1 remote)---------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*------------------------------------LAST UPDATE AT 14:40:00, 26 JUN 25-----------------------------------*/

// Define DEBUG to enable debugging; comment it out to disable
//#define DEBUG

#ifdef DEBUG
  #define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
  #define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
  #define DEBUG_BEGIN(baud) Serial.begin(baud)
#else
  #define DEBUG_PRINT(...)    // Do nothing
  #define DEBUG_PRINTLN(...)  // Do nothing
  #define DEBUG_BEGIN(baud)   // Do nothing
#endif

#include "A_GlobalVariables.h"
#include "B_ESPNOW_Functions.h"
#include "C_Joystick_Functions.h"
#include "D_Oled_Functions.h"
#include "E_WebServer_Update.h"

//==========================================MAIN PROGRAM (SETUP & LOOP)=====================================//
void setup(){
  DEBUG_BEGIN(115200);
  WiFi.mode(WIFI_STA);  
//  String addrStr = String(WiFi.macAddress());
//  strcpy(sendData.address, addrStr.c_str());
  sendData.remoteIndex = 1;
  
  EEPROM.begin(256);
  reload_EEPROM();
  
  u8g2.setI2CAddress(0x78);
  u8g2.begin(); // start the u8g2 library

  for(uint8_t i = 0; i < 15; i++){
    pinMode(button[i], INPUT_PULLUP);
  }
  pinMode(2, INPUT_PULLUP);
  pinMode(voltPin, INPUT);
  pinMode(5, OUTPUT);
  
  calibrateJoysticks();
  dispSplashLogo();
//  batteryCheck();
  
  if (digitalRead(33)) { 
    settingMode = false; 
    startSend();
    startRecv();
//    OledDisplaySend();
  }else{
    settingMode = true;
    batteryCheck();
    StartOTA();
    displaySetPage();
  }  
}
 
void loop() {
  if (settingMode){
    ledBlink(500); 
    batteryCheck();
  }else{
    checkLatency(true);
    joystickRead();
    dataSent();
    if (digitalRead(2)) OledMonitorSend();
    else OledDisplaySend();
    batteryCheck();
//    delay(10);
    checkLatency(false);
  }
}

//===========================================Voltage reading FUNC===============================================//
float readings[20]; 
uint8_t readIndex = 0;
uint8_t totalSamples = 0;
unsigned long lastOutputTime = 0;
unsigned long lastSampleTime = 0;
void batteryCheck() {
  if (millis() - lastSampleTime >= 100) {
    lastSampleTime = millis();

    float vOut = (analogRead(voltPin) / 4095.0) * 3.3;
    float newVoltage = (vOut / voltageDividerFactor) * calibrationFactor;

    readings[readIndex] = newVoltage;
    readIndex = (readIndex + 1) % 20;
    if (totalSamples < 20) totalSamples++;
  }

  if ((millis() - lastOutputTime >= 2000)) {
    lastOutputTime = millis();
    float sum = 0;
    for (int i = 0; i < totalSamples; i++) {
      sum += readings[i];
    }
    batteryVoltage = sum / totalSamples;
//    batteryPercentage = (batteryVoltage - minVoltage) / (maxVoltage - minVoltage) * 100;
//    batteryPercentage = constrain(batteryPercentage, 0, 100);

    DEBUG_PRINT("                                             Battery Voltage: ");
    DEBUG_PRINT(batteryVoltage, 2);
    DEBUG_PRINT(" V, Percentage: ");
//    DEBUG_PRINT(batteryPercentage, 0);
//    DEBUG_PRINTLN(" %");
  }
}

//=============================================Blink ISR FUNC===============================================//

//void ledBlink(int wait){
//  static uint32_t blinkMillis;
//  if(millis() - blinkMillis >= wait){
//    blinkMillis=millis();
//    digitalWrite(5, !digitalRead(5));    
//  }
//}

void checkLatency(bool mod){  
  static unsigned long startTime;
  if (mod){ startTime = millis(); }
  else{
    unsigned long endTime = millis(); 
    periodMs = endTime - startTime;
    frequencyHz = 1000.0 / periodMs;
    DEBUG_PRINT("--F : "); DEBUG_PRINT(frequencyHz); DEBUG_PRINT(" Hz");
    DEBUG_PRINT("--T : ");   DEBUG_PRINT(periodMs); DEBUG_PRINTLN(" mS");
  }
}
