#include <esp_now.h>
#include <WiFi.h>

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
//  strcpy(sendData.a, "THIS IS A CHAR");
//  sendData.b = random(1,20);
//  sendData.c = random(20,60);
//  sendData.d = random(0,1);
//  Serial.print("---Sent : ");
//  Serial.print(sendData.a);
//  Serial.print("--");
//  Serial.print(sendData.b);
//  Serial.print("--");
//  Serial.print(sendData.c);
//  Serial.print("--");
//  Serial.print(sendData.d);
//  Serial.print("----");
  
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
