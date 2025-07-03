/*---------------------------------------------------------------------------------------------------------*/
/*--------------------ARDUINO MEGA PARSING DATA FROM ESP32, MECANUM WHEEL WITH JOYSTICK--------------------*/
/*--------------------INCLUDING INTERNAL ENCODER FEEDBACK WITH PID CLOSED LOOP FEEDBACK--------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*--------------------------------------Source Code by LEXARGA-24 TEAM-------------------------------------*/
/*-----------------------------------Modified & Adapted by LEXARGA-24 TEAM---------------------------------*/
/*----------------------------------------------------V2.0-------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
/*------------------------------------LAST UPDATE AT 14:50:00, 7 JUN 25-----------------------------------*/

// Define DEBUG to enable debugging; comment it out to disable
#define DEBUG

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

bool R2lastState = true;
uint8_t relay1 = 46;

bool L2lastState = true;
uint8_t relay2 = 48;

//idc 5 connector, AUX
uint8_t M5_A = 33;
uint8_t M5_B = 31;
uint8_t pwm5 = 8;
//idc 6 connector, AUX2
uint8_t M6_A = 29;
uint8_t M6_B = 27;
uint8_t pwm6 = 9;

uint8_t topLimit = 32;
uint8_t bottomLimit = 33;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  
  pinMode(topLimit, INPUT_PULLUP);
  pinMode(bottomLimit, INPUT_PULLUP);
  recvStart();
  PIDSetup();
  mecanumSetup();
}

void loop(){
  checkData();
  delay(30);
  calcMecanum();
  ballThrow();
  handleLifter();
 
}

void handleLifter(){
  if (!recvData.stat[4] && digitalRead(topLimit)) { // index 4 = up button
    analogWrite(pwm6, 70);
    digitalWrite(M6_A, HIGH);
    digitalWrite(M6_B, LOW);
  }
  if (!recvData.stat[6] && digitalRead(bottomLimit)) {  // index 6 = down button
    analogWrite(pwm6, 70);
    digitalWrite(M6_A, LOW);
    digitalWrite(M6_B, HIGH);
  }  
}

void ballThrow(){
  //=============================== turn on/off relay at D30 when R2 is pressed ===============================//
  //========================================== ball thrower feeder ============================================//
  if (!recvData.stat[12] && R2lastState) {  // index 12 = R2 button
    digitalWrite(relay1, !digitalRead(relay1)); 
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); 
  }
  R2lastState = recvData.stat[12];

  
  int pwmVal = mapJoystick(recvData.joyData[3], -4095, 4095, 255, -255);
  if (pwmVal > 0){
    analogWrite(pwm5, pwmVal);
    digitalWrite(M5_A, HIGH);
    digitalWrite(M5_B, LOW);
  }
  else if (pwmVal < -30){
    analogWrite(pwm5, 255);
    digitalWrite(M5_A, HIGH);
    digitalWrite(M5_B, HIGH);
  }
  else{
    analogWrite(pwm5, 0);
    digitalWrite(M5_A, HIGH);
    digitalWrite(M5_B, HIGH);
  }
}





//=============================== turn on/off relay at D32 when R2 is pressed ===============================//
//  if (!recvData.stat[2] && L2lastState) { 
//    digitalWrite(relay2, !digitalRead(relay2));
//    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
//  }
//  L2lastState = recvData.stat[2];
