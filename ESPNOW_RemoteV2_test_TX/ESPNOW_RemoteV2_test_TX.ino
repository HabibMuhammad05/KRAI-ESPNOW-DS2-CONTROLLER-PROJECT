#include <esp_now.h>
#include <WiFi.h>

unsigned long previousMillis = 0;

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xD0, 0xEF, 0x76, 0x34, 0x7A, 0xC0};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

bool connectOk = false;
// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  connectOk = (status == ESP_NOW_SEND_SUCCESS);
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  pinMode(23, OUTPUT);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // Set values to send
  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1,20);
  myData.c = 1.2;
  myData.d = false;
  
  TonePlay();
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(1000);
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
