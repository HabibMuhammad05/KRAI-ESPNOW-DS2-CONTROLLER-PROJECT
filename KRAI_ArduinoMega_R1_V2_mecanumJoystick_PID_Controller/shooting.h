// === Pin Definition ===

const int pelontarPWM = 8;
const int motorDirA = 31;
const int motorDirB = 33;

// const int sudut_RPWM = 19;
// const int sudut_LPWM = 25;

// === STATUS VARIABEL ===
bool pelontarAktif = false;
bool LastRoundState = false;
bool LastCrossState = false;
bool LastSquareState = false;
bool sedangTembak = false;


void shootingSetup() {
  pinMode(pelontarPWM, OUTPUT);
  // pinMode(sudut_RPWM, OUTPUT);
  // pinMode(sudut_LPWM, OUTPUT);

  //===========PIN ENABLE=============//
  pinMode(31, OUTPUT);  // EN_L
  pinMode(33, OUTPUT);  // EN_R

  Serial.println("Sistem shooting siap.");
}

// === PELONTAR ===
void aktifkanPelontarA() {
  analogWrite(pelontarPWM, 125);  //Pass
  digitalWrite(31, HIGH);
  digitalWrite(33, LOW);
  pelontarAktif = true;
  Serial.println("Pelontar aktif.");
}

void aktifkanPelontarB(){
  analogWrite(pelontarPWM, 255);  //Shoot
  digitalWrite(31, HIGH);
  digitalWrite(33, LOW);
  pelontarAktif = true;
}

void matikanPelontar() {
  analogWrite(pelontarPWM, 0);
  digitalWrite(31, LOW);
  digitalWrite(33, LOW);
  pelontarAktif = false;
  Serial.println("Pelontar mati.");
}

// === LOOP TASK ===
void shootingTask() {

  //======PASSING BUTTON=======)
  if (!recvData.stat[10] && LastRoundState) {
    aktifkanPelontarA();
  }
  LastRoundState = recvData.stat[10];

  //=======SHOOT BUTTON=========//
  if (!recvData.stat[8] && LastSquareState) {
    aktifkanPelontarB();
  }
  LastSquareState = recvData.stat[8];

  //=========MOTOR SHUTDOWN=============//
  if (!recvData.stat[9] && LastCrossState) {
    matikanPelontar();
  }
  LastCrossState = recvData.stat[9];
}


//====================================================================================================//
// Deteksi rising edge: tombol baru ditekan
// if (currentButtonState && !lastRoundState) {
//   pelontarAktif = !pelontarAktif;  // Toggle status

//   if (pelontarAktif) {
//     aktifkanPelontar();
//   } else {
//     matikanPelontar();    
//   }
// }

// === SUDUT ===
// void naikkanSudut() {
//   analogWrite(sudut_RPWM, 200);
//   analogWrite(sudut_LPWM, 0);
//   Serial.println("Sudut naik.");
// }

// void turunkanSudut() {
//   analogWrite(sudut_RPWM, 0);
//   analogWrite(sudut_LPWM, 200);
//   Serial.println("Sudut turun.");
// }

// void hentikanMotorSudut() {
//   analogWrite(sudut_RPWM, 0);
//   analogWrite(sudut_LPWM, 0);
// }

// Kontrol sudut NAIK tombol UP (index 7)
// Kontrol sudut TURUN tombol DOWN (index 8)
// if (recvData.stat[4] == true) {
//   naikkanSudut();
// } else if (recvData.stat[6] == true) {
//   turunkanSudut();
// } else {
//   hentikanMotorSudut();
// }

