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


#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
AsyncWebServer server(80);

const char* ssid = "OTA";
const char* password = "LEXARGA24";
bool settingMode;
unsigned long holdStartTime = 0;
bool inOtaMode = false;
bool holding = false;

void startOtaWebServer(){
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
  request->send(200, "text/plain", "Hi! I am ESP32.");
  });

  AsyncElegantOTA.begin(&server);    
  server.begin();
}

void enterOtaMode() {
  inOtaMode = true;
  esp_now_deinit(); 
  delay(100);
  startOtaWebServer();
}
