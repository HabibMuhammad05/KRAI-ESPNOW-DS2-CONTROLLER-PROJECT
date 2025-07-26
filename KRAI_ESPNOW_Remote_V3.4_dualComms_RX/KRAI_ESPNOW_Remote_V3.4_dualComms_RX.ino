/*---------------------------------------------------------------------------------------------------------*/
/*---------------------ESP32 ESP-NOW PROTOCOL RECEIVER WITH DATA SENDING TO ARDUINO MEGA-------------------*/
/*--------------------------------------Source Code by LEXARGA-24 TEAM-------------------------------------*/
/*-----------------------------------Modified & Adapted by LEXARGA-24 TEAM---------------------------------*/
/*----------------------------------------------------V3.4-------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*------------------------------------LAST UPDATE AT 11:42:00, 2 JUL 25-----------------------------------*/

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

// Define ROBOT1 before uploading it to Robot 1;
// Comment it if uploading this code to robot 2; 
//#define ROBOT1


void check_EEPROM();

#include "ESPNOW_Functions.h"
#include "EEPROM_Functions.h"
#include "BatteryEncoderLed_Functions.h"

#define UART2_RX 16
#define UART2_TX 17

void setup(){
    DEBUG_BEGIN(115200);
    Serial2.begin(115200, SERIAL_8N1, UART2_RX, UART2_TX);

    pinMode(pin3S, INPUT);
    pinMode(pin4S, INPUT);
    pinMode(pin6S, INPUT);
    pinMode(encPin1, INPUT_PULLUP); 
    pinMode(encPin2, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(encPin1), isr1, FALLING);
    attachInterrupt(digitalPinToInterrupt(encPin2), isr2, FALLING);

    pinMode(rRelay, OUTPUT);
    pinMode(gRelay, OUTPUT);
    pinMode(bRelay, OUTPUT);
    digitalWrite(rRelay, HIGH);
    digitalWrite(gRelay, HIGH);
    digitalWrite(bRelay, HIGH);
    
    EEPROM.begin(256);
    reload_EEPROM();
    
    WiFi.mode(WIFI_STA);
    startComms();
//    esp_now_register_recv_cb(OnDataRecv);

    DEBUG_PRINTLN("ESP32 initialized");
    
}

void loop(){
    batteryRead();
    dataSent();
    failSafeCheck(incomingData);
    handleEncoders();
    handleLED();
}
