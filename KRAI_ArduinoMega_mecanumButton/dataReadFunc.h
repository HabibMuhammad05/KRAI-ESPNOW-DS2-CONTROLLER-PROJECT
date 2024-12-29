#include <Arduino.h>

typedef struct struct_message {
    bool stat[15];
    int joyData[4];
} struct_message;

struct_message recvData;


void recvStart(){
    DEBUG_BEGIN(115200);
    Serial1.begin(115200); 

    DEBUG_PRINTLN("Arduino Mega initialized");
}

void parseData(String line) {
    int index = 0;
    char *token = strtok(line.c_str(), ",");
    while (token != nullptr) {
        if (index < 4) {
            recvData.joyData[index] = atoi(token); // Parse joystick data
        } else if (index < 19) {
            recvData.stat[index - 4] = atoi(token); // Parse button states
        }
        index++;
        token = strtok(nullptr, ",");
    }
}

void checkData(){
    if (Serial1.available()) {
        String receivedLine = Serial1.readStringUntil('\n'); 
        if (receivedLine.length() > 0) {
            parseData(receivedLine);

            DEBUG_PRINT("Joystick: ");
            for (int i = 0; i < 4; i++) {
                DEBUG_PRINT(recvData.joyData[i]);
                if (i < 3) DEBUG_PRINT(", ");
            }

            DEBUG_PRINT("---Button: ");
            for (int i = 0; i < 15; i++) {
                DEBUG_PRINT(recvData.stat[i]);
                if (i < 14) DEBUG_PRINT(", ");
            }
            DEBUG_PRINTLN();
        }
    }
}
