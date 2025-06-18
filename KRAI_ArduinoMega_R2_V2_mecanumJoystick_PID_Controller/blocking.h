//======================================= MOTOR DRIVER PINS =======================================//
uint8_t EN_L = 31;
uint8_t EN_R = 33;
uint8_t PWM = 8;
const int LS_A = 44; 
const int LS_B = 42;  
bool motorON = false;
bool lastUpState = false;
bool lastDownState = false;

void lifterSetup() {
  pinMode(PWM, OUTPUT);
  pinMode(EN_L, OUTPUT);
  pinMode(EN_R, OUTPUT);
  pinMode(LS_A, INPUT_PULLUP);
  pinMode(LS_B, INPUT_PULLUP);
  DEBUG_PRINTLN(115200);

  // digitalWrite(LS_A, HIGH);
  // digitalWrite(LS_B, HIGH);
}

void motorNaik() {
  digitalWrite(EN_L, LOW);
  digitalWrite(EN_R, HIGH);
  analogWrite(PWM, 125);
  motorON = true;
}

void motorTurun() {
  digitalWrite(EN_L, HIGH);
  digitalWrite(EN_R, LOW);
  analogWrite(PWM, 125);
  motorON = true;
}

void controlLift() {
  bool upPressed = recvData.stat[6] == 0;    // aktif LOW
  bool downPressed = recvData.stat[4] == 0;  // aktif LOW

  // Status limit switch
  bool atTop = (digitalRead(LS_A) == false);     // Limit atas aktif
  bool atBottom = (digitalRead(LS_B) == false);  // Limit bawah aktif


  if (!downPressed && upPressed && !atBottom) {
    digitalWrite(EN_L, HIGH);
    digitalWrite(EN_R, LOW);
    analogWrite(PWM, 100);
    DEBUG_PRINT("BOTTOM Touched");
  } else if (!upPressed && downPressed && !atTop) {
    digitalWrite(EN_L, LOW);
    digitalWrite(EN_R, HIGH);
    analogWrite(PWM, 100);  // sesuaikan speed
    DEBUG_PRINT("TOP Touched");
  } else {
    // Stop motor
    analogWrite(PWM, 0);
    digitalWrite(EN_L, LOW);
    digitalWrite(EN_R, LOW);
  }
}

//=======================================================================//
// Tombol (UP = index 4, DOWN = index 6)
//   bool limittertekanA = (digitalRead(LS_A) == LOW);
//   bool limittertekanB = (digitalRead(LS_B) == LOW);

//   // if(!limittertekanA || !limittertekanB){
//   //   digitalWrite(EN_L, LOW);
//   //   digitalWrite(EN_R, LOW);
//   //   analogWrite(PWM, 0);
//   //   motorON = false;
//   //   return;
//   // }
// //========================================================
//   if (!recvData.stat[4] && lastUpState && !limittertekanA) {
//     // if(!limittertekanA){
//       motorNaik();
//     // }

//   }
//   // lastUpState = recvData.stat[4];

//   //=======================================================
//   if (!recvData.stat[6] && lastDownState && !limittertekanB) {
//     // if(!limittertekanB){
//     motorTurun();
//     // }
//   }
//   // lastDownState = recvData.stat[6];
// }

// void motorStopAtas() {
//   digitalRead(LS_A, HIGH);
//   digitalRead(LS_B, LOW);
//   digitalWrite(EN_L, LOW);
//   digitalWrite(EN_R, LOW);
//   analogWrite(PWM, 0);
//   motornaik = false;
// }

// void motorStopBawah() {
//   digitalRead(LS_A, LOW);
//   digitalRead(LS_B, HIGH);
//   digitalWrite(EN_L, LOW);
//   digitalWrite(EN_R, LOW);
//   analogWrite(PWM, 0);
//   motornaik = false;
// }