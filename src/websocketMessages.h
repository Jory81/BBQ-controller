void onRootRequest(AsyncWebServerRequest *request) {
  request->send(SPIFFS, "/index.html", "text/html");    // , false, processor
}

void initWebServer() {
    server.on("/", onRootRequest);
    server.serveStatic("/", SPIFFS, "/");
    server.begin();
}
             
void onEvent(AsyncWebSocket       *server,
             AsyncWebSocketClient *client,
             AwsEventType          type,
             void                 *arg,
             uint8_t              *data,
             size_t                len) {

    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            handleWebSocketMessage(arg, data, len);
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

void initWebSocket() {
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}

void notifyClients() {
//    ws.textAll(led.on ? "on" : "off");
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
      for (int i = 0; i < 3; i++){
      strval[i]=(char)data[i];
      firstChar=(char)data[0];
      }
      data[len]=NULL;
      //Serial.print("len"); Serial.println(len);
      if (firstChar == "G" || firstChar == "1"){ // I and P are reserved for wifi password and ID (SEE WIFI UPDATES)
        strval[3]=0;
        str=String(strval);
        //Serial.println(str);
        if (str == "1V1" || str == "1V2" || str == "1V3" || str == "1V4" || str == "1V5" ){
        dataVarFloat = (float) strtof((const char *) &data[3], NULL); 
        processWebSocketMessageFloat(str, dataVarFloat);
        //Serial.print(str); Serial.println(dataVarFloat);
        }
        else if (str == "1W1" || str == "1W2" || str == "1W3"){
        dataVarDouble = (double) strtof((const char *) &data[3], NULL); 
        processWebSocketMessageDouble(str, dataVarDouble);
        //Serial.print(str); Serial.println(dataVarFloat);
        }  
        else{
        dataVar = (uint16_t) strtol((const char *) &data[3], NULL, 10);  
        processWebSocketMessage(str, dataVar);
        }
        //Serial.print("dataVar "); Serial.println(dataVar); 
      }
      else { // WIFI UPDATES
        strval[1]=0;
        str=String(strval);
        //Serial.println(str);
          for (int i = 1; i < len+1; i++){
          stringStorage[i-1]=(char)data[i];
          }
        stringStorage[len]=0;
        dataString = String(stringStorage);
        size_t stringLength = sizeof(stringStorage);        
        processWebSocketMessageS(str, stringLength, dataString);     
    }
}
}

void processWebSocketMessageS(String str, int stringLength, String dataString){
    if (str == "I"){
      uint8_t arrayPos =0;
      EEPROMposition = offsetof(storeInEEPROM, ssidStorage[0]);    
        for(int n=EEPROMposition; n < stringLength+EEPROMposition ; n++){
          EEPROM.write(n,stringStorage[arrayPos]);
          arrayPos++;
        }
        EEPROM.write((stringLength+EEPROMposition),NULL);
        EEPROM.commit();
        String mergedString = "RI"+String(dataString); 
        //Serial.print("SSID "); Serial.println(mergedString);
        ws.textAll(mergedString);   
    }
    else if (str == "P"){
      uint8_t arrayPos =0;
      EEPROMposition = offsetof(storeInEEPROM, passStorage[0]);
        for(int n= EEPROMposition; n < stringLength+EEPROMposition ; n++){
          EEPROM.write(n,stringStorage[arrayPos]);
          arrayPos++;
        } 
      EEPROM.write((stringLength+EEPROMposition),NULL);    
      EEPROM.commit();
      String mergedString = "RP"+String(dataString); 
      //Serial.print("PASS "); Serial.println(mergedString);
      ws.textAll(mergedString);
    }   
}

void processWebSocketMessage(String str, int dataVar){
  if (firstChar == "G"){
    if (dataVar == 0){String mergedString = "GA"+String(presetTimer1); ws.textAll(mergedString);}
    else if (dataVar == 1){if (targetTemperature1 > 0){String mergedString = "GC"+String(targetTemperature1); ws.textAll(mergedString);} else {ws.textAll("G");};}
    else if (dataVar == 2){if (targetTemperature2 > 0){String mergedString = "GD"+String(targetTemperature2); ws.textAll(mergedString);} else {ws.textAll("G");};}    
    else if (dataVar == 3){if (alarmTime1 > 0){String mergedString = "GE"+String(alarmTime1); ws.textAll(mergedString);} else {ws.textAll("G");};}
    else if (dataVar == 8){if (alarmTime2 > 0){String mergedString = "GF"+String(alarmTime2); ws.textAll(mergedString);} else {ws.textAll("G");};}
    else if (dataVar == 4){String mergedString = "GG"+String(alarmActive1); ws.textAll(mergedString);}
    else if (dataVar == 5){String mergedString = "GH"+String(alarmActive2); ws.textAll(mergedString);} 
    else if (dataVar == 6){String mergedString = "GI"+String(timerCheck1); ws.textAll(mergedString);}        
    else if (dataVar == 9){if (targetTemperature1 > 0){String mergedString = "GM"+String(alarmReachTemp1); ws.textAll(mergedString);} else {ws.textAll("G");};}
    else if (dataVar == 10){if (targetTemperature2 > 0){String mergedString = "GN"+String(alarmReachTemp2); ws.textAll(mergedString);} else {ws.textAll("G");};}  
    else if (dataVar == 11){ if (offsetTemperature1 > 0) {String mergedString = "GO"+String(offsetTemperature1); ws.textAll(mergedString);} else {ws.textAll("G");};} 
    else if (dataVar == 12){String mergedString = "GQ"+String(fanON); ws.textAll(mergedString);}  
    else if (dataVar == 7){String mergedString = "GR"+String(tempOffsetAlarm); ws.textAll(mergedString);}
    else if (dataVar == 13){String mergedString = "GS"+String(sensorAmount); ws.textAll(mergedString);}
    else if (dataVar == 14){String mergedString = "GT"+String(sensorType); ws.textAll(mergedString);}
    else if (dataVar == 15){String mergedString = "GU"+String(fanManual); ws.textAll(mergedString);}
    else if (dataVar == 16){sendAllCalibrationValues();}
    else if (dataVar == 17){sendAllPIDValues();}
  }
  
  else if (firstChar == "1"){  
      if (str == "1A1"){targetTemperature1 = dataVar; setPoint = dataVar; fanState = !fanON; String mergedString = "GC"+String(targetTemperature1); ws.textAll(mergedString); } // alarmActiveT1 = true;}
      else if (str == "1A2"){targetTemperature2 = dataVar; String mergedString = "GD"+String(targetTemperature2); ws.textAll(mergedString); } // alarmActiveT2 = true;}   
      else if (str == "1B1"){targetTime1 = dataVar*1000; timer = true; startTime1=millis();}
      else if (str == "1C1"){counter1 = dataVar; startCounter1=millis();}
      else if (str == "1C2"){counter2 = dataVar; startCounter2=millis();} 
      else if (str == "1D1"){presetTimer1 = dataVar; timerCheck1 = true; String mergedString = "GA"+String(presetTimer1); ws.textAll(mergedString);}      
      else if (str == "1E1"){timerCheck1 = false; String mergedString = "GI"+String(timerCheck1); ws.textAll(mergedString);}
      else if (str == "1F1"){alarmTime1 = dataVar; String mergedString = "GE"+String(alarmTime1); ws.textAll(mergedString);}
      else if (str == "1F2"){alarmTime2 = dataVar; String mergedString = "GF"+String(alarmTime2); ws.textAll(mergedString);}
      else if (str == "1G1"){alarmActive1 = dataVar; String mergedString = "GG"+String(alarmActive1); ws.textAll(mergedString);}// blinkON = false; digitalWrite(LED, LOW);}
      else if (str == "1G2"){alarmActive2 = dataVar; String mergedString = "GH"+String(alarmActive2); ws.textAll(mergedString);}// blinkON = false; digitalWrite(LED, LOW);}      
      else if (str == "1J1"){alarmReachTemp1 = dataVar; String mergedString = "GM"+String(alarmReachTemp1); ws.textAll(mergedString);}
      else if (str == "1J2"){alarmReachTemp2 = dataVar; String mergedString = "GN"+String(alarmReachTemp2); ws.textAll(mergedString);}
      else if (str == "1K1"){offsetTemperature1 = dataVar; (double) dataVar; myPID.setBangBang(dataVar);  String mergedString = "GO"+String(offsetTemperature1); ws.textAll(mergedString);}
      else if (str == "1M1"){tempOffsetAlarm = dataVar; String mergedString = "GR"+String(tempOffsetAlarm); ws.textAll(mergedString);}
      else if (str == "1L1"){fanON = dataVar; fanState = !fanON; String mergedString = "GQ"+String(fanON); ws.textAll(mergedString);}
      else if (str == "1RR"){alarmMessage = dataVar; alarmMessageTimer = millis();}
      else if (str == "1ST"){sensorType = dataVar; EEPROM.put(offsetof(storeInEEPROM, sensorType), sensorType);  EEPROM.commit(); String mergedString = "GT"+String(sensorType); ws.textAll(mergedString); }
      else if (str == "1SN"){sensorAmount = dataVar; EEPROM.put(offsetof(storeInEEPROM, sensorAmount), sensorAmount);  EEPROM.commit(); ESP.restart(); }
      else if (str == "1T1"){fanManual = dataVar; fanState = !fanON; String mergedString = "GU"+String(fanManual); ws.textAll(mergedString);}
      else if (str == "1U1"){ESP.restart();};
  }    
  else {
      return;
  }
}

void processWebSocketMessageFloat(String str, float dataVar){
  if (firstChar == "1"){
      if (str == "1V1"){calibrationValue[0] = dataVar; EEPROM.put(offsetof(storeInEEPROM, calibrationValue[0]), calibrationValue[0]);  EEPROM.commit(); sendAllCalibrationValues(); }
      else if (str == "1V2"){calibrationValue[1] = dataVar; EEPROM.put(offsetof(storeInEEPROM, calibrationValue[1]), calibrationValue[1]);  EEPROM.commit(); sendAllCalibrationValues(); }
      else if (str == "1V3"){calibrationValue[2] = dataVar; EEPROM.put(offsetof(storeInEEPROM, calibrationValue[2]), calibrationValue[2]);  EEPROM.commit(); sendAllCalibrationValues(); }
      else if (str == "1V4"){calibrationValue[3] = dataVar; EEPROM.put(offsetof(storeInEEPROM, calibrationValue[3]), calibrationValue[3]);  EEPROM.commit(); sendAllCalibrationValues(); }
      else if (str == "1V5"){calibrationValue[4] = dataVar; EEPROM.put(offsetof(storeInEEPROM, calibrationValue[4]), calibrationValue[4]);  EEPROM.commit(); sendAllCalibrationValues(); }  
  }
}

void processWebSocketMessageDouble(String str, double dataVar){
  if (firstChar == "1"){
      if (str == "1W1"){PIDValue[0] = dataVar; KP = dataVar; OUTPUT_MIN = dataVar; sendAllPIDValues(); } // EEPROM.put(offsetof(storeInEEPROM, PIDValue[0]), PIDValue[0]);  EEPROM.commit(); sendAllPIDValues(); } 
      else if (str == "1W2"){PIDValue[1] = dataVar; KI = dataVar; OUTPUT_MAX = dataVar; sendAllPIDValues(); } //  EEPROM.put(offsetof(storeInEEPROM, PIDValue[1]), PIDValue[1]);  EEPROM.commit(); sendAllPIDValues(); }
      else if (str == "1W3"){PIDValue[2] = dataVar; KD = dataVar; sendAllPIDValues(); } // EEPROM.put(offsetof(storeInEEPROM, PIDValue[2]), PIDValue[2]);  EEPROM.commit(); sendAllPIDValues(); }
  }
}


void sendAllTempToClient (){
            String mergedString = "T" + String(temp[0]) + "+" + String(temp[1]) + "+" + String(temp[2]) + "+" + String(temp[3])+ "+" + String(temp[4]) + "+";
            ws.textAll(mergedString); 
            Serial.println(mergedString);
  
}

void sendAllCalibrationValues(){
            String mergedString = "GV" + String(calibrationValue[0], 1) + "+" + String(calibrationValue[1], 1) + "+" + String(calibrationValue[2], 1) + "+" + String(calibrationValue[3], 1)+ "+" + String(calibrationValue[4], 1) + "+"; 
            ws.textAll(mergedString);
            Serial.println(mergedString);
}

void sendAllPIDValues(){
            String mergedString = "GW" + String(PIDValue[0], 4) + "+" + String(PIDValue[1], 4) + "+" + String(PIDValue[2], 4) + "+"; 
            ws.textAll(mergedString);
            Serial.println(mergedString);
}

void updateGraph (float temp){
            String mergedString = "SC"+String(temp);
            ws.textAll(mergedString);                
}

void updateFanSpeed(byte fanSpeed){
            String mergedString = "SI"+String(fanSpeed);
            ws.textAll(mergedString);                
}

void updateGraph2 (float temp){
            String mergedString = "SD"+String(temp);
            ws.textAll(mergedString);                
}

void sendTimeToClient1 (uint16_t Time){
            String mergedString = "SE"+String(Time);
            ws.textAll(mergedString);                
}

void sendCounterToClient1 (uint16_t Time){
            String mergedString = "SG"+String(Time);
            ws.textAll(mergedString);                
}

void sendCounterToClient2 (uint16_t Time){
            String mergedString = "SH"+String(Time);
            ws.textAll(mergedString);                
}
