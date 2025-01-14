//LEFT = UP(-1742) DOWN(2353) RIGHT(2242) LEFT(-1853)
//RIGHT = UP(-1754) DOWN(2341) RIGHT(2329) LEFT(-1766)
//[0]LX, [1]LY, [2]RX, [3]RY


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


//============================ MECANUM KINEMATICS CALCULATION VARIABLES ==========================//
int joystickX1, joystickY1, joystickX2;
float currentSpeedFL = 0, currentSpeedFR = 0, currentSpeedRL = 0, currentSpeedRR = 0;
float speedRamp = 1; 


//================================== PID CALCULATION VARIABLES ===================================//
uint8_t maxRPM = 200;

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
void calcPID(float target_FL, float target_FR, float target_RL, float target_RR){
  
  targetRPM[0] = target_FL;
  targetRPM[1] = target_FR;
  targetRPM[2] = target_RL;
  targetRPM[3] = target_RR;
  
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
  
      if (motorPWM[motorIndex] > 0) {
        motorPWM[motorIndex] = constrain(motorPWM[motorIndex], 0, 50);
        analogWrite(motorPWM_Pin[motorIndex], motorPWM[motorIndex]);
        digitalWrite(motorA[motorIndex], HIGH);
        digitalWrite(motorB[motorIndex], LOW);
      } else {
        motorPWM[motorIndex] = constrain(abs(motorPWM[motorIndex]), 0, 50);
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

//=================================== driver motor declaration =====================================//
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

//================================== JOYSTICK DATA CALCULATION =====================================//
int mapJoystick(int input, int negFrom, int posFrom, int negTo, int posTo) {
    if (input > 0) {
        return map(input, 0, posFrom, 0, posTo);
    } else if (input < 0) {
        return map(input, 0, negFrom, 0, negTo);
    } else {
        return 0;
    }
}

//============================ MECANUM WHEEL KINEMATICS CALCULATION=================================//
void calcMecanum() { 
  int x1Map = mapJoystick(recvData.joyData[0], -1855, 2244, -255, 255);
  int y1Map = mapJoystick(recvData.joyData[1], -1744, 2355, -255, 255);
  int x2Map = mapJoystick(recvData.joyData[2], -1743, 2331, -255, 255);
  
  joystickX1 = x1Map; // First joystick X-axis (left-right)
  joystickY1 = map(y1Map, -255, 255, 255, -255); // First joystick Y-axis (forward-backward)
  joystickX2 = x2Map; // Second joystick X-axis (rotation)

  DEBUG_PRINT("x1: "); DEBUG_PRINT(joystickX1);
  DEBUG_PRINT("--y1: "); DEBUG_PRINT(joystickY1);
  DEBUG_PRINT("--x2: "); DEBUG_PRINTLN(joystickX2);

  float N_x = joystickX1 / 255.0;// Normalize joystick values to range -1 to 1
  float N_y = joystickY1 / 255.0;
  float N_r = joystickX2 / 255.0;

  if (abs(N_x) < 0.05) N_x = 0; // Apply dead zone
  if (abs(N_y) < 0.05) N_y = 0;
  if (abs(N_r) < 0.05) N_r = 0;

  float targetFL = N_y - N_x - N_r; // Calculate wheel speeds
  float targetFR = N_y + N_x + N_r;
  float targetRL = N_y + N_x - N_r;
  float targetRR = N_y - N_x + N_r;

  float maxSpeed = max(max(abs(targetFL), abs(targetFR)), max(abs(targetRL), abs(targetRR))); // Normalize wheel speeds
  if (maxSpeed > 1.0) {
    targetFL /= maxSpeed;
    targetFR /= maxSpeed;
    targetRL /= maxSpeed;
    targetRR /= maxSpeed;
  }

  currentSpeedFL += constrain(targetFL - currentSpeedFL, -speedRamp, speedRamp); // Gradual speed adjustment for smooth motion
  currentSpeedFR += constrain(targetFR - currentSpeedFR, -speedRamp, speedRamp);
  currentSpeedRL += constrain(targetRL - currentSpeedRL, -speedRamp, speedRamp);
  currentSpeedRR += constrain(targetRR - currentSpeedRR, -speedRamp, speedRamp);

  
  int rpmFL = currentSpeedFL * maxRPM; // Map to PWM (0-255) and determine direction
  int rpmFR = currentSpeedFR * maxRPM;
  int rpmRL = currentSpeedRL * maxRPM;
  int rpmRR = currentSpeedRR * maxRPM;

//  if((pwmFL == 0 || pwmFR == 0) || (pwmRL = 0 || pwmRR == 0)){
//    analogWrite(pwm1, 0);
//    analogWrite(pwm2, 0);
//    analogWrite(pwm3, 0);
//    analogWrite(pwm4, 0);
//  }else{
    calcPID(rpmFL, rpmFR, rpmRL, rpmRR);
//  }

  DEBUG_PRINT("RPM FL: "); DEBUG_PRINT(pwmFL);
  DEBUG_PRINT(" | FR: "); DEBUG_PRINT(pwmFR);
  DEBUG_PRINT(" | RL: "); DEBUG_PRINT(pwmRL);
  DEBUG_PRINT(" | RR: "); DEBUG_PRINTLN(pwmRR);
//  delay(50);
}
