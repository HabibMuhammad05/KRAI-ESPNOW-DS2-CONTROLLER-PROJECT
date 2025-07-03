#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[4][6] = {
  {0x80, 0x7D, 0x3A, 0xEA, 0xB1, 0x98}, // esp32 ext ant R1
  {0xE8, 0x6B, 0xEA, 0xD4, 0xA3, 0xA0}, // esp32 v1 microUSB 
  {0x80, 0x7D, 0x3A, 0xB9, 0x1F, 0xB4}, // remote2
  {0xA4, 0xCF, 0x12, 0x42, 0xAC, 0x50}  // remote39
};

typedef struct DatatoSend {
  int rpmData[2];
  float voltData[3];
  int pressureData;
} DatatoSend;
DatatoSend sendData; //struct to store sent data.
typedef struct struct_message {
    bool stat[15];
    int joyData[4];
    uint8_t remoteIndex = 1;
//    char address[18];
} struct_message;
struct_message incomingData; // struct to store received data
uint8_t storedMac;

// ======== ADD BATTERY FUNCTION ========
const float R1 = 10000.0;
const float R2 = 100000.0;
const int BATTERY_PIN0 = 36;

float readBatteryVoltage0() {
  int raw = analogRead(BATTERY_PIN0);
  float voltage = raw * (3.3 / 4095.0);
  return voltage * ((R1 + R2) / R2);
}
const float R3 = 10000.0;
const float R4 = 22000.0;
const int BATTERY_PIN1 = 39;

float readBatteryVoltage1() {
  int raw = analogRead(BATTERY_PIN1);
  float voltage = raw * (3.3 / 4095.0);
  return voltage * ((R3 + R4) / R4);
}
const float R5 = 22000.0;
const float R6 = 100000.0;
const int BATTERY_PIN2 = 34;

float readBatteryVoltage2() {
  int raw = analogRead(BATTERY_PIN2);
  float voltage = raw * (3.3 / 4095.0);
  return voltage * ((R5 + R6) / R6);
}

// ======== RPM SENSOR CONFIGURATION ========
#define RPM_SENSOR_PIN1 18  // Pin sensor RPM motor 1
#define RPM_SENSOR_PIN2 19  // Pin sensor RPM motor 2
#define HOLES_PER_DISK 20   // Jumlah lubang pada disk

// Variabel untuk perhitungan RPM
volatile unsigned long pulseCount1 = 0;
volatile unsigned long pulseCount2 = 0;
unsigned long lastPulseTime1 = 0;
unsigned long lastPulseTime2 = 0;
const unsigned long DEBOUNCE_TIME = 5; // Waktu debounce (ms)

// Mutex untuk akses aman ke variabel pulseCount
static portMUX_TYPE mux1 = portMUX_INITIALIZER_UNLOCKED;
static portMUX_TYPE mux2 = portMUX_INITIALIZER_UNLOCKED;

// Interrupt handler untuk sensor RPM
void IRAM_ATTR pulseInterrupt1() {
    static unsigned long lastInterruptTime1 = 0;
    unsigned long currentTime = millis();
    
    if (currentTime - lastInterruptTime1 > DEBOUNCE_TIME) {
        portENTER_CRITICAL_ISR(&mux1);
        pulseCount1++;
        lastPulseTime1 = currentTime;
        portEXIT_CRITICAL_ISR(&mux1);
        lastInterruptTime1 = currentTime;
    }
}

void IRAM_ATTR pulseInterrupt2() {
    static unsigned long lastInterruptTime2 = 0;
    unsigned long currentTime = millis();
    
    if (currentTime - lastInterruptTime2 > DEBOUNCE_TIME) {
        portENTER_CRITICAL_ISR(&mux2);
        pulseCount2++;
        lastPulseTime2 = currentTime;
        portEXIT_CRITICAL_ISR(&mux2);
        lastInterruptTime2 = currentTime;
    }
}

// ======== ADD FUNCTION DECLARATION ========
float readBatteryVoltage0();
float readBatteryVoltage1();
float readBatteryVoltage2();


//========================================ESPNOW DATA RECEIVING FUNCTIONS======================================//


void OnDataRecv(const uint8_t *mac, const uint8_t *incomingLocal, int len) {
    memcpy(&incomingData, incomingLocal, sizeof(incomingData));
    String dataLine = String();
    for (int i = 0; i < 4; i++) {  dataLine += String(incomingData.joyData[i]) + ","; }
    for (int i = 0; i < 15; i++) { dataLine += String(incomingData.stat[i]) + ",";    }
    
    dataLine += String(incomingData.remoteIndex);
//    dataLine += incomingData.address;
//    dataLine.remove(dataLine.length() - 1); 

    Serial2.println(dataLine);
    DEBUG_PRINTLN("Serial Sent Data: " + dataLine);
    check_EEPROM();
}

//========================================ESPNOW DATA SENDING FUNCTIONS======================================//


//esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {  
//  DEBUG_PRINT("----Last Packet Send Status:");
  bool connectOk = (status == ESP_NOW_SEND_SUCCESS);
  if(connectOk){
    DEBUG_PRINTLN("Delivery Success");
  }else{
    DEBUG_PRINTLN("Delivery Fail");
  }
}

void startComms(){
  if (esp_now_init() != ESP_OK) { DEBUG_PRINTLN("Error initializing ESP-NOW"); return; }
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
  
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress[incomingData.remoteIndex], 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;      
  esp_now_add_peer(&peerInfo);
//  String macStr = macToString(broadcastAddress[incomingData.remoteIndex]);
//  DEBUG_PRINT("Sending Data To: ");  DEBUG_PRINTLN(macStr);
}

void dataSent() {  
    static uint32_t pM;
    static unsigned long lastRPMTime = 0;
    const unsigned long RPMInterval = 100; // Interval pembacaan RPM (ms)
    uint32_t cM = millis();

        // Hitung RPM setiap interval
    if (cM - lastRPMTime >= RPMInterval) {
        unsigned long count1, count2;
        unsigned long timeDiff1 = 0, timeDiff2 = 0;
        
        // Baca dan reset pulseCount dengan aman
        portENTER_CRITICAL(&mux1);
        count1 = pulseCount1;
        pulseCount1 = 0;
        if (lastPulseTime1 > 0) {
            timeDiff1 = cM - lastPulseTime1;
        }
        portEXIT_CRITICAL(&mux1);
        
        portENTER_CRITICAL(&mux2);
        count2 = pulseCount2;
        pulseCount2 = 0;
        if (lastPulseTime2 > 0) {
            timeDiff2 = cM - lastPulseTime2;
        }
        portEXIT_CRITICAL(&mux2);
        
        // Hitung RPM berdasarkan frekuensi
        if (timeDiff1 > 0) {
            float frequency1 = (count1 * 1000.0) / timeDiff1;
            sendData.rpmData[0] = (frequency1 * 60) / HOLES_PER_DISK;
        } else {
            sendData.rpmData[0] = 0;
        }
        
        if (timeDiff2 > 0) {
            float frequency2 = (count2 * 1000.0) / timeDiff2;
            sendData.rpmData[1] = (frequency2 * 60) / HOLES_PER_DISK;
        } else {
            sendData.rpmData[1] = 0;
        }
        
        lastRPMTime = cM;
    }
    
    if (cM - pM > 2000) {
       String dataStr = String();
       sendData.voltData[0] = readBatteryVoltage0();
       sendData.voltData[1] = readBatteryVoltage1();
       sendData.voltData[2] = readBatteryVoltage2();
       sendData.pressureData = random(0, 600);

       for (int i = 0; i < 2; i++) {  dataStr += String(sendData.rpmData[i]) + ","; }
       for (int i = 0; i < 3; i++) {  dataStr += String(sendData.voltData[i]) + ","; }
       dataStr += String(sendData.pressureData) + " - ";
       DEBUG_PRINT(dataStr);
       esp_err_t result = esp_now_send(broadcastAddress[incomingData.remoteIndex], (uint8_t*)&sendData, sizeof(sendData));
       if (result == ESP_OK) DEBUG_PRINT("SENT SUCCESS - ");
       else DEBUG_PRINT("ERROR - "); 
       DEBUG_PRINTLN(storedMac); 
//       String macStr = macToString(broadcastAddress[incomingData.remoteIndex]);
//       DEBUG_PRINTLN(macStr);
       pM = cM;    
    }
}

void failSafeCheck(struct_message &recvData) {
    static struct_message lastReceivedData;
    static unsigned long lastReceiveTime = 0;
    static bool failsafeTriggered = false;

    bool dataChanged = false;
    for (int i = 0; i < 4; i++) {
        if (recvData.joyData[i] != lastReceivedData.joyData[i]) {
            dataChanged = true;
            break;
        }
    }
    if (!dataChanged) {
        for (int i = 0; i < 15; i++) {
            if (recvData.stat[i] != lastReceivedData.stat[i]) {
                dataChanged = true;
                break;
            }
        }
    }
    if (dataChanged) {
        lastReceiveTime = millis();
        lastReceivedData = recvData; 
        failsafeTriggered = false;
    }
    if (!failsafeTriggered && (millis() - lastReceiveTime >= 1000)) {
        bool joystickMoved = false;
        bool buttonPressed = false;
        for (int i = 0; i < 4; i++) {
            if (lastReceivedData.joyData[i] != 0) {
                joystickMoved = true;
                break;
            }
        }
        for (int i = 0; i < 15; i++) {
            if (!lastReceivedData.stat[i]) {
                buttonPressed = true;
                break;
            }
        }
        if (joystickMoved || buttonPressed) {
            for (int i = 0; i < 4; i++) recvData.joyData[i] = 0;
            for (int i = 0; i < 15; i++) recvData.stat[i] = true;
            String dataLine;
            
            for (int i = 0; i < 4; i++)  dataLine += String(recvData.joyData[i]) + ",";
            for (int i = 0; i < 15; i++) dataLine += String(recvData.stat[i]) + ",";
            dataLine += String(recvData.remoteIndex);
            Serial2.println(dataLine);
            DEBUG_PRINTLN("FAILSAFE: RESETTING THE SERIAL DATA");
            DEBUG_PRINTLN("Serial Sent Data: " + dataLine);

            failsafeTriggered = true;
        }
    }
}









//String macToString(const uint8_t *mac) {
//  char macChr[18];
//  sprintf(macChr, "%02X:%02X:%02X:%02X:%02X:%02X",
//          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
//  return String(macChr);
//}
