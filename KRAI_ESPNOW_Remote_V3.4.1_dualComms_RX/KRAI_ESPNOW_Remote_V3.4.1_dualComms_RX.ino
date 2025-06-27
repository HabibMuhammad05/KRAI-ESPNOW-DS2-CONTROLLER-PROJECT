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

// ======== ADD BATTERY FUNCTION ========
const float R1 = 10000.0;
const float R2 = 100000.0;
const int BATTERY_PIN0 = 36;

float readBatteryVoltage0() {
  int raw = analogRead(BATTERY_PIN0);
  float voltage = raw * (3.3 / 4095.0);
  return voltage * ((R1 + R2) / R2);
}
const float R3 = 10000.0;
const float R4 = 22000.0;
const int BATTERY_PIN1 = 39;

float readBatteryVoltage1() {
  int raw = analogRead(BATTERY_PIN1);
  float voltage = raw * (3.3 / 4095.0);
  return voltage * ((R3 + R4) / R4);
}
const float R5 = 22000.0;
const float R6 = 100000.0;
const int BATTERY_PIN2 = 34;

float readBatteryVoltage2() {
  int raw = analogRead(BATTERY_PIN2);
  float voltage = raw * (3.3 / 4095.0);
  return voltage * ((R5 + R6) / R6);
}
void setup() {
    DEBUG_BEGIN(115200);
    Serial2.begin(115200, SERIAL_8N1, UART2_RX, UART2_TX);
     // ======== ADD ADC SETUP ========
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db); // Untuk pengukuran hingga 3.3V
    pinMode(BATTERY_PIN0, INPUT);
    pinMode(BATTERY_PIN1, INPUT);
    pinMode(BATTERY_PIN2, INPUT);
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
