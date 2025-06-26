#ifndef shooting_H
#define shooting_H

#include "dataReadFunc.h"

// === Pin Definition ===
// Pelontar atas (CCW)
const int pelontarAtas_RPWM = 40;
const int pelontarAtas_LPWM = 41;

// Pelontar bawah (CW)
const int pelontarBawah_RPWM = 42;
const int pelontarBawah_LPWM = 43;

// Motor sudut
const int sudut_RPWM = 44;
const int sudut_LPWM = 45;

// Pneumatic feeder
const int PneumaticfeederPin = 46;

// === STATUS VARIABEL ===
bool pelontarAktif = false;
bool sedangTembak = false;
unsigned long feederStartTime = 0;
const unsigned long feederDuration = 500; // ms

// === Setup Functions ===
void shootingSetup() {
  // Set pin output
  pinMode(pelontarAtas_RPWM, OUTPUT);
  pinMode(pelontarAtas_LPWM, OUTPUT);
  pinMode(pelontarBawah_RPWM, OUTPUT);
  pinMode(pelontarBawah_LPWM, OUTPUT);
  pinMode(sudut_RPWM, OUTPUT);
  pinMode(sudut_LPWM, OUTPUT);
  pinMode(PneumaticfeederPin, OUTPUT);
  digitalWrite(PneumaticfeederPin, LOW);

  // Matikan semua motor
  matikanPelontar();
  hentikanMotorSudut();

  Serial.println("Sistem shooting siap.");
}

// === PELONTAR ===
void aktifkanPelontar() {
  // Atas CCW
  analogWrite(pelontarAtas_RPWM, 0);
  analogWrite(pelontarAtas_LPWM, 200);

  // Bawah CW
  analogWrite(pelontarBawah_RPWM, 200);
  analogWrite(pelontarBawah_LPWM, 0);

  pelontarAktif = true;
  Serial.println("Pelontar aktif.");
}

void matikanPelontar() {
  analogWrite(pelontarAtas_RPWM, 0);
  analogWrite(pelontarAtas_LPWM, 0);
  analogWrite(pelontarBawah_RPWM, 0);
  analogWrite(pelontarBawah_LPWM, 0);

  pelontarAktif = false;
  Serial.println("Pelontar mati.");
}

// === SUDUT ===
/*void naikkanSudut() {
  analogWrite(sudut_RPWM, 200);
  analogWrite(sudut_LPWM, 0);
  Serial.println("Sudut naik.");
}

void turunkanSudut() {
  analogWrite(sudut_RPWM, 0);
  analogWrite(sudut_LPWM, 200);
  Serial.println("Sudut turun.");
}

void hentikanMotorSudut() {
  analogWrite(sudut_RPWM, 0);
  analogWrite(sudut_LPWM, 0);
}*/

// === FEEDER ===
void tembakBola() {
  digitalWrite(PneumaticfeederPin, HIGH);
  feederStartTime = millis();
  sedangTembak = true;
  Serial.println("Tembak bola!");
}

// === LOOP TASK ===
void shootingTask() {
  // Kontrol pelontar tombol round (index 5)
  if (recvData.stat[10] == true && !pelontarAktif) {
    aktifkanPelontar();
  } else if (recvData.stat[10] == false && pelontarAktif) {
    matikanPelontar();
  }

  // Kontrol feeder tombol TRIANGLE (index 6)
  if (recvData.stat[11] == true && !sedangTembak) {
    tembakBola();
  }

  if (sedangTembak && millis() - feederStartTime >= feederDuration) {
    digitalWrite(PneumaticfeederPin, LOW);
    sedangTembak = false;
    Serial.println("Feeder selesai.");
  }

  // Kontrol sudut NAIK tombol UP (index 7)
  // Kontrol sudut TURUN tombol DOWN (index 8)
 /* if (recvData.stat[4] == true) {
    naikkanSudut();
  } else if (recvData.stat[6] == true) {
    turunkanSudut();
  } else {
    hentikanMotorSudut();
  }
}*/

#endif