/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0x10, 0x06, 0x1C, 0x341, 0xC2, 0xD0};

typedef struct struct_message {
    bool stat[14];
    int joyData[4];
    char a[32];
    int b;
    float c;
    bool d;
} struct_message;

struct_message recvData; //struct variable for receiving data.

struct_message sendData; //struct variable for sending data.

//========================================ESPNOW DATA RECEIVING FUNCTIONS======================================//
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&recvData, incomingData, sizeof(recvData));
  Serial.print("Bytes received: ");
  Serial.print(len);
//  Serial.print("Char: ");
//  Serial.println(recvData.a);
//  Serial.print("Int: ");
//  Serial.println(recvData.b);
//  Serial.print("Float: ");
//  Serial.println(recvData.c);
//  Serial.print("Bool: ");
//  Serial.println(recvData.d);
  Serial.print("----");
  
  Serial.print(String(recvData.joyData[0]) + ", " + String(recvData.joyData[1]) + ", " + String(recvData.joyData[2]) + ", " + String(recvData.joyData[3]) + "-----");
  Serial.print("-----");
  for(uint8_t i = 0; i < 14; i++){
    Serial.print(recvData.stat[i]);
    Serial.print(", ");
  }
  Serial.println();
}

void startRecv(){
  if (esp_now_init() != ESP_OK) { 
    Serial.println("Error initializing ESP-NOW"); 
    return; 
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}
//================================================END OF FUNCTIONS=============================================//

//========================================ESPNOW DATA SENDING FUNCTIONS======================================//
esp_now_peer_info_t peerInfo;
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {  // callback when data is sent
//  Serial.print("\r---Last Packet Send Status: ");
//  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
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
  sendData.b = random(3,48);
  sendData.c = random(1,9);
  sendData.d = random(0,1);
//  Serial.print("----Sent : ");
//  Serial.print(sendData.a);
//  Serial.print("--");
//  Serial.print(sendData.b);
//  Serial.print("--");
//  Serial.print(sendData.c);
//  Serial.print("--");
//  Serial.print(sendData.d);
//  Serial.println();
  
  
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &sendData, sizeof(sendData));
//  if (result == ESP_OK) { Serial.print("---Sent with success"); }
//  else { Serial.print("---Error sending the data"); }
}
//=============================================END OF FUNCTIONS===============================================//

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  startRecv();
  startSend();
}
 
void loop() {
  dataSent();
  delay(50);

}
