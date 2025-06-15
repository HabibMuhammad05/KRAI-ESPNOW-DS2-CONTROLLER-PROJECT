void ledBlink(int wait){
  static uint32_t blinkMillis;
  if(millis() - blinkMillis >= wait){
    blinkMillis=millis();
    digitalWrite(5, !digitalRead(5));    
  }
}

//========================================ESPNOW DATA SENDING FUNCTIONS======================================//
bool connectOk = false;
uint8_t connStat = false;
unsigned long lastActiveTime = 0;      
const unsigned long timeout = 1000;    
bool isSending = false; 

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {  
  DEBUG_PRINT("----Last Packet Send Status:");
  connectOk = (status == ESP_NOW_SEND_SUCCESS);
  if(connectOk){
    DEBUG_PRINT("Delivery Success----");
  }else{
    DEBUG_PRINT("Delivery Fail----");
  }
}

void startSend(){
  if (esp_now_init() != ESP_OK) { DEBUG_PRINTLN("Error initializing ESP-NOW"); return; }
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress[macIndex], 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;      
  if (esp_now_add_peer(&peerInfo) != ESP_OK){ DEBUG_PRINTLN("Failed to add peer"); return; }
}

void dataSent() {
    unsigned long currentTime = millis();
    bool isButtonPressed = false;      
    bool isJoystickMoved = false;      

    for (uint8_t i = 0; i < 14; i++) {
        sendData.stat[i] = digitalRead(button[i]); 
        if (sendData.stat[i] == LOW){
            isButtonPressed = true;  
            DEBUG_PRINT("Button "); DEBUG_PRINT(i); DEBUG_PRINTLN(" is pressed.");
        }
    }
    sendData.stat[14] = !digitalRead(button[14]); 

    for (uint8_t i = 0; i < 4; i++) {
        if (sendData.joyData[i] != 0) {           
            isJoystickMoved = true;              
            DEBUG_PRINT("Joystick axis "); DEBUG_PRINT(i); DEBUG_PRINT(" moved.");
            DEBUG_PRINT(" -val: "); DEBUG_PRINT(sendData.joyData[i]); DEBUG_PRINTLN("  - ");
        }
    }
    if (isButtonPressed || isJoystickMoved) {
        lastActiveTime = currentTime;            
        isSending = true;                        
    } else if (currentTime - lastActiveTime >= timeout) {
        isSending = false;                       
    }

    if (isSending) { 
        static uint32_t pM;
        uint32_t cM = millis();
        if (cM - pM > 45) {
            esp_err_t result = esp_now_send(broadcastAddress[macIndex], (uint8_t*)&sendData, sizeof(sendData));
            connStat = (connectOk ? 2 : 0);
            if (result == ESP_OK) DEBUG_PRINTLN("Sent with success!");
            else DEBUG_PRINTLN("Error sending the data.");
            pM = cM;   
        }
        ledBlink(250);
    } 
    else {
        DEBUG_PRINTLN("No activity, no data sent.");
        connStat = 1;
        ledBlink(1000);
    }
}

//========================================ESPNOW DATA RECEIVING FUNCTIONS======================================//
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingLocal, int len) {
  memcpy(&incomingData, incomingLocal, sizeof(incomingData));
  DEBUG_PRINT("Bytes received: ");
  DEBUG_PRINTLN(len);
}

void startRecv(){
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}


//
//void dataSent(){  
//  for(uint8_t i = 0; i < 15; i++){
//    sendData.stat[i] = digitalRead(button[i]);
//    DEBUG_PRINT(sendData.stat[i]);
//    DEBUG_PRINT(", ");
//  }
//  DEBUG_PRINTLN();
//  
//  esp_err_t result = esp_now_send(broadcastAddress[macIndex], (uint8_t *) &sendData, sizeof(sendData));
//  if (result == ESP_OK) { DEBUG_PRINT("Sent with success---"); }
//  else { DEBUG_PRINT("Error sending the data---"); }
//}
