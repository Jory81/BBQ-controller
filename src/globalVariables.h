String firstChar="0";
String str;
String dataString;
String wifiID;
String wifiPASS;

//const char *WIFI_SSID = "Su***s**l";
//const char *WIFI_PASS = "se*******at13";

uint32_t updateTimeGraph = 5000;
char lightSchedule[9] = "05302030";

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
uint8_t humiditySensorAmount = 0;
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

float humidity[3] = {0,0,0};
float predhtTemp[3] = {0,0,0};
float dhtTemp[3] = {0,0,0};
float preHumidity[3] = {0,0,0};

float calibrationValue[5] = {-0.0, -0.0, -0.0, -0.0, -0.0};

uint32_t Time1=0;
uint32_t cTime1=0;
uint32_t cTime2=0;
//uint16_t targetTemperature1=0;
float targetTemperature1=25.0f;
unsigned long startTime1=0;

boolean humidAlarmActiveLO=true;
boolean humidAlarmActiveHI=false;


unsigned long alarmMessageTimer=0;

boolean lastfanONState=false;
boolean tempControlPID=false; 
byte fanSpeed = 0;

uint8_t humidmin = 30;
uint8_t humidmax = 80;

uint16_t targetTemperature2=60;
uint8_t alarmMessage=0;
//boolean timer2 = false;


boolean alarmReachTemp2 = true; 

boolean heaterON = false;
boolean heaterState = !heaterON;

boolean lightsON = true;
boolean lightState = !lightsON;

boolean humidifierON = false;
boolean humidifierState = !humidifierON;



boolean fanON = false;
boolean msgFanState= true;
boolean tempOffsetAlarmMax=true;
boolean fanManual = false;
boolean wifiStationMode=false;
byte fanManualAmount = 127;

uint32_t targetTime1 = 0;
//uint32_t targetTime2 = 0;

//uint16_t offsetTemperatureMax = 10;
float offsetTemperatureMax=10.0f;
//uint16_t offsetTemperatureMin = 20;
float offsetTemperatureMin = 1.5f;

struct storeInEEPROM {
int check;
uint8_t sensorType;
uint8_t sensorAmount;
uint8_t humiditySensorAmount;
uint8_t humidmin;
uint8_t humidmax;
byte fanManualAmount;
char ssidStorage[32];
char passStorage[32];
float calibrationValue[5];
boolean tempControlPID;
double KP;
double KI;
double KD;
double OUTPUT_MIN;
double OUTPUT_MAX;
char lightSchedule[9];
//uint16_t targetTemperature1;
float targetTemperature1;
//uint16_t offsetTemperatureMax;
float offsetTemperatureMax;
//uint16_t offsetTemperatureMin;
float offsetTemperatureMin;
};

storeInEEPROM customVar = {
      11234, // code to check
      1, // sensorType ; 1 is PT100 2 is thermokoppel
      1, // sensorAmount 
      0, // humidsensoramount
      30, // humidmin
      80, // humidmax
      50, // fanManualAmount
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // ssidstorage
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // passstorage
      0, 0, 0, 0, 0, // calibration values
      0, // boolean tempControlPID;
      5, 3, 1, // PID values
      0, 255, // OUTPUT_MIN OUTPUT_MAX
      "05302030",
      25.0f, // targetTemp
      10.0f, // offsetTempMax
      1.5f // offsetTempMin
    };

char stringStorage[32];
