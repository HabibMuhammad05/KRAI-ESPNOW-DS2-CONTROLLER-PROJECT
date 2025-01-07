
//LEFT = UP(-1742) DOWN(2353) RIGHT(2242) LEFT(-1853)
//RIGHT = UP(-1754) DOWN(2341) RIGHT(2329) LEFT(-1766)
//[0]LX, [1]LY, [2]RX, [3]RY


//idc 1 connector, FRONT LEFT
uint8_t FL_A = 49;
uint8_t FL_B = 47;
uint8_t pwm1 = 2;

//idc 2 connector, FRONT RIGHT
uint8_t FR_A = 43;
uint8_t FR_B = 45;
uint8_t pwm2 = 3;

//idc 3 connector, REAR LEFT
uint8_t RL_A = 41;
uint8_t RL_B = 39;
uint8_t pwm3 = 4;

//idc 4 connector, REAR RIGHT
uint8_t RR_A = 35;
uint8_t RR_B = 37;
uint8_t pwm4 = 5;

int joystickX1, joystickY1, joystickX2;
float currentSpeedFL = 0, currentSpeedFR = 0, currentSpeedRL = 0, currentSpeedRR = 0;
float speedRamp = 0.05; // Smooth acceleration rate

void mecanumSetup() {
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
  DEBUG_PRINTLN("Mecanum Ready");
}

int mapJoystick(int input, int negFrom, int posFrom, int negTo, int posTo) {
    if (input > 0) {
        return map(input, 0, posFrom, 0, posTo);
    } else if (input < 0) {
        return map(input, 0, negFrom, 0, negTo);
    } else {
        return 0;
    }
}

void controlMotor(int pinA, int pinB, int pwmPin, bool isForward, int pwmValue) {
  if (isForward) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
  } else {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
  }
  analogWrite(pwmPin, pwmValue);
}

void calcMecanum() { 
  int x1Map = mapJoystick(recvData.joyData[0], -1855, 2244, -255, 255);
  int y1Map = mapJoystick(recvData.joyData[1], -1744, 2355, -255, 255);
  int x2Map = mapJoystick(recvData.joyData[2], -1743, 2331, -255, 255);
  
  joystickX1 = x1Map; // First joystick X-axis (left-right)
  joystickY1 = map(y1Map, -255, 255, 255, -255); // First joystick Y-axis (forward-backward)
  joystickX2 = x2Map; // Second joystick X-axis (rotation)

  Serial.print("x1: "); Serial.print(joystickX1);
  Serial.print("--y1: "); Serial.print(joystickY1);
  Serial.print("--x2: "); Serial.println(joystickX2);

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

  
  int pwmFL = abs(currentSpeedFL * 60); // Map to PWM (0-255) and determine direction
  int pwmFR = abs(currentSpeedFR * 60);
  int pwmRL = abs(currentSpeedRL * 60);
  int pwmRR = abs(currentSpeedRR * 60);

  
  controlMotor(FL_A, FL_B, pwm1, currentSpeedFL > 0, pwmFL); // Control IBT motor driver
  controlMotor(FR_A, FR_B, pwm2, currentSpeedFR > 0, pwmFR);
  controlMotor(RL_A, RL_B, pwm3, currentSpeedRL > 0, pwmRL);
  controlMotor(RR_A, RR_B, pwm4, currentSpeedRR > 0, pwmRR);

  Serial.print("PWM FL: "); Serial.print(pwmFL);
  Serial.print(" | FR: "); Serial.print(pwmFR);
  Serial.print(" | RL: "); Serial.print(pwmRL);
  Serial.print(" | RR: "); Serial.println(pwmRR);
//  delay(50);
}
