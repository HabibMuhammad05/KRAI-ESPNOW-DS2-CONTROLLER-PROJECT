#include <EEPROM.h>

// ============================================= EEPROM ======================================
void write_EEPROM(int pos, String strText) {
  int len = strText.length();
  for (int i = 0; i < len; i++) {
    EEPROM.write(pos + i, strText[i]);
  }
  EEPROM.write(pos + len, '\0'); // Write null terminator
  EEPROM.commit(); // Commit once after writing
}

String read_EEPROM(int pos) {
  char c;
  String temp = "";
  while (true) {
    c = EEPROM.read(pos++);
    if (c == '\0') break;
    temp += c;
  }
  return temp;
}

void reload_EEPROM() {
  String macRead = read_EEPROM(100);
  storedMac = atoi(macRead.c_str());
  incomingData.remoteIndex = storedMac;
  DEBUG_PRINT("Index MAC : "); DEBUG_PRINTLN(storedMac);
}

void check_EEPROM() {
  if (storedMac == incomingData.remoteIndex) return;

  String macSave = String(incomingData.remoteIndex);
  write_EEPROM(100, macSave);
  DEBUG_PRINTLN("Remote Index Changed, resetting");
  ESP.restart();
}
