AsyncWebServer server(80);
// ========================================== WEB Interface ==================================
const char webHTML[] PROGMEM = R"rawliteral(
  <!DOCTYPE html>
    <html lang='en'>
    <head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>ESP32 DS2 REMOTE SETUP</title>
    <style>
    body {
        font-family: Arial, sans-serif;
        background-color: lightslategray;
        display: flex;
        justify-content: center;
        align-items: center;
        height: 100vh;
        margin: 0;
    }
    .container {
        background-color: gray;
        padding: 20px;
        border-radius: 10px;
        box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        max-width: 600px;
        width: 900%;
    }
    h1, h2 {
        color: #333;
        text-align: center;
    }
    table, th, td {
        width: 100%;
        border-collapse: collapse;
        margin-top: 20px;
        border: 1px solid #ccc;
        padding: 10px;
        text-align: center;
    }
    .submitButton{
        background-color: #4CAF50;
        color: #fff;
        padding: 10px 20px;
        border: none;
        border-radius: 4px;
        cursor: pointer;
        font-size: 16px;
        width: 100%;
    }
    </style>
    </head>
    <body>
    <div class='container'>    
        <h1>INFORMATION MENU</h1> 
        <h2>ESP32 BASED DS2 CONTROLLER</h3> 
        <form action='/overwriteSettings' method='get'> 
            <table>
                <tr> 
                    <td colspan='5' style='width:20%' >Koneksi Tombol - GPIO</td> 
                </tr> 
                <tr> 
                    <td style='width:20%' ><b>GPIO 33 - L3</b></td> 
                    <td style='width:20%' ><b>GPIO 15 - LEFT</b></td>  
                    <td style='width:20%' ><b>GPIO 12 - RIGHT</b></td>
                    <td style='width:20%' ><b>GPIO 14 - TRIANGLE</b></td> 
                    <td style='width:20%' ><b>GPIO 18 - R3</b></td> 
                </tr> 
                <tr> 
                    <td style='width:20%' ><b>GPIO 16 - L2</b></td> 
                    <td style='width:20%' ><b>GPIO 4  - UP</b></td> 
                    <td style='width:20%' ><b>GPIO 2 - SWITCH</b></td> 
                    <td style='width:20%' ><b>GPIO 26 - CROSS</b></td> 
                    <td style='width:20%' ><b>GPIO 19 - R2</b></td> 
                </tr> 
                <tr> 
                    <td style='width:20%' ><b>GPIO 17 - L1</b></td> 
                    <td style='width:20%' ><b>GPIO 13 - DOWN</b></td> 
                    <td style='width:20%' ><b>GPIO 25 - SQUARE</b></td> 
                    <td style='width:20%' ><b>GPIO 27 - ROUND</b></td> 
                    <td style='width:20%' ><b>GPIO 23 - R1</b></td> 
                </tr> 
                <tr> 
                    <td colspan='5'> 
                        <label><b>Setting MAC Address RX: </b></label>
                        <br><br>
                        <input type='radio' id='slc1' name='addr' value='0'> ESP32U |  R1  | 80:7D:3A:EA:B1:98
                        <br>
                        <input type='radio' id='slc2' name='addr' value='1'> ESP32  | USB  | 8C:4F:00:3C:91:7C
                        <br>
                        <input type='radio' id='slc3' name='addr' value='2'> ESP32U |   2  | 14:33:5C:02:34:58
                        <br>
                        <input type='radio' id='slc4' name='addr' value='3'> ESP32U |   3  | 14:33:5C:02:49:58
                        <br><br>
                        <input class='submitButton' type='submit' name='Save' value='Submit'>
                    </td>
                </tr> 
                <tr> 
                    <td colspan='5'> 
                       <h5><a href='http://192.168.4.1/update'>Open ESPNOW Remote OTA Upload</a></h5>
                       <h5><a href='https://www.instagram.com/lexarga_team'>@KRAI - LEXARGA24 TEAM</a></h5>
                    </td>
                </tr> 
            </table>
        </form>
    </div>
    </body>
    </html>
)rawliteral";

const char webOverwrite[] PROGMEM = R"rawliteral(
  <!DOCTYPE html>
  <html lang='en'>
    <head>
      <meta charset='UTF-8'>
      <meta name='viewport' content='width=device-width, initial-scale=1.0'>
      <title>Setting Berhasil!</title>
      <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            background-color: #f4f4f4;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 140vh;
            margin: 0;
        }
        .container {
            background-color: #fff;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
            max-width: 600px;
            width: 100%;
        }
        h1, h2 {
            color: #333;
        }
      </style>
    </head>
    <body>
      <div class='container'>
        <h1>Setting Done</h1>
        <h2>Restarting...</h2>
      </div>
    </body>
  </html>
)rawliteral";

// ============================================= EEPROM ======================================
void write_EEPROM(int pos1, int pos2,String strText){
  int pj_strText=strText.length()+1;
  char stringIn[pj_strText];
  strText.toCharArray(stringIn,pj_strText);

  int j=-1;
  for(int i=pos1; i<pos2+1; i++){
    j++;
    if (i<pos1+pj_strText )  {
      EEPROM.write(i, stringIn[j]);
      EEPROM.commit();
    }else{
      EEPROM.write(i, '\0');
      EEPROM.commit();
    }
  }
}

String read_EEPROM(int pos1,int pos2){ 
  int i;
  char c;
  String temp="";
  for (i=pos1; i<pos2; i++){
    c=EEPROM.read(i);
    temp=temp+String(c);
  }
  temp=temp+'\0';
  return temp;
}

void reload_EEPROM(){
  //============= MAIN VAR =============
  String macStr  = read_EEPROM(100,101);
  macStr  = macStr.substring(0, macStr.length());
  macIndex = atoi(macStr.c_str());
  
  DEBUG_PRINT("Index MAC : "); DEBUG_PRINTLN(macIndex);

}

void overwriteSettings(AsyncWebServerRequest *request) {
  //============= MAIN VAR ============
  if (request->hasParam("addr")) {
    String macSel = request->getParam("addr")->value();
    write_EEPROM(100, 101, macSel);  
  }
//  String macSel = server.arg("addr");

  request->send_P(200, "text/html", webOverwrite);
  
  reload_EEPROM();

  DEBUG_PRINTLN("Setup done, resetting");
  delay(2000);
  ESP.restart();
}

void StartOTA(){
  WiFi.softAP("SETUP_REMOTE_3", "LEXARGA24");
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", webHTML);
  });  
  server.on("/overwriteSettings",overwriteSettings);

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
}


// ============================================= EEPROM ======================================
//void write_EEPROM(int pos1, int pos2,String strText){
//  int pj_strText=strText.length()+1;
//  char stringIn[pj_strText];
//  strText.toCharArray(stringIn,pj_strText);
//
//  int j=-1;
//  for(int i=pos1; i<pos2+1; i++){
//    j++;
//    if (i<pos1+pj_strText )  {
//      EEPROM.write(i, stringIn[j]);
//      EEPROM.commit();
//    }else{
//      EEPROM.write(i, '\0');
//      EEPROM.commit();
//    }
//  }
//}
//
//String read_EEPROM(int pos1,int pos2){ 
//  int i;
//  char c;
//  String temp="";
//  for (i=pos1; i<pos2; i++){
//    c=EEPROM.read(i);
//    temp=temp+String(c);
//  }
//  temp=temp+'\0';
//  return temp;
//}
