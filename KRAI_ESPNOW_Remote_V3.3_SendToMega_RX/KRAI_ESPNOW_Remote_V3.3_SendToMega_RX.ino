/*---------------------------------------------------------------------------------------------------------*/
/*---------------------ESP32 ESP-NOW PROTOCOL RECEIVER WITH DATA SENDING TO ARDUINO MEGA-------------------*/
/*--------------------------------------Source Code by LEXARGA-24 TEAM-------------------------------------*/
/*-----------------------------------Modified & Adapted by LEXARGA-24 TEAM---------------------------------*/
/*----------------------------------------------------V3.3-------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
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

#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
    bool stat[15];
    int joyData[4];
} struct_message;

struct_message recvData;

#define UART2_RX 16
#define UART2_TX 17

void setup() {
    DEBUG_BEGIN(115200);
    Serial2.begin(115200, SERIAL_8N1, UART2_RX, UART2_TX);

    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        DEBUG_PRINTLN("Error initializing ESP-NOW");
        return;
    }
    esp_now_register_recv_cb(OnDataRecv);

    DEBUG_PRINTLN("ESP32 initialized");
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    memcpy(&recvData, incomingData, sizeof(recvData));
    String dataLine = String();
    for (int i = 0; i < 4; i++) {
        dataLine += String(recvData.joyData[i]) + ",";
    }
    for (int i = 0; i < 15; i++) {
        dataLine += String(recvData.stat[i]) + ",";
    }
    dataLine.remove(dataLine.length() - 1); 

    Serial2.println(dataLine);
    DEBUG_PRINTLN("Sent Data: " + dataLine);
}

void loop() {
}
