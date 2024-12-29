#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
    bool stat[15];
    int joyData[4];
//    char a[32];
//    int b;
//    float c;
//    bool d;
} struct_message;

struct_message recvData; //struct variable for receiving data.

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
  for(uint8_t i = 0; i < 15; i++){
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


void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  startRecv();
}

void loop() {
  
}
