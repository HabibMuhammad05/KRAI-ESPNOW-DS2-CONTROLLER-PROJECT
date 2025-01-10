
//idc 1 connector, FRONT LEFT
uint8_t FL_A = 49;
uint8_t FL_B = 47;
uint8_t pwm1 = 2; //6

//idc 2 connector, FRONT RIGHT
uint8_t FR_A = 43;
uint8_t FR_B = 45;
uint8_t pwm2 = 3; //7

//idc 3 connector, REAR LEFT
uint8_t RL_A = 41;
uint8_t RL_B = 39;
uint8_t pwm3 = 4;

//idc 4 connector, REAR RIGHT
uint8_t RR_A = 35;
uint8_t RR_B = 37;
uint8_t pwm4 = 5;

uint8_t pwmVal = 60;

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
  DEBUG_PRINTLN("Mecanum Ready");
}

void forward() {
  DEBUG_PRINTLN("Moving Forward");
  analogWrite(pwm1, pwmVal);
  digitalWrite(FL_A, HIGH);
  digitalWrite(FL_B, LOW);
  analogWrite(pwm2, pwmVal);
  digitalWrite(FR_A, HIGH);
  digitalWrite(FR_B, LOW);
  analogWrite(pwm3, pwmVal);
  digitalWrite(RL_A, HIGH);
  digitalWrite(RL_B, LOW);
  analogWrite(pwm4, pwmVal);
  digitalWrite(RR_A, HIGH);
  digitalWrite(RR_B, LOW);
}

void backward() {
  DEBUG_PRINTLN("Moving Backward");
  analogWrite(pwm1, pwmVal);
  digitalWrite(FL_A, LOW);
  digitalWrite(FL_B, HIGH);
  analogWrite(pwm2, pwmVal);
  digitalWrite(FR_A, LOW);
  digitalWrite(FR_B, HIGH);
  analogWrite(pwm3, pwmVal);
  digitalWrite(RL_A, LOW);
  digitalWrite(RL_B, HIGH);
  analogWrite(pwm4, pwmVal);
  digitalWrite(RR_A, LOW);
  digitalWrite(RR_B, HIGH);
}

void left() {
  DEBUG_PRINTLN("Turning Left");
  analogWrite(pwm1, pwmVal);
  digitalWrite(FL_A, LOW);
  digitalWrite(FL_B, HIGH);
  analogWrite(pwm2, pwmVal);
  digitalWrite(FR_A, HIGH);
  digitalWrite(FR_B, LOW);
  analogWrite(pwm3, pwmVal);
  digitalWrite(RL_A, HIGH);
  digitalWrite(RL_B, LOW);
  analogWrite(pwm4, pwmVal);
  digitalWrite(RR_A, LOW);
  digitalWrite(RR_B, HIGH);
}

void right() {
  DEBUG_PRINTLN("Turning Right");
  analogWrite(pwm1, pwmVal);
  digitalWrite(FL_A, HIGH);
  digitalWrite(FL_B, LOW);
  analogWrite(pwm2, pwmVal);
  digitalWrite(FR_A, LOW);
  digitalWrite(FR_B, HIGH);
  analogWrite(pwm3, pwmVal);
  digitalWrite(RL_A, LOW);
  digitalWrite(RL_B, HIGH);
  analogWrite(pwm4, pwmVal);
  digitalWrite(RR_A, HIGH);
  digitalWrite(RR_B, LOW);
}

void forwardLeft() {
  DEBUG_PRINTLN("Moving Forward Left");
  analogWrite(pwm1, 0);
  digitalWrite(FL_A, LOW);
  digitalWrite(FL_B, LOW);
  analogWrite(pwm2, pwmVal);
  digitalWrite(FR_A, HIGH);
  digitalWrite(FR_B, LOW);
  analogWrite(pwm3, pwmVal);
  digitalWrite(RL_A, HIGH);
  digitalWrite(RL_B, LOW);
  analogWrite(pwm4, 0);
  digitalWrite(RR_A, LOW);
  digitalWrite(RR_B, LOW);
}

void forwardRight() {
  DEBUG_PRINTLN("Moving Forward Right");
  analogWrite(pwm1, pwmVal);
  digitalWrite(FL_A, HIGH);
  digitalWrite(FL_B, LOW);
  analogWrite(pwm2, 0);
  digitalWrite(FR_A, LOW);
  digitalWrite(FR_B, LOW);
  analogWrite(pwm3, 0);
  digitalWrite(RL_A, LOW);
  digitalWrite(RL_B, LOW);
  analogWrite(pwm4, pwmVal);
  digitalWrite(RR_A, HIGH);
  digitalWrite(RR_B, LOW);
}

void backwardLeft() {
  DEBUG_PRINTLN("Moving Backward Left");
  analogWrite(pwm1, pwmVal);
  digitalWrite(FL_A, LOW);
  digitalWrite(FL_B, HIGH);
  analogWrite(pwm2, 0);
  digitalWrite(FR_A, LOW);
  digitalWrite(FR_B, LOW);
  analogWrite(pwm3, 0);
  digitalWrite(RL_A, LOW);
  digitalWrite(RL_B, LOW);
  analogWrite(pwm4, pwmVal);
  digitalWrite(RR_A, LOW);
  digitalWrite(RR_B, HIGH);
}

void backwardRight() {
  DEBUG_PRINTLN("Moving Backward Right");
  analogWrite(pwm1, 0);
  digitalWrite(FL_A, LOW);
  digitalWrite(FL_B, LOW);
  analogWrite(pwm2, pwmVal);
  digitalWrite(FR_A, LOW);
  digitalWrite(FR_B, HIGH);
  analogWrite(pwm3, pwmVal);
  digitalWrite(RL_A, LOW);
  digitalWrite(RL_B, HIGH);
  analogWrite(pwm4, 0);
  digitalWrite(RR_A, LOW);
  digitalWrite(RR_B, LOW);
}

void clockwise(){
  DEBUG_PRINTLN("Moving Clockwise");
  analogWrite(pwm1, pwmVal);
  digitalWrite(FL_A, HIGH);
  digitalWrite(FL_B, LOW);
  analogWrite(pwm2, pwmVal);
  digitalWrite(FR_A, LOW);
  digitalWrite(FR_B, HIGH);
  analogWrite(pwm3, pwmVal);
  digitalWrite(RL_A, HIGH);
  digitalWrite(RL_B, LOW);
  analogWrite(pwm4, pwmVal);
  digitalWrite(RR_A, LOW);
  digitalWrite(RR_B, HIGH);
}

void counterClockwise(){
  DEBUG_PRINTLN("Moving Counter Clocwise");
  analogWrite(pwm1, pwmVal);
  digitalWrite(FL_A, LOW);
  digitalWrite(FL_B, HIGH);
  analogWrite(pwm2, pwmVal);
  digitalWrite(FR_A, HIGH);
  digitalWrite(FR_B, LOW);
  analogWrite(pwm3, pwmVal);
  digitalWrite(RL_A, LOW);
  digitalWrite(RL_B, HIGH);
  analogWrite(pwm4, pwmVal);
  digitalWrite(RR_A, HIGH);
  digitalWrite(RR_B, LOW);
}

void stopMotor(){
  DEBUG_PRINTLN("stop no brake");
  analogWrite(pwm1, 0);
  analogWrite(pwm2, 0);
  analogWrite(pwm3, 0);
  analogWrite(pwm4, 0);
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
