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
      
      if (millis() - alarmMessageTimer >= 15000){
      alarmMessage = 0;
      }
}
