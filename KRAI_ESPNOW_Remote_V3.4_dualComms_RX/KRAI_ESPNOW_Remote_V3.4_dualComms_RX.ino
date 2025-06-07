/*---------------------------------------------------------------------------------------------------------*/
/*---------------------ESP32 ESP-NOW PROTOCOL RECEIVER WITH DATA SENDING TO ARDUINO MEGA-------------------*/
/*--------------------------------------Source Code by LEXARGA-24 TEAM-------------------------------------*/
/*-----------------------------------Modified & Adapted by LEXARGA-24 TEAM---------------------------------*/
/*----------------------------------------------------V3.3-------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*------------------------------------LAST UPDATE AT 11:42:00, 27 MAY 25-----------------------------------*/

// Define DEBUG to enable debugging; comment it out to disable
#define DEBUG

#ifdef DEBUG
  #define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
  #define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
  #define DEBUG_BEGIN(baud) Serial.begin(baud)
#else
  #define DEBUG_PRINT(...)    // Do nothing
  #define DEBUG_PRINTLN(...)  // Do nothing
  #define DEBUG_BEGIN(baud)   // Do nothing
#endif

void check_EEPROM();

#include "ESPNOW_Functions.h"
#include "EEPROM_Functions.h"

#define UART2_RX 16
#define UART2_TX 17

void setup() {
    DEBUG_BEGIN(115200);
    Serial2.begin(115200, SERIAL_8N1, UART2_RX, UART2_TX);

    EEPROM.begin(256);
    reload_EEPROM();
    
    WiFi.mode(WIFI_STA);
    startComms();
//    esp_now_register_recv_cb(OnDataRecv);

    DEBUG_PRINTLN("ESP32 initialized");
    
}

void loop() {
    dataSent();
    failSafeCheck(incomingData);
}
