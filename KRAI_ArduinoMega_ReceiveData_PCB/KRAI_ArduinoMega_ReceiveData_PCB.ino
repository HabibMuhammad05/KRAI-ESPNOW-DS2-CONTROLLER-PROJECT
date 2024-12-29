/*---------------------------------------------------------------------------------------------------------*/
/*-----------------------------------ARDUINO MEGA PARSING DATA FROM ESP32----------------------------------*/
/*--------------------------------------Source Code by LEXARGA-24 TEAM-------------------------------------*/
/*-----------------------------------Modified & Adapted by LEXARGA-24 TEAM---------------------------------*/
/*----------------------------------------------------V1.0-------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
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

#include <Arduino.h>

typedef struct struct_message {
    bool stat[15];
    int joyData[4];
} struct_message;

struct_message recvData;

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200);

    Serial.println("Arduino Mega initialized");
}

void loop() {
    if (Serial1.available()) {
        String receivedLine = Serial1.readStringUntil('\n'); 
        if (receivedLine.length() > 0) {
            parseData(receivedLine);

            Serial.print("Joystick: ");
            for (int i = 0; i < 4; i++) {
                Serial.print(recvData.joyData[i]);
                if (i < 3) Serial.print(", ");
            }

            Serial.print("---Button: ");
            for (int i = 0; i < 15; i++) {
                Serial.print(recvData.stat[i]);
                if (i < 14) Serial.print(", ");
            }
            Serial.println();
        }
    }
}

void parseData(String line) {
    int index = 0;
    char *token = strtok(line.c_str(), ",");
    while (token != nullptr) {
        if (index < 4) {
            recvData.joyData[index] = atoi(token); 
        } else if (index < 19) {
            recvData.stat[index - 4] = atoi(token); 
        }
        index++;
        token = strtok(nullptr, ",");
    }
}
