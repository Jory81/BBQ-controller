String firstChar="0";
String str;
String dataString;
String wifiID;
String wifiPASS;

//const char *WIFI_SSID = "Su***s**l";
//const char *WIFI_PASS = "se*******at13";

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
double PIDValue[5] = {-0.0, -0.0, -0.0};

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
boolean timerCheck1 = false;
boolean alarmActive1 = false;
//boolean alarmActiveT1 = false;
boolean alarmReachTemp1 = false; 
boolean fanState=false;
boolean lastTempState=false;
boolean tempState=false;
byte fanSpeed = 0;

uint16_t targetTemperature2=0;
uint8_t alarmMessage=0;
//boolean timer2 = false;
boolean counter2 = false;
//boolean presetTimer2 = false;
//boolean timerCheck2 = false;
boolean alarmActive2 = false;
boolean alarmActiveT2 = false;
boolean alarmReachTemp2 = false; 
boolean fanON = false;
boolean tempOffsetAlarm=false;
boolean fanManual = false;
boolean wifiStationMode=false;

uint32_t targetTime1 = 0;
//uint32_t targetTime2 = 0;

uint16_t offsetTemperature1 = 20;

struct storeInEEPROM {
int check;
uint8_t sensorType;
uint8_t sensorAmount;
char ssidStorage[32];
char passStorage[32];
float calibrationValue[5];
double PIDValue[3];
};

storeInEEPROM customVar = {
      22553, // code to check
      1, // sensorType ; 1 is PT100 2 is thermokoppel
      1, // sensorAmount 
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // ssidstorage
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // passstorage
      0, 0, 0, 0, 0, // calibration values
      0, 0, 0 // PID values
    };

char stringStorage[32];
