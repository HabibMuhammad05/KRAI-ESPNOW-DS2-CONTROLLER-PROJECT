//======================================= MOTOR DRIVER PINS =======================================//
//idc 1 connector, FRONT LEFT
uint8_t FL_A = 49;
uint8_t FL_B = 47;
uint8_t pwm1 = 8;

//idc 2 connector, FRONT RIGHT
uint8_t FR_A = 43;
uint8_t FR_B = 45;
uint8_t pwm2 = 9;

//idc 3 connector, REAR LEFT
uint8_t RL_A = 41;
uint8_t RL_B = 39;
uint8_t pwm3 = 4;

//idc 4 connector, REAR RIGHT
uint8_t RR_A = 35;
uint8_t RR_B = 37;
uint8_t pwm4 = 5;


//========================================= ENCODER PINS =========================================//
uint8_t ENCA_FL = 21;
uint8_t ENCB_FL = 28;
uint8_t ENCA_FR = 20;
uint8_t ENCB_FR = 26;
uint8_t ENCA_RL = 3;
uint8_t ENCB_RL = 24;
uint8_t ENCA_RR = 2;
uint8_t ENCB_RR = 22;


//=================================== INSERT PINS INTO AN ARRAY ==================================//
const int motorA[4] = {FL_A, FR_A, RL_A, RR_A};       //driver motor A pins as an array
const int motorB[4] = {FL_B, FR_B, RL_B, RR_B};       //driver motor B pins as an array
const int motorPWM_Pin[4] = {pwm1, pwm2, pwm3, pwm4}; //driver motor PWM pins as an array


//================================== PID CALCULATION VARIABLES ===================================//
float rpmPlus = 150.0;                        //max CW RPM for the motors
float rpmMin = -150.0;                        //max CCW RPM for the motors

uint8_t maxPWM = 200;                         //amount of max PWM sent to motor driver
    
const int encoderPPR = 326;                   //PG45 = 17PPR @1:19.2 Ratio = 17x19.2 ~ 326
const unsigned long sampleTime = 25;          //PID Calculation Interval
const unsigned long debugInterval = 1000;     //Serial Print Debug interval, avoid excessive RAM&Overflow

volatile long encoderCount[4] = {0, 0, 0, 0}; //num of Encoder
float currentRPM[4] = {0.0, 0.0, 0.0, 0.0};   //Encoder RPM Reading
float targetRPM[4] = {0.0, 0.0, 0.0, 0.0};    //Target RPM as reference for PID
float error[4] = {0.0, 0.0, 0.0, 0.0};        //PID - Proportional
float lastError[4] = {0.0, 0.0, 0.0, 0.0};    //PID - Derivative
float integral[4] = {0.0, 0.0, 0.0, 0.0};     //PID - Integral
float Kp = 0.45, Ki = 0.8, Kd = 0.01;         //PID Times Factor
int motorPWM[4] = {0, 0, 0, 0};               //Amount of PWM sent to driver
unsigned long lastTime[4] = {0, 0, 0, 0};     //store millis for calculation Interval
unsigned long lastDebugTime = 0;              //store millis for Debug Interval


//==================================== ENCODER ISR FUNCTIONS =====================================//
void readEncoder_FL() {
  if (digitalRead(ENCB_FL)) encoderCount[0]++;
  else encoderCount[0]--;
}

void readEncoder_FR() {
  if (digitalRead(ENCB_FR)) encoderCount[1]--;
  else encoderCount[1]++;
}

void readEncoder_RL() {
  if (digitalRead(ENCB_RL)) encoderCount[2]++;
  else encoderCount[2]--;
}

void readEncoder_RR() {
  if (digitalRead(ENCB_RR)) encoderCount[3]--;
  else encoderCount[3]++;
}


//==========================Encoder Pins declaration & Interrupt Setup============================//
void PIDSetup(){
  pinMode(ENCA_FL, INPUT_PULLUP);
  pinMode(ENCB_FL, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCA_FL), readEncoder_FL, RISING);
  pinMode(ENCA_FR, INPUT_PULLUP);
  pinMode(ENCB_FR, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCA_FR), readEncoder_FR, RISING);
  pinMode(ENCA_RL, INPUT_PULLUP);
  pinMode(ENCB_RL, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCA_RL), readEncoder_RL, RISING);
  pinMode(ENCA_RR, INPUT_PULLUP);
  pinMode(ENCB_RR, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCA_RR), readEncoder_RR, RISING); 
}


//===================================PID Feedback Calculation=====================================//
void calcPID(){
  for (int motorIndex = 0; motorIndex < 4; motorIndex++) {
    
    unsigned long startTime = micros();
    if (millis() - lastTime[motorIndex] >= sampleTime) {
      lastTime[motorIndex] = millis();
  
      currentRPM[motorIndex] = (encoderCount[motorIndex] / (float)encoderPPR) * (60000.0 / sampleTime);
      encoderCount[motorIndex] = 0;
      error[motorIndex] = targetRPM[motorIndex] - currentRPM[motorIndex];
      integral[motorIndex] += error[motorIndex] * (sampleTime / 1000.0);
      float derivative = (error[motorIndex] - lastError[motorIndex]) / (sampleTime / 1000.0);
      lastError[motorIndex] = error[motorIndex];
  
      motorPWM[motorIndex] = Kp * error[motorIndex] + Ki * integral[motorIndex] + Kd * derivative;
      
      if(targetRPM[motorIndex] == 0.0) {
        analogWrite(motorPWM_Pin[motorIndex], 0);
        digitalWrite(motorA[motorIndex], LOW);
        digitalWrite(motorB[motorIndex], LOW);
      }else if (motorPWM[motorIndex] > 0) {
        motorPWM[motorIndex] = constrain(motorPWM[motorIndex], 0, maxPWM);
        analogWrite(motorPWM_Pin[motorIndex], motorPWM[motorIndex]);
        digitalWrite(motorA[motorIndex], HIGH);
        digitalWrite(motorB[motorIndex], LOW);
      }else {
        motorPWM[motorIndex] = constrain(abs(motorPWM[motorIndex]), 0, maxPWM);
        analogWrite(motorPWM_Pin[motorIndex], motorPWM[motorIndex]);
        digitalWrite(motorA[motorIndex], LOW);
        digitalWrite(motorB[motorIndex], HIGH);
      }
    }
    unsigned long computationTime = micros() - startTime;
    DEBUG_PRINT("Motor ");
    DEBUG_PRINT(motorIndex);
    DEBUG_PRINT(" PID computation time: ");
    DEBUG_PRINT(computationTime);
    DEBUG_PRINTLN(" us");
  }
}


//===================================driver motor declaration=====================================//
void mecanumSetup(){
  pinMode(pwm1, OUTPUT);
  pinMode(FL_A, OUTPUT);
  pinMode(FL_B, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(FR_A, OUTPUT);
  pinMode(FR_B, OUTPUT);
  pinMode(pwm3, OUTPUT);
  pinMode(RL_A, OUTPUT);
  pinMode(RL_B, OUTPUT);
  pinMode(pwm4, OUTPUT);
  pinMode(RR_A, OUTPUT);
  pinMode(RR_B, OUTPUT);
}


//===============================Mecanum Wheel Simple Kinematics==================================//
void forward() {
  DEBUG_PRINTLN("Moving Forward");
  
  targetRPM[0] = rpmPlus;
  targetRPM[1] = rpmPlus;
  targetRPM[2] = rpmPlus;
  targetRPM[3] = rpmPlus;
}

void backward() {
  DEBUG_PRINTLN("Moving Backward");
  
  targetRPM[0] = rpmMin;
  targetRPM[1] = rpmMin;
  targetRPM[2] = rpmMin;
  targetRPM[3] = rpmMin;
}

void left() {
  DEBUG_PRINTLN("Turning Left");
  
  targetRPM[0] = rpmMin;
  targetRPM[1] = rpmPlus;
  targetRPM[2] = rpmPlus;
  targetRPM[3] = rpmMin;
}

void right() {
  DEBUG_PRINTLN("Turning Right");
  
  targetRPM[0] = rpmPlus;
  targetRPM[1] = rpmMin;
  targetRPM[2] = rpmMin;
  targetRPM[3] = rpmPlus;
}

void forwardLeft() {
  DEBUG_PRINTLN("Moving Forward Left");
  
  targetRPM[0] = 0.0;
  targetRPM[1] = rpmPlus;
  targetRPM[2] = rpmPlus;
  targetRPM[3] = 0.0;
}

void forwardRight() {
  DEBUG_PRINTLN("Moving Forward Right");
  
  targetRPM[0] = rpmPlus;
  targetRPM[1] = 0.0;
  targetRPM[2] = 0.0;
  targetRPM[3] = rpmPlus;
}

void backwardLeft() {
  DEBUG_PRINTLN("Moving Backward Left");
  
  targetRPM[0] = rpmMin;
  targetRPM[1] = 0.0;
  targetRPM[2] = 0.0;
  targetRPM[3] = rpmMin;
}

void backwardRight() {
  DEBUG_PRINTLN("Moving Backward Right");
  
  targetRPM[0] = 0.0;
  targetRPM[1] = rpmMin;
  targetRPM[2] = rpmMin;
  targetRPM[3] = 0.0;
}

void clockwise(){
  DEBUG_PRINTLN("Moving Clockwise");
  
  targetRPM[0] = rpmPlus;
  targetRPM[1] = rpmMin;
  targetRPM[2] = rpmPlus;
  targetRPM[3] = rpmMin;
}

void counterClockwise(){
  DEBUG_PRINTLN("Moving Counter Clocwise");
  
  targetRPM[0] = rpmMin;
  targetRPM[1] = rpmPlus;
  targetRPM[2] = rpmMin;
  targetRPM[3] = rpmPlus;
}

void stopMotor(){
  DEBUG_PRINTLN("stop no brake");
  
  targetRPM[0] = 0.0;
  targetRPM[1] = 0.0;
  targetRPM[2] = 0.0;
  targetRPM[3] = 0.0;
}

//  forward();
//  delay(2000);
//  backward();
//  delay(2000);
//  left();
//  delay(2000);
//  right();
//  delay(2000);
//  forwardLeft();
//  delay(2000);
//  forwardRight();
//  delay(2000);
//  backwardLeft();
//  delay(2000);
//  backwardRight();
//  delay(2000);
