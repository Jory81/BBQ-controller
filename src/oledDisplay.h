void displayOledScreen(float temp1, float temp2, float temp3, float temp4){

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,32);// 30 is fit
  display.print("bbq");
  display.setCursor(0,56); // 54 is fit
  display.print("food");
  display.setCursor(0,0);
  if (alarmMessage == 0){  
     if (wifiStationMode){
     display.print("wifi "); display.println(WiFi.localIP());
     }
     else {
     display.print("soft-AP "); display.println(WiFi.softAPIP());
     }
  }
  else if (alarmMessage > 0){
    switch(alarmMessage){
      case 1: {display.print("reached targetTemp");} break;
      case 2: {display.print("exceeded tempOffset");} break;
      case 3: {display.print("reached alarmTimer");} break;
    }
  }
  if (sensorAmount > 2){
    display.setCursor(0,8);
    display.print("T3: "); display.print(temp3, 1);
      if (sensorAmount > 3){
      display.print("  T4: "); display.print(temp4, 1);  
    }
  }
  display.setTextSize(3); 
  display.setCursor(24,16);
  display.println(temp1, 1);
  display.setCursor(24,40); 
  display.println(temp2, 1);
  display.display();
}