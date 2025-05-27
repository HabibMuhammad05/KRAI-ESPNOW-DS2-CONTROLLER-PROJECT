//=====================================Joystick Calibration & read FUNC=====================================//
void calibrateJoysticks() {
  DEBUG_PRINTLN("JOYSTICK CALIBRATION");

  long joy1XSum = 0, joy1YSum = 0;
  long joy2XSum = 0, joy2YSum = 0;

  for (int i = 0; i < 50; i++) {
    joy1XSum += analogRead(joy1XPin);
    joy1YSum += analogRead(joy1YPin);
    joy2XSum += analogRead(joy2XPin);
    joy2YSum += analogRead(joy2YPin);
    delay(10);
  }

  joy1XCenter = joy1XSum / 50;
  joy1YCenter = joy1YSum / 50;
  joy2XCenter = joy2XSum / 50;
  joy2YCenter = joy2YSum / 50;

  DEBUG_PRINTLN("COMPLETE");
  DEBUG_PRINT("Joystick 1 Center: ");
  DEBUG_PRINT(joy1XCenter);
  DEBUG_PRINT(", ");
  DEBUG_PRINTLN(joy1YCenter);
  DEBUG_PRINT("Joystick 2 Center: ");
  DEBUG_PRINT(joy2XCenter);
  DEBUG_PRINT(", ");
  DEBUG_PRINTLN(joy2YCenter);
}

void joystickRead(){
  auto updateAverage = [](int &sum, int *buffer, int &index, int newValue) -> int {
      sum -= buffer[index];
      buffer[index] = newValue;
      sum += newValue;
      index = (index + 1) % NUM_SAMPLES;
      return sum / NUM_SAMPLES;
  };

  sendData.joyData[0] = updateAverage(joy1XSum, joy1XBuffer, joy1XIndex, analogRead(joy1XPin) - joy1XCenter);
  sendData.joyData[1] = updateAverage(joy1YSum, joy1YBuffer, joy1YIndex, analogRead(joy1YPin) - joy1YCenter);
  sendData.joyData[2] = updateAverage(joy2XSum, joy2XBuffer, joy2XIndex, analogRead(joy2XPin) - joy2XCenter);
  sendData.joyData[3] = updateAverage(joy2YSum, joy2YBuffer, joy2YIndex, analogRead(joy2YPin) - joy2YCenter);

  sendData.joyData[0] = map(sendData.joyData[0], -1866, 2229, -4095, 4095);
  sendData.joyData[1] = map(sendData.joyData[1], -1868, 2227, -4095, 4095);
  sendData.joyData[2] = map(sendData.joyData[2], -1799, 2296, -4095, 4095);
  sendData.joyData[3] = map(sendData.joyData[3], -1850, 2245, -4095, 4095);
  
  sendData.joyData[0] = constrain(sendData.joyData[0], -4095, 4095);
  sendData.joyData[1] = constrain(sendData.joyData[1], -4095, 4095);
  sendData.joyData[2] = constrain(sendData.joyData[2], -4095, 4095);
  sendData.joyData[3] = constrain(sendData.joyData[3], -4095, 4095);
  
  sendData.joyData[0] = abs(sendData.joyData[0]) > centerThreshold ? sendData.joyData[0] : 0;
  sendData.joyData[1] = abs(sendData.joyData[1]) > centerThreshold ? sendData.joyData[1] : 0;
  sendData.joyData[2] = abs(sendData.joyData[2]) > centerThreshold ? sendData.joyData[2] : 0;
  sendData.joyData[3] = abs(sendData.joyData[3]) > centerThreshold ? sendData.joyData[3] : 0;
  
  joy1XPos = map(sendData.joyData[0], -4095, 4095, joy1XBox - boxSize / 2, joy1XBox + boxSize / 2);
  joy1YPos = map(sendData.joyData[1], -4095, 4095, joy1YBox - boxSize / 2, joy1YBox + boxSize / 2);
  joy2XPos = map(sendData.joyData[2], -4095, 4095, joy2XBox - boxSize / 2, joy2XBox + boxSize / 2);
  joy2YPos = map(sendData.joyData[3], -4095, 4095, joy2YBox - boxSize / 2, joy2YBox + boxSize / 2);
}  

//    sendData.joyData[0] = analogRead(joy1XPin) - joy1XCenter;
//    sendData.joyData[1] = analogRead(joy1YPin) - joy1YCenter;
//    sendData.joyData[2] = analogRead(joy2XPin) - joy2XCenter;
//    sendData.joyData[3] = analogRead(joy2YPin) - joy2YCenter;

//  DEBUG_PRINT(String(joy1XValue) + ", " + String(joy1YValue) + ", " + String(joy2XValue) + ", " + String(joy2YValue) + "-----");
  
// Apply threshold to create a dead zone
//  DEBUG_PRINT(String(joy1XValue) + ", " + String(joy1YValue) + ", " + String(joy2XValue) + ", " + String(joy2YValue) + "-----");

//  sendData.joyData[0] = joy1XValue;
//  sendData.joyData[1] = joy1YValue;
//  sendData.joyData[2] = joy2XValue;
//  sendData.joyData[3] = joy2YValue;
