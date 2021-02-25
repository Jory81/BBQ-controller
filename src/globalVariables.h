String firstChar="0";
String str;
String dataString;
String wifiID;
String wifiPASS;

//const char *WIFI_SSID = "Su***s**l";
//const char *WIFI_PASS = "se*******at13";

double updateTimeGraph = 5000;

const char *WIFI_SSID_AP = "ESP32-AP";
const char *WIFI_PASS_AP =  "";

char strval[5]={0};
uint16_t dataVar=0;
float dataVarFloat =0.0f;
double dataVarDouble =0;

uint16_t EEPROMposition = 0;
uint16_t stringLength;

int check;
uint8_t sensorType = 1;
uint8_t sensorAmount = 1;
char ssidStorage[32];
char passStorage[32];

unsigned long previousMillis=0;
unsigned long previousMillis1=0;
unsigned long previousMillis2=0;
unsigned long previousMillis3=0;

boolean blinkON = false;
boolean setBlink = false;

float temp[5] = {0,0,0,0,0};
float oldtemp[5] = {0,0,0,0,0};

float calibrationValue[5] = {-0.0, -0.0, -0.0, -0.0, -0.0};

uint32_t Time1=0;
uint32_t cTime1=0;
uint32_t cTime2=0;
uint16_t targetTemperature1=0;
unsigned long startTime1=0;
unsigned long startCounter1=0;
unsigned long startCounter2=0;
unsigned long alarmMessageTimer=0;
uint16_t alarmTime1=0;
uint16_t alarmTime2=0;

boolean timer = false;
boolean counter1 = false;
boolean presetTimer1 = false;
boolean presetTimer2 = false;
boolean timerCheck1 = false;
boolean timerCheck2 = false;
boolean alarmActive1 = false;
//boolean alarmActiveT1 = false;
boolean alarmReachTemp1 = false; 
boolean lastfanONState=false;
boolean tempControlPID=false; 
byte fanSpeed = 0;

uint16_t targetTemperature2=0;
uint8_t alarmMessage=0;
//boolean timer2 = false;
boolean counter2 = false;
boolean alarmActive2 = false;
boolean alarmActiveT2 = false;
boolean alarmReachTemp2 = false; 
boolean lightsON = true;
boolean humidifierON = false;
boolean fanON = false;
boolean msgFanState= true;
boolean tempOffsetAlarmMin=false;
boolean tempOffsetAlarmMax=false;
boolean fanManual = false;
boolean wifiStationMode=false;
byte fanManualAmount = 127;

uint32_t targetTime1 = 0;
//uint32_t targetTime2 = 0;

uint16_t offsetTemperatureMax = 5;
uint16_t offsetTemperatureMin = 20;

struct storeInEEPROM {
int check;
uint8_t sensorType;
uint8_t sensorAmount;
char ssidStorage[32];
char passStorage[32];
float calibrationValue[5];
boolean tempControlPID;
double KP;
double KI;
double KD;
double OUTPUT_MIN;
double OUTPUT_MAX;
uint16_t targetTemperature1;
uint16_t offsetTemperatureMax;
uint16_t offsetTemperatureMin;
};

storeInEEPROM customVar = {
      11332, // code to check
      1, // sensorType ; 1 is PT100 2 is thermokoppel
      1, // sensorAmount 
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // ssidstorage
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // passstorage
      0, 0, 0, 0, 0, // calibration values
      0, // boolean tempControlPID;
      5, 3, 1, // PID values
      0, 255, // OUTPUT_MIN OUTPUT_MAX
      50, // targetTemp
      10, // offsetTempMax
      30 // offsetTempMin
    };

char stringStorage[32];
