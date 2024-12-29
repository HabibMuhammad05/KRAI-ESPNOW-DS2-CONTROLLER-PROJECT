//=====================================Joystick Calibration & read FUNC=====================================//
void calibrateJoysticks() {
  Serial.println("Calibrating joysticks...");

  int numSamples = 50;
  long joy1XSum = 0, joy1YSum = 0;
  long joy2XSum = 0, joy2YSum = 0;

  for (int i = 0; i < numSamples; i++) {
    joy1XSum += analogRead(joy1XPin);
    joy1YSum += analogRead(joy1YPin);
    joy2XSum += analogRead(joy2XPin);
    joy2YSum += analogRead(joy2YPin);
    delay(10);
  }

  // Calculate the average center position
  joy1XCenter = joy1XSum / numSamples;
  joy1YCenter = joy1YSum / numSamples;
  joy2XCenter = joy2XSum / numSamples;
  joy2YCenter = joy2YSum / numSamples;

  Serial.println("Calibration complete.");
  Serial.print("Joystick 1 Center: ");
  Serial.print(joy1XCenter);
  Serial.print(", ");
  Serial.println(joy1YCenter);
  Serial.print("Joystick 2 Center: ");
  Serial.print(joy2XCenter);
  Serial.print(", ");
  Serial.println(joy2YCenter);
}

void joystickRead(){
  int joy1XValue = analogRead(joy1XPin) - joy1XCenter;
  int joy1YValue = analogRead(joy1YPin) - joy1YCenter;
  int joy2XValue = analogRead(joy2XPin) - joy2XCenter;
  int joy2YValue = analogRead(joy2YPin) - joy2YCenter;
  sendData.joyData[0] = joy1XValue;
  sendData.joyData[1] = joy1YValue;
  sendData.joyData[2] = joy2XValue;
  sendData.joyData[3] = joy2YValue;
  
  Serial.print(String(joy1XValue) + ", " + String(joy1YValue) + ", " + String(joy2XValue) + ", " + String(joy2YValue) + "-----");
  
  // Apply threshold to create a dead zone
  joy1XValue = abs(joy1XValue) > centerThreshold ? joy1XValue : 0;
  joy1YValue = abs(joy1YValue) > centerThreshold ? joy1YValue : 0;
  joy2XValue = abs(joy2XValue) > centerThreshold ? joy2XValue : 0;
  joy2YValue = abs(joy2YValue) > centerThreshold ? joy2YValue : 0;

  // Map joystick values to box positions
  joy1XPos = map(joy1XValue, -2048, 2048, joy1XBox - boxSize / 2, joy1XBox + boxSize / 2);
  joy1YPos = map(joy1YValue, -2048, 2048, joy1YBox - boxSize / 2, joy1YBox + boxSize / 2);
  joy2XPos = map(joy2XValue, -2048, 2048, joy2XBox - boxSize / 2, joy2XBox + boxSize / 2);
  joy2YPos = map(joy2YValue, -2048, 2048, joy2YBox - boxSize / 2, joy2YBox + boxSize / 2);

}  
