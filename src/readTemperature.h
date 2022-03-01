/* checks if there are faults 
 *  
 */

float processRTD(uint16_t rtd){
  uint16_t ohmsx100 ;
  uint32_t dummy ;
  float ohms, Tlut ;  
  float Tcvd, Tcube, Tpoly, Trpoly ;    

  dummy = ((uint32_t)(rtd << 1)) * 100 * ((uint32_t) floor(RREF)) ;
  dummy >>= 16 ;
  ohmsx100 = (uint16_t) (dummy & 0xFFFF) ;
  ohms = (float)(ohmsx100 / 100) + ((float)(ohmsx100 % 100) / 100.0) ; 
  Tlut  = PT100.celsius(ohmsx100) ;     // NoobNote: LUT== LookUp Table
  return Tlut;
}

//void checkFault(void)
//{
//  // Check and print any faults
//  uint8_t fault = maxthermo.readFault();
//  if (fault)
//  {
//    Serial.print("Fault 0x"); Serial.println(fault, HEX);
//    if (fault & MAX31865_FAULT_HIGHTHRESH)
//    {
//      Serial.println("RTD High Threshold"); 
//    }
//    if (fault & MAX31865_FAULT_LOWTHRESH)
//    {
//      Serial.println("RTD Low Threshold"); 
//    }
//    if (fault & MAX31865_FAULT_REFINLOW)
//    {
//      Serial.println("REFIN- > 0.85 x Bias"); 
//    }
//    if (fault & MAX31865_FAULT_REFINHIGH)
//    {
//      Serial.println("REFIN- < 0.85 x Bias - FORCE- open"); 
//    }
//    if (fault & MAX31865_FAULT_RTDINLOW)
//    {
//      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open"); 
//    }
//    if (fault & MAX31865_FAULT_OVUV)
//    {
//      Serial.println("Under/Over voltage"); 
//    }
//    maxthermo.clearFault();
//  }
//}

//void checkFault2(void)
//{
//  // Check and print any faults
//  uint8_t fault = maxthermo2.readFault();
//  if (fault)
//  {
//    Serial.print("Fault 0x"); Serial.println(fault, HEX);
//    if (fault & MAX31865_FAULT_HIGHTHRESH)
//    {
//      Serial.println("RTD High Threshold"); 
//    }
//    if (fault & MAX31865_FAULT_LOWTHRESH)
//    {
//      Serial.println("RTD Low Threshold"); 
//    }
//    if (fault & MAX31865_FAULT_REFINLOW)
//    {
//      Serial.println("REFIN- > 0.85 x Bias"); 
//    }
//    if (fault & MAX31865_FAULT_REFINHIGH)
//    {
//      Serial.println("REFIN- < 0.85 x Bias - FORCE- open"); 
//    }
//    if (fault & MAX31865_FAULT_RTDINLOW)
//    {
//      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open"); 
//    }
//    if (fault & MAX31865_FAULT_OVUV)
//    {
//      Serial.println("Under/Over voltage"); 
//    }
//    maxthermo2.clearFault();
//  }
//}

void fanControl(){
if (fanManual){
  if (fanON){ outputVal=fanManualAmount; }
  else { outputVal=0; }
  if (msgFanState){
    messageFanState();
  }
}
else if (!fanManual){
  if (!tempControlPID){
    if (temp[0] > targetTemperature1){
      outputVal = 0;
      fanON = false;
    }
    else if (temp[0] < targetTemperature1){
      fanON = true;
      if(temp[0] < targetTemperature1 - offsetTemperatureMin){
        outputVal = OUTPUT_MAX;
      }
      else if (temp[0] > targetTemperature1 - offsetTemperatureMin){
        outputVal = modifiedMap((targetTemperature1-temp[0]), 0, offsetTemperatureMin, OUTPUT_MIN, OUTPUT_MAX);
      } 
    }
  }
  else {
      temperature=temp[0];
      myPID.run(); //call every loop, updates automatically at certain time interval
      if (outputVal < 5){
        fanON = false;
      }
      else{
        fanON = true;
      }
  }
  if (lastfanONState != fanON || msgFanState == true){
  lastfanONState = fanON;
  messageFanState();
  }   
}
//Serial.println(outputVal);
ledcWrite(ledChannel1, outputVal);
ledcWrite(ledChannel2, outputVal);
fanSpeed = map(outputVal, 0, 255, 0, 100);
updateFanSpeed(fanSpeed);
}

void messageFanState(){
digitalWrite(LED, fanON);
String mergedString = "GQ"+String(fanON); ws.textAll(mergedString);
msgFanState = false;
return;
}

double modifiedMap(double x, double in_min, double in_max, double out_min, double out_max)
{
 double temp = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
 temp = (int) (4*temp + .5);
 return (double) temp/4;
}

void updateTimeAndGraph ()
{   
      if (millis()-previousMillis >= updateTimeGraph){
        updateGraph (oldtemp[0]);
        if (sensorAmount > 1){
        updateGraph2 (oldtemp[1]);
        }
        previousMillis = millis();
    }
        
      if (timer){
      Time1 = (targetTime1-(millis()-startTime1))/1000;
        if (Time1 < targetTime1/1000){   
        sendTimeToClient1 (Time1); 
        }    
        if (millis()-startTime1 >= targetTime1){
        timer = false;
        }
      }
      
      if (counter1){
      cTime1 = (millis()-startCounter1)/1000;
      sendCounterToClient1 (cTime1);     
      }
      
      if (counter2){
      cTime2 = (millis()-startCounter2)/1000;
      sendCounterToClient2 (cTime2);     
      }  
      
      if (millis() - alarmMessageTimer >= 15000){
      alarmMessage = 0;
      }
}
