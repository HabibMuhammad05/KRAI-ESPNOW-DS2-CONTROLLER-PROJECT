// ============================================= BATTERY READING VARS ======================================

#ifdef ROBOT1           //ROBOT 1
  float calib3 = 1.05;
  float calib4 = 1;
  float calib6 = 1.06;
#else                   //ROBOT 2
  float calib3 = 1.1;
  float calib4 = 1.05;
  float calib6 = 1.125;
#endif

uint8_t pin3S = 36;
uint8_t pin4S = 34;
uint8_t pin6S = 39;

float div3S = 22.0 / 122.0;
float div4S = 22.0 / 122.0;
float div6S = 10.0 / 110.0;
float readings3S[15], readings4S[15], readings6S[15];
uint8_t idx3S = 0, idx4S = 0, idx6S = 0;
unsigned long readMil = 0;

void batteryRead(){
  if (millis() - readMil >= 100) {
    readMil = millis();
    float vOut3 = analogRead(pin3S) * 3.3 / 4095.0;
    float vOut4 = analogRead(pin4S) * 3.3 / 4095.0;
    float vOut6 = analogRead(pin6S) * 3.3 / 4095.0;

    readings3S[idx3S] = (vOut3 / div3S) * calib3;
    readings4S[idx4S] = (vOut4 / div4S) * calib4;
    readings6S[idx6S] = (vOut6 / div6S) * calib6;
    idx3S = (idx3S + 1) % 10;
    idx4S = (idx4S + 1) % 10;
    idx6S = (idx6S + 1) % 10;

    if (idx3S == 0 && idx4S == 0 && idx6S == 0) {
      float sum3 = 0, sum4 = 0, sum6 = 0;
      for (int i = 0; i < 10; i++) {
        sum3 += readings3S[i];
        sum4 += readings4S[i];
        sum6 += readings6S[i];
      }
      sendData.voltData[0] = sum3 / 10.0;
      sendData.voltData[1] = sum4 / 10.0;
      sendData.voltData[2] = sum6 / 10.0;

      DEBUG_PRINT("3S: "); DEBUG_PRINT(sendData.voltData[0], 2); DEBUG_PRINT(" V | ");
      DEBUG_PRINT("4S: "); DEBUG_PRINT(sendData.voltData[1], 2); DEBUG_PRINT(" V | ");
      DEBUG_PRINT("6S: "); DEBUG_PRINT(sendData.voltData[2], 2); DEBUG_PRINTLN(" V");
    }
  }
}

// ============================================== ENCODER READING FUNC =====================================
const byte encPin1 = 23, encPin2 = 22;
const float correction = 3.08;
const int sampleMs = 300, idleMs = 400, avgCount = 10;

struct Encoder {
  volatile unsigned int count = 0;
  unsigned long lastPulse = 0, lastSample = 0;
  float buffer[avgCount] = {0};
  int i = 0;
  float rpm = 0;
  bool active = false;
} enc1, enc2;

void IRAM_ATTR isr1() { enc1.count++; enc1.lastPulse = millis(); }
void IRAM_ATTR isr2() { enc2.count++; enc2.lastPulse = millis(); }

void handleEncoders() {
  Encoder* e[2] = { &enc1, &enc2 };
  const char* label[2] = { "ENC1", "ENC2" };

  for (int n = 0; n < 2; n++) {
    unsigned long now = millis();
    if (now - e[n]->lastPulse > idleMs && e[n]->active) {
      e[n]->active = false; 
      e[n]->rpm = 0;
      encoderOn = false;
      DEBUG_PRINT(label[n]); DEBUG_PRINTLN(": 0 RPM (idle)");
    }

    if ((now - e[n]->lastSample >= sampleMs) && (e[n]->active || e[n]->count > 0)) {
      e[n]->lastSample = now;
      noInterrupts(); unsigned int p = e[n]->count; e[n]->count = 0; interrupts();
      float r = p * (60000.0 / sampleMs) * correction;
      if (r > 800) e[n]->active = true;

      e[n]->buffer[e[n]->i] = r; e[n]->i = (e[n]->i + 1) % avgCount;
      float sum = 0; for (int j = 0; j < avgCount; j++) sum += e[n]->buffer[j];
      e[n]->rpm = sum / avgCount;

      sendData.rpmData[n] = e[n]->rpm;
      
      encoderOn = true;
      DEBUG_PRINT(label[n]); DEBUG_PRINT(": ");
      DEBUG_PRINT(e[n]->rpm, 0);DEBUG_PRINTLN(" RPM");
    }
    else{
      encoderOn = false;
    }
  }
}

uint8_t rRelay = 18, gRelay = 5, bRelay = 19;

void handleLED(){
  static uint32_t redMil;
  static uint32_t greenMil;
  static uint32_t blueMil;
  
  if(failsafeTriggered){
   if (millis() - redMil >= 50) {
      redMil = millis();
      digitalWrite(rRelay, !digitalRead(rRelay));
    }
  }
  else if(connectOk){   
    if (millis() - redMil >= 2000) {
       redMil = millis();
       digitalWrite(rRelay, !digitalRead(rRelay));
    }
  }
  else{
    if (millis() - redMil >= 500) {
       redMil = millis();
       digitalWrite(rRelay, !digitalRead(rRelay));
    }
  }


 
  if((enc1.rpm > 15000) || (enc2.rpm > 15000)){
   if (millis() - greenMil >= 100) {
       greenMil = millis();
       digitalWrite(gRelay, !digitalRead(gRelay));
    }
  } 
  else if(enc1.active || enc2.active){
   if (millis() - greenMil >= 1000) {
       greenMil = millis();
       digitalWrite(gRelay, !digitalRead(gRelay));
    }
  }
  else{
    digitalWrite(gRelay, HIGH);
  }



  if((incomingData.joyData[0] > 550) || (incomingData.joyData[1] > 550) || (incomingData.joyData[2] > 550) ||
     (incomingData.joyData[0] < -550) || (incomingData.joyData[1] < -550) || (incomingData.joyData[2] < -550)){
//    if (millis() - blueMil >= 1000) {
//       blueMil = millis();
       digitalWrite(bRelay, LOW);
//    }
  }
  else{
    digitalWrite(bRelay, HIGH);
  }
  
}
