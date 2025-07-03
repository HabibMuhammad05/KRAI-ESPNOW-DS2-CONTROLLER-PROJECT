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
