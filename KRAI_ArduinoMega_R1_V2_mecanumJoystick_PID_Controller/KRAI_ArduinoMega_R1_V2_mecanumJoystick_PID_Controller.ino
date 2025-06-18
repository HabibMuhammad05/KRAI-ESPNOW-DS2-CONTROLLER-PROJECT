/*---------------------------------------------------------------------------------------------------------*/
/*--------------------ARDUINO MEGA PARSING DATA FROM ESP32, MECANUM WHEEL WITH JOYSTICK--------------------*/
/*--------------------INCLUDING INTERNAL ENCODER FEEDBACK WITH PID CLOSED LOOP FEEDBACK--------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*--------------------------------------Source Code by LEXARGA-24 TEAM-------------------------------------*/
/*-----------------------------------Modified & Adapted by LEXARGA-24 TEAM---------------------------------*/
/*----------------------------------------------------V3.1-------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*------------------------------------LAST UPDATE AT 13:20:00, 13 JUNE 25-----------------------------------*/

// Define DEBUG to enable debugging; comment it out to disable
// #define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
#define DEBUG_BEGIN(baud) Serial.begin(baud)
#else
#define DEBUG_PRINT(...)
#define DEBUG_PRINTLN(...)
#define DEBUG_BEGIN(baud)
#endif

#include "dataReadFunc.h"
#include "mecanumControl.h"
#include "shooting.h"

unsigned long relay5StartTime = 0;
bool relay5Active = false;

bool R2lastState = true;
bool L2lastState = true;
bool R1lastState = true;
bool L1lastState = true;

uint8_t relay1 = 46;
uint8_t relay2 = 48;
uint8_t relay3 = 50;
uint8_t relay4 = 52;
uint8_t relay5 = 34;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(relay1, OUTPUT);
  digitalWrite(relay1, LOW);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay2, HIGH);
  pinMode(relay3, OUTPUT);
  digitalWrite(relay3, LOW);
  pinMode(relay4, OUTPUT);
  digitalWrite(relay4, LOW);
  pinMode(relay5, OUTPUT);
  digitalWrite(relay5, LOW);
  recvStart();
  PIDSetup();
  mecanumSetup();
  shootingSetup();
}

void loop() {
  checkData();
  delay(30);
  calcMecanum();
  relayCheck();
  shootingTask();
}

void relayCheck() {
  unsigned long currentMillis = millis();
  if (!recvData.stat[12] && R2lastState) {  // R2
    digitalWrite(relay2, !digitalRead(relay2));
    delay(20);

    //Mengaktifkan Relay1 Sesaat
    digitalWrite(relay1, LOW);
    delay(100);
    digitalWrite(relay1, HIGH);
  }
  R2lastState = recvData.stat[12];

  if (!recvData.stat[2] && L2lastState) {  //L2
    digitalWrite(relay2, !digitalRead(relay2));
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  L2lastState = recvData.stat[2];

  if (!recvData.stat[13] && R1lastState) {  //R1
    digitalWrite(relay3, !digitalRead(relay3));
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  R1lastState = recvData.stat[13];

  // Cek L1
  if (!recvData.stat[3] && L1lastState) {  // L1 berubah
    digitalWrite(relay4, !digitalRead(relay4));

    // Aktifkan relay5 sesaat (HIGH lalu LOW)
    digitalWrite(relay5, HIGH);
    relay5Active = true;
    relay5StartTime = currentMillis;
  } else if (!recvData.stat[3] && L1lastState) {  // L1 berubah
    digitalWrite(relay4, !digitalRead(relay4));

    // Aktifkan relay5 sesaat (HIGH lalu LOW)
    digitalWrite(relay5, HIGH);
    relay5Active = true;
    relay5StartTime = currentMillis;
  }
  L1lastState = recvData.stat[3];

  // Matikan relay5 setelah 2000 ms jika sedang aktif
  if (relay5Active && (currentMillis - relay5StartTime >= 1000)) {
    digitalWrite(relay5, LOW);
    relay5Active = false;
  }
}




// if (!recvData.stat[3] && L1lastState) {  //L1
//   digitalWrite(relay4, !digitalRead(relay4));
//   digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

// }
// L1lastState = recvData.stat[3];



// if (!recvData.stat[12] && R2lastState) { //R2
//   digitalWrite(relay2, !digitalRead(relay2));
//   delay(100);
//   digitalWrite(relay1, !digitalRead(relay1));
//   digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
// }