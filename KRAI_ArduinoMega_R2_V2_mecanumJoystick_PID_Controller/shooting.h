// === Pin Definition === //

const int pelontarPWM = 9;
const int motorDirA = 27;
const int motorDirB = 29;

// const int sudut_RPWM = 19;
// const int sudut_LPWM = 25;

// === STATUS VARIABEL === //
bool pelontarAktif = false;
bool LastRoundState = false;
bool LastCrossState = false;
bool sedangTembak = false;

void shootingSetup() {
  pinMode(pelontarPWM, OUTPUT);
  // pinMode(sudut_RPWM, OUTPUT);
  // pinMode(sudut_LPWM, OUTPUT);

  //===========PIN ENABLE=============//
  pinMode(27, OUTPUT);  // EN_L
  pinMode(29, OUTPUT);  // EN_R

  Serial.println("Sistem shooting siap.");
}

// === PELONTAR === //
void aktifkanPelontar() {
  analogWrite(pelontarPWM, 255);  //CCW
  digitalWrite(27, HIGH);
  digitalWrite(29, LOW);
  pelontarAktif = true;
  Serial.println("Pelontar aktif.");
}

void matikanPelontar() {
  analogWrite(pelontarPWM, 0);
  digitalWrite(27, LOW);
  digitalWrite(29, LOW);
  pelontarAktif = false;
  Serial.println("Pelontar mati.");
}

// === LOOP TASK === //
void shootingTask() {
  // bool currentButtonState = recvData.stat[10];  // Tombol ROUND
  if (!recvData.stat[10] && LastRoundState) {
    aktifkanPelontar();
  }
  LastRoundState = recvData.stat[10];
  if (!recvData.stat[9] && LastCrossState) {
    matikanPelontar();
  }
  LastCrossState = recvData.stat[9];
}

// void feederTask() {
//   // Tombol TRIANGLE ditekan dan belum sedang menembak
//   if (recvData.stat[3] == true && !sedangTembak) {
//     tembakBola();  // Aktifkan feeder dan mulai timer
//   }

//   // Cek apakah sudah waktunya mematikan feeder
//   if (sedangTembak && millis() - feederStartTime >= feederDuration) {
//     // digitalWrite(relay4, LOW);
//     sedangTembak = false;
//     Serial.println("Feeder OFF");
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

// Deteksi rising edge: tombol baru ditekan
// if (currentButtonState && !lastRoundState) {
//   pelontarAktif = !pelontarAktif;  // Toggle status

//   if (pelontarAktif) {
//     aktifkanPelontar();
//   } else {
//     matikanPelontar();    // Fungsi kamu untuk matikan motor pelontar
//   }
// }

// // === FEEDER ===
// void tembakBola() {
//   // digitalWrite(relay4, HIGH);
//   feederStartTime = millis();
//   sedangTembak = true;
//   Serial.println("Tembak bola!");
// }
