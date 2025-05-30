#ifndef penangkapBola_H
#define penangkapBola_H

#include <AccelStepper.h>
#include "dataReadFunc.h"

// === Pin Definitions ===
const int stepPins[4] = {5, 7, 9, 11};
const int dirPins[4]  = {6, 8, 10, 12};

// === Stepper Objects ===
AccelStepper steppers[4] = {
  AccelStepper(AccelStepper::DRIVER, stepPins[0], dirPins[0]),
  AccelStepper(AccelStepper::DRIVER, stepPins[1], dirPins[1]),
  AccelStepper(AccelStepper::DRIVER, stepPins[2], dirPins[2]),
  AccelStepper(AccelStepper::DRIVER, stepPins[3], dirPins[3])
};

// === Setup ===
void penangkapSetup() {
  for (int i = 0; i < 4; i++) {
    steppers[i].setMaxSpeed(800);
    steppers[i].setAcceleration(400);
  }
  Serial.println("Blocking Control Initialized with 4 steppers.");
}

// === Manual Control Task ===
void penangkapTask() {
  bool naik = recvData.stat[4];
  bool turun = recvData.stat[6];

  for (int i = 0; i < 4; i++) {
    if (naik) {
      steppers[i].move(10);  // Naik pelan
    } else if (turun) {
      steppers[i].move(-10); // Turun pelan
    } else {
      steppers[i].stop();
    }

    steppers[i].run();  // Wajib dipanggil agar motor berjalan
  }
}

#endif
