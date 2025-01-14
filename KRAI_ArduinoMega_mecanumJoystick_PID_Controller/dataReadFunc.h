//button[15] = {33,   18,   16,   17,   4,     15,     13,     12,       25,      26,      27,        14,      19,   23,  2}; 
            // "L3", "R3", "L2", "L1", "UP", "LEFT", "DOWN", "RIGHT", "SQUARE", "CROSS", "ROUND", "TRIANGLE", "R2", "R1", SW
            //   0    1     2     3     4       5       6       7         8         9       10        11       12    13   14
#include <Arduino.h>


//================================== STRUCT FOR RECEIVED AND PARSED DATA ==================================//
typedef struct struct_message {
    bool stat[15] = {true};
    int joyData[4];
} struct_message;
struct_message recvData;


//=================================== SERIAL MONITOR AND UART COMM INIT ===================================//
void recvStart(){
    DEBUG_BEGIN(115200);
    Serial1.begin(115200); 
    DEBUG_PRINTLN("Arduino Mega initialized");
}


//================================== UART INCOMING DATA PARSING FUNCTION ==================================//
void parseData(String line) {
    int index = 0;
    char *token = strtok(line.c_str(), ",");
    while (token != nullptr) {
        if (index < 4) {
            recvData.joyData[index] = atoi(token); //joystick data
        } else if (index < 19) {
            recvData.stat[index - 4] = atoi(token); //button states
        }
        index++;
        token = strtok(nullptr, ",");
    }
}


//========================================= UART DATA CHECKING ============================================//
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
