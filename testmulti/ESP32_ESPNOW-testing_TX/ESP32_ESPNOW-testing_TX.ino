/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

// Create a struct_message called sendData
// REPLACE WITH YOUR RECEIVER MAC Address
// Structure example to send data
// Must match the receiver structure
*/
#include <esp_now.h>
#include <WiFi.h>

#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 22, /* data=*/ 23, /* cs=*/ U8X8_PIN_NONE, /* dc=*/ 21, /* reset=*/ 19);

static const unsigned char image_GameMode_bits[] U8X8_PROGMEM = {0x20,0x00,0xfe,0x03,0xfb,0x07,0x71,0x05,0xfb,0x07,0x8f,0x07,0x07,0x07,0x03,0x06};
static const unsigned char image_Bluetooth_Connected_bits[] U8X8_PROGMEM = {0x04,0x00,0x0d,0x00,0x16,0x60,0x4c,0x97,0x4c,0x97,0x16,0x60,0x0d,0x00,0x04,0x00};
static const unsigned char image_arrow_up_bits[] U8X8_PROGMEM = {0x04,0x0e,0x15,0x04,0x04,0x04,0x04};
static const unsigned char image_arrow_down_bits[] U8X8_PROGMEM = {0x04,0x04,0x04,0x04,0x15,0x0e,0x04};
static const unsigned char image_arrow_right_bits[] U8X8_PROGMEM = {0x10,0x20,0x7f,0x20,0x10};
static const unsigned char image_arrow_left_bits[] U8X8_PROGMEM = {0x04,0x02,0x7f,0x02,0x04};
static const unsigned char image_InfraredArrowUp_bits[] U8X8_PROGMEM = {0x18,0x3c,0x7e,0xff};
static const unsigned char image_InfraredArrowDown_bits[] U8X8_PROGMEM = {0xff,0x7e,0x3c,0x18};
static const unsigned char image_ButtonRight_bits[] U8X8_PROGMEM = {0x01,0x03,0x07,0x0f,0x07,0x03,0x01};
static const unsigned char image_ButtonLeft_bits[] U8X8_PROGMEM = {0x08,0x0c,0x0e,0x0f,0x0e,0x0c,0x08};
uint8_t button[15] = {33, 32, 23, 19, 4, 15, 13, 12, 25, 26, 27, 14, 16, 17, 2}; // "L3", "R3", "L2", "L1", "UP", "LEFT", "DOWN", "RIGHT", "SQUARE", "CROSS", "ROUND", "TRIANGLE", "R2", "R1"
uint8_t joy1XPin = 39; // Joystick 1 X-axis
uint8_t joy1YPin = 36; // Joystick 1 Y-axis
uint8_t joy2XPin = 35; // Joystick 2 X-axis
uint8_t joy2YPin = 34; // Joystick 2 Y-axis
int joy1XCenter, joy1YCenter, joy2XCenter, joy2YCenter;
int joy1XPos, joy1YPos, joy2XPos, joy2YPos;

const int boxSize = 32;
const int joy1XBox = 48;
const int joy1YBox = 48;
const int joy2XBox = 80;
const int joy2YBox = 48;

const int centerThreshold = 10;

uint8_t broadcastAddress[] = {0xD0, 0xEF, 0x76, 0x34, 0x7A, 0xC0};

typedef struct struct_message {
  bool stat[15];
  int joyData[4];
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;
struct_message sendData; //struct variable for sending data.

struct_message recvData; //struct variable for receiving data.

//========================================ESPNOW DATA SENDING FUNCTIONS======================================//
bool connectOk = false;
esp_now_peer_info_t peerInfo;
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {  // callback when data is sent
  Serial.print("--Last Packet Send Status:");
  Serial.print(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  connectOk = (status == ESP_NOW_SEND_SUCCESS);
}

void startSend(){
  if (esp_now_init() != ESP_OK) { Serial.println("Error initializing ESP-NOW"); return; }
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;      
  if (esp_now_add_peer(&peerInfo) != ESP_OK){ Serial.println("Failed to add peer"); return; }
}

void dataSent(){
  strcpy(sendData.a, "THIS IS A CHAR");
  sendData.b = random(1,20);
  sendData.c = random(20,60);
  sendData.d = random(0,1);
  Serial.print("---Sent : ");
  Serial.print(sendData.a);
  Serial.print("--");
  Serial.print(sendData.b);
  Serial.print("--");
  Serial.print(sendData.c);
  Serial.print("--");
  Serial.print(sendData.d);
  Serial.print("----");
  
  for(uint8_t i = 0; i < 15; i++){
    sendData.stat[i] = digitalRead(button[i]);
    Serial.print(sendData.stat[i]);
    Serial.print(", ");
  }
  Serial.println();
  
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &sendData, sizeof(sendData));
  if (result == ESP_OK) { Serial.print("Sent with success---"); }
  else { Serial.print("Error sending the data---"); }
}

//========================================ESPNOW DATA RECEIVING FUNCTIONS==================================//
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&recvData, incomingData, sizeof(recvData));
  Serial.print("Bytes received: ");
  Serial.print(len);
  Serial.print("---Char: ");
  Serial.print(recvData.a);
  Serial.print("---Int: ");
  Serial.print(recvData.b);
  Serial.print("---Float: ");
  Serial.print(recvData.c);
  Serial.print("---Bool: ");
  Serial.print(recvData.d);
  Serial.println();
}
void startRecv(){
  if (esp_now_init() != ESP_OK) { 
    Serial.println("Error initializing ESP-NOW"); 
    return; 
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

//===============================================BUTTON READING FUNC=======================================//
void calibrateJoysticks() {
  Serial.println("Calibrating joysticks...");

  // Take multiple readings to determine the center position
  int numSamples = 50;
  long joy1XSum = 0, joy1YSum = 0;
  long joy2XSum = 0, joy2YSum = 0;

  for (int i = 0; i < numSamples; i++) {
    joy1XSum += analogRead(joy1XPin);
    joy1YSum += analogRead(joy1YPin);
    joy2XSum += analogRead(joy2XPin);
    joy2YSum += analogRead(joy2YPin);
    delay(10);
  }

  // Calculate the average center position
  joy1XCenter = joy1XSum / numSamples;
  joy1YCenter = joy1YSum / numSamples;
  joy2XCenter = joy2XSum / numSamples;
  joy2YCenter = joy2YSum / numSamples;

  Serial.println("Calibration complete.");
  Serial.print("Joystick 1 Center: ");
  Serial.print(joy1XCenter);
  Serial.print(", ");
  Serial.println(joy1YCenter);
  Serial.print("Joystick 2 Center: ");
  Serial.print(joy2XCenter);
  Serial.print(", ");
  Serial.println(joy2YCenter);
}

//=========================================joystick and oled READING FUNC==================================//
void joystickRead(){
  
  int joy1XValue = analogRead(joy1XPin) - joy1XCenter;
  int joy1YValue = analogRead(joy1YPin) - joy1YCenter;
  int joy2XValue = analogRead(joy2XPin) - joy2XCenter;
  int joy2YValue = analogRead(joy2YPin) - joy2YCenter;
  sendData.joyData[0] = joy1XValue;
  sendData.joyData[1] = joy1YValue;
  sendData.joyData[2] = joy2XValue;
  sendData.joyData[3] = joy2YValue;
  
  Serial.print(String(joy1XValue) + ", " + String(joy1YValue) + ", " + String(joy2XValue) + ", " + String(joy2YValue) + "-----");
  
  // Apply threshold to create a dead zone
  joy1XValue = abs(joy1XValue) > centerThreshold ? joy1XValue : 0;
  joy1YValue = abs(joy1YValue) > centerThreshold ? joy1YValue : 0;
  joy2XValue = abs(joy2XValue) > centerThreshold ? joy2XValue : 0;
  joy2YValue = abs(joy2YValue) > centerThreshold ? joy2YValue : 0;

  // Map joystick values to box positions
  joy1XPos = map(joy1XValue, -2048, 2048, joy1XBox - boxSize / 2, joy1XBox + boxSize / 2);
  joy1YPos = map(joy1YValue, -2048, 2048, joy1YBox - boxSize / 2, joy1YBox + boxSize / 2);
  joy2XPos = map(joy2XValue, -2048, 2048, joy2XBox - boxSize / 2, joy2XBox + boxSize / 2);
  joy2YPos = map(joy2YValue, -2048, 2048, joy2YBox - boxSize / 2, joy2YBox + boxSize / 2);

}  

void OledDisplaySend(){
  unsigned long startTime = micros();
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_t0_18b_tr);
  u8g2.drawStr(0, 13, connectOk ? "CONN OK" : "ERROR");
  
  u8g2.drawXBMP(68, 3, 11, 8, image_GameMode_bits);
  u8g2.drawXBMP(81, 3, 16, 8, image_Bluetooth_Connected_bits);
  u8g2.setFont(u8g2_font_4x6_tr);
  String v1 = "v:" + String(recvData.b);
  String v2 = "v:" + String(recvData.c);
  u8g2.drawStr(100, 7, v1.c_str());
  u8g2.drawStr(100, 13, v2.c_str());
  u8g2.drawLine(0, 15, 128, 15);
  u8g2.updateDisplayArea(0, 0, 16, 2);
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvB08_tr);
    for(uint8_t i = 0; i < 15; i++){
    sendData.stat[i] = !digitalRead(button[i]);
    if(sendData.stat[i]){
      switch(i){
        case 0 : u8g2.drawStr(11, 28, "------L3         --------"); break;
        case 1 : u8g2.drawStr(11, 28, "--------         ------R3"); break;
        case 2 : u8g2.drawStr(11, 28, "---L2---         --------"); break;
        case 3 : u8g2.drawStr(11, 28, "L1------         --------"); break;
        case 4 : u8g2.drawXBMP(19, 45, 5, 7, image_arrow_up_bits); break;
        case 5 : u8g2.drawXBMP(13, 51, 7, 5, image_arrow_left_bits); break;
        case 6 : u8g2.drawXBMP(19, 55, 5, 7, image_arrow_down_bits); break;
        case 7 : u8g2.drawXBMP(23, 51, 7, 5, image_arrow_right_bits); break;
        case 8 : u8g2.drawXBMP(98, 50, 4, 7, image_ButtonLeft_bits); break;
        case 9 : u8g2.drawXBMP(103, 58, 8, 4, image_InfraredArrowDown_bits); break;
        case 10 : u8g2.drawXBMP(112, 50, 4, 7, image_ButtonRight_bits); break;
        case 11 : u8g2.drawXBMP(103, 45, 8, 4, image_InfraredArrowUp_bits); break;
        case 12 : u8g2.drawStr(11, 28, "--------         ---R2---"); break;
        case 13 : u8g2.drawStr(11, 28, "--------         R1------"); break;
        default : u8g2.drawStr(11, 28, "--------         --------"); break;
      }
    }
  }
  u8g2.drawStr(11, 28, "--------         --------");
  
  
  u8g2.setFont(u8g2_font_haxrcorp4089_tr);
  u8g2.drawStr(57, 28, sendData.stat[14] ? "OFF" : "ON");
  
  u8g2.drawFrame(11, 43, 21, 21);
  u8g2.drawFrame(96, 42, 22, 22);
  u8g2.drawFrame(joy1XBox - boxSize / 2, joy1YBox - boxSize / 2, boxSize, boxSize);
  u8g2.drawFrame(joy2XBox - boxSize / 2, joy2YBox - boxSize / 2, boxSize, boxSize);
  u8g2.drawDisc(joy1XPos-1, joy1YPos, 3, U8G2_DRAW_ALL);
  u8g2.drawDisc(joy2XPos-1, joy2YPos, 3, U8G2_DRAW_ALL);
  
  u8g2.updateDisplayArea(0, 2, 16, 6);
  
  unsigned long endTime = micros(); // Record end time
  unsigned long elapsedTime = endTime - startTime; // Calculate elapsed time
  
  Serial.print("Time to send data: ");
  Serial.print(elapsedTime);
  Serial.println(" microseconds");
}

//==================================================VOID SETUP============================================//
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  u8g2.begin();

  for(uint8_t i = 0; i < 15; i++){
    pinMode(button[i], INPUT_PULLUP);
  }
    
  pinMode(18, OUTPUT);
  pinMode(5, OUTPUT);
  
  calibrateJoysticks();
  
  startSend();
  startRecv();
  
  OledDisplaySend();
}
 
void loop() {
  dataSent();
  joystickRead();
  OledDisplaySend();
  delay(10);
}
