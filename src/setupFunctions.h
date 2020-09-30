void setupESP32(){
  Serial.begin(115200);   
  pinMode(LED,OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(RELAY,OUTPUT);
}
void setupOledScreen(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  display.clearDisplay();
  display.setTextSize(1); 
  display.setTextColor(WHITE,BLACK);  
  display.setCursor (0,0);  
}
void setupSPIFFS(){
  if( !SPIFFS.begin()){
    Serial.println("Error mounting SPIFFS");
    while(1);
  }  
}
void setupEEPROM(){
  EEPROM.begin(255);
  
  #ifdef ESP8266
  int check;
  EEPROM.get(0, check);
  #else
  int check  = EEPROM.readInt(0);
  #endif
  
  if (check == 016520){
  display.print(F("code: ")); display.println(check);
  display.println(F("EEPROM SET"));
  display.display();
  initializeEEPROMvariables(); // functio in global_variables.h at the bottom.    
  delay(1000);
  }
  
  else if (check != 016520){
  display.println(F("EEPROM not initialized"));
  display.println(F("Write to EEPROM"));
  display.display();
  int eeAddress=0;
  EEPROM.put(eeAddress, customVar);
  EEPROM.commit();
  display.println(F("EEPROM is initialized"));
  display.display();
  delay(2000);
  ESP.restart();
  }
  display.clearDisplay();  
}

void setupWIFI(){
  int wifiCounter=0;
  WiFi.mode(WIFI_STA);
  const char *WIFI_SSID = &wifiID[0];
  const char *WIFI_PASS = &wifiPASS[0];
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.printf("Trying to connect [%s] ", WiFi.macAddress().c_str());
  while (WiFi.status() != WL_CONNECTED) {
      Serial.println(".");
       display.clearDisplay();
      display.setCursor(0,0);
      display.println(F("connecting to wifi..."));
      display.display();
      delay(500);
      wifiCounter++;
      if (wifiCounter > 20){
      break;  
      }
  }
  if (wifiCounter >20){
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_SSID_AP, WIFI_PASS_AP);
  Serial.println(F("soft-AP IP:"));
  display.println(F("soft-AP IP:"));
  Serial.println(WiFi.softAPIP());
  display.println(WiFi.softAPIP());
  display.display();    
  }
  Serial.printf(" %s\n", WiFi.localIP().toString().c_str());
  Serial.printf(" %s\n", WiFi.softAPIP().toString().c_str());
  
  display.clearDisplay();
  display.setCursor(0,0);

  display.printf(" %s\n", WiFi.localIP().toString().c_str());
  display.printf(" %s\n", WiFi.softAPIP().toString().c_str());
    
  initWebSocket();
  initWebServer();
    
  display.println(F("setup complete"));
//    display.println(F("mDNS: esp32.local/"));
  display.display();  
  
  delay(1000);   
}

void setupTempSensors(){
  if (sensorType == 1){
    for (int sensor = 0; sensor <sensorAmount; sensor++){
    maxthermo[sensor].begin(MAX31865_2WIRE);  // set to 3WIRE or 4WIRE as necessary
    //maxthermo[sensor].begin(MAX31865_3WIRE);  // set to 2WIRE or 4WIRE as necessary
    //maxthermo[sensor].begin(MAX31865_4WIRE);  // set to 2WIRE or 3WIRE as necessary
    }
  }
  else {
    for (int sensor = 0; sensor <sensorAmount; sensor++){    
    thermocouple[0] = new MAX6675_Thermocouple(SCK_PIN, CS_PIN, SO_PIN); // sensor 1
    thermocouple[1] = new MAX6675_Thermocouple(SCK_PIN, CS2_PIN, SO_PIN); // sensor 2
    
  }
}
}

void initializeEEPROMvariables(){
sensorType  = EEPROM.read(offsetof(storeInEEPROM, sensorType));
sensorAmount = EEPROM.read(offsetof(storeInEEPROM, sensorAmount));
 
  for (int m = 0; m < 32; m++){
  int offsetPosition = offsetof(storeInEEPROM, ssidStorage[0]);
  ssidStorage[m]  = EEPROM.read(offsetPosition+m);
  }
  wifiID = String(ssidStorage);
  Serial.print("wifiID "); Serial.println(wifiID);
  
  for (int m = 0; m < 32; m++){
  //Serial.print(m); Serial.print("m ");   
  int offsetPosition = offsetof(storeInEEPROM, passStorage[0]);
  passStorage[m]  = EEPROM.read(offsetPosition+m);
  }
  wifiPASS = String(passStorage);
  Serial.print("wifiPASS "); Serial.println(wifiPASS);

}
