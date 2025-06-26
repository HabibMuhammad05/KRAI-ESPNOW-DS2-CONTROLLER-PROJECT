#ifndef dribble_H
#define dribble_H


// === Pin Definition ===
const int pneumaticPushPin = 30;      // Pneumatic 1: Dorong bola ke bawah
const int pneumaticHoldPin1 = 31;     // Pneumatic 2: Penahan bola
const int pneumaticHoldPin2 = 32;     // Pneumatic 3: Penahan bola
const int slidingPneumaticPin = 33;   // Pneumatic untuk sliding rack
const int proximityPin = A0;

// === State Variables ===
bool slidingManualAktif = false;
unsigned long slidingStartTime = 0;
const unsigned long slidingDuration = 1000; // ms
bool bolaTerlepas = false;
bool slidingRackMaju = false;
unsigned long pneumaticStartTime = 0;
const unsigned long pneumaticDuration = 1000; // ms



// === Setup Functions ===
void pneumaticSetup() {
  pinMode(pneumaticPushPin, OUTPUT);
  pinMode(pneumaticHoldPin1, OUTPUT);
  pinMode(pneumaticHoldPin2, OUTPUT);
  pinMode(slidingPneumaticPin, OUTPUT);

  digitalWrite(pneumaticPushPin, LOW);
  digitalWrite(pneumaticHoldPin1, HIGH); // Penahan aktif default
  digitalWrite(pneumaticHoldPin2, HIGH); // Penahan aktif default
  digitalWrite(slidingPneumaticPin, LOW); // Sliding awal nonaktif

  Serial.println("Pneumatic initialized.");
}

void proximitySetup() {
  pinMode(proximityPin, INPUT);
  Serial.println("Proximity sensor initialized.");
}

// === Logic Functions ===
void lepasBola() {
  if (digitalRead(pneumaticPushPin) != LOW) {
    Serial.println("Error: Pneumatic push not in correct state.");
    return;
  }

  Serial.println("Melepas bola...");
  digitalWrite(pneumaticHoldPin1, LOW);
  digitalWrite(pneumaticHoldPin2, LOW);
  delay(200); // Waktu buka penahan

  digitalWrite(pneumaticPushPin, HIGH); // Dorong bola
  pneumaticStartTime = millis();
  bolaTerlepas = true;
}

void tangkapBola() {
  if (!bolaTerlepas) {
    Serial.println("Error: Bola not released.");
    return;
  }

  Serial.println("Bola terdeteksi! Menutup penahan...");
  digitalWrite(pneumaticPushPin, LOW);      // Stop dorong
  digitalWrite(pneumaticHoldPin1, HIGH);    // Aktifkan penahan
  digitalWrite(pneumaticHoldPin2, HIGH);
  bolaTerlepas = false;

}

void slidingRackMajuFunc() {
  Serial.println("Sliding rack maju (pneumatic on)...");
  digitalWrite(slidingPneumaticPin, HIGH);
  slidingRackMaju = true;
}

void slidingRackMundur() {
  Serial.println("Sliding rack mundur (pneumatic off)...");
  digitalWrite(slidingPneumaticPin, LOW);
  slidingRackMaju = false;
  delay(1000); // Waktu mundur mekanik
}

// === Task / Loop Function ===
void slidingRackControlTask() {
  // Tekan SQUARE -> aktifkan sliding (jika belum aktif)
  if (recvData.stat[8] == true && !slidingManualAktif) {
    Serial.println("SQUARE ditekan: Sliding rack maju.");
    digitalWrite(slidingPneumaticPin, HIGH);
    slidingStartTime = millis();
    slidingManualAktif = true;
  }

  // Otomatis nonaktif setelah durasi
  if (slidingManualAktif && (millis() - slidingStartTime >= slidingDuration)) {
    Serial.println("Sliding rack mundur (otomatis).");
    digitalWrite(slidingPneumaticPin, LOW);
    slidingManualAktif = false;
  }
}


void giringBolaTask() {
  // Cek tombol CROSS (index 9)
  if (recvData.stat[9] == true && !bolaTerlepas) {
    lepasBola();
  }

  if (bolaTerlepas) {
    if (millis() - pneumaticStartTime >= pneumaticDuration) {
      digitalWrite(pneumaticPushPin, LOW); // Hentikan dorong
    }

    if (digitalRead(proximityPin) == HIGH) {
      tangkapBola();
    }
  }
}



#endif
