//========================================ESPNOW DATA SENDING FUNCTIONS======================================//
uint8_t connectOk = false;
unsigned long lastActiveTime = 0;      
const unsigned long timeout = 1000;    
bool isSending = false; 

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {  
  DEBUG_PRINT("----Last Packet Send Status:");
  connectOk = (status == ESP_NOW_SEND_SUCCESS);
  DEBUG_PRINT(connectOk ? "Delivery Success----" : "Delivery Fail----");
}

void startSend(){
  if (esp_now_init() != ESP_OK) { DEBUG_PRINTLN("Error initializing ESP-NOW"); return; }
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;      
  if (esp_now_add_peer(&peerInfo) != ESP_OK){ DEBUG_PRINTLN("Failed to add peer"); return; }
}

void dataSent() {
    unsigned long currentTime = millis();
    bool isButtonPressed = false;      
    bool isJoystickMoved = false;      

    for (uint8_t i = 0; i < 15; i++) {
        sendData.stat[i] = digitalRead(button[i]); 
        if (sendData.stat[i] == LOW){
            isButtonPressed = true;  
            DEBUG_PRINT("Button "); DEBUG_PRINT(i); DEBUG_PRINTLN(" is pressed.");
        }
    }

    for (uint8_t i = 0; i < 4; i++) {
        if (sendData.joyData[i] != 0) {           
            isJoystickMoved = true;              
            DEBUG_PRINT("Joystick axis "); DEBUG_PRINT(i); DEBUG_PRINTLN(" moved.");
        }
    }

    if (isButtonPressed || isJoystickMoved) {
        lastActiveTime = currentTime;            
        isSending = true;                        
    } else if (currentTime - lastActiveTime >= timeout) {
        isSending = false;                       
    }

    if (isSending) { 
        connectOk = (connectOk ? 2 : false);
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t*)&sendData, sizeof(sendData));
        if (result == ESP_OK) {
            DEBUG_PRINTLN("Sent with success!");
        } else {
            DEBUG_PRINTLN("Error sending the data.");
        }
    } else {
        DEBUG_PRINTLN("No activity, no data sent.");
    }
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
//  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &sendData, sizeof(sendData));
//  if (result == ESP_OK) { DEBUG_PRINT("Sent with success---"); }
//  else { DEBUG_PRINT("Error sending the data---"); }
//}
