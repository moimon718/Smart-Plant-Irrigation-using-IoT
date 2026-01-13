#include "WiFi.h" 
#include <HTTPClient.h>
#include <ArduinoJson.h> //DOWNLOAD THIS LIBRARY FILE(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)
#include "DHT.h"
bool tankEmptyLatched = false;
bool rainEventLatched = false;
bool soilWasWet = true;// TO STOP PUMP TRIGERRING AGAIN AND AGAIN
unsigned long rainPredictionStartTime = 0;
const unsigned long RAIN_GRACE_PERIOD = 3 * 60 * 60 * 1000; // 3 hours
int CRITICAL_MOISTURE_OFFSET = 10; // % below threshold
unsigned long lastWeatherCheck = 0;
const unsigned long WEATHER_INTERVAL = 2 * 60 * 60 * 1000; // 2 hours
bool rainPredicted = false;
bool rainConfirmed = false;
//ENTER API KEY AGAIN OF WEATHER APP
String city = "Kolkata";// HOOGLY HOLE HOOGLY, SITE E SELECT KORTE HOBE
const char* ssid = ""; //SEPARATE DONT INTERFARE
const char* password = ""; 
//const char* pass = ""; 
unsigned long lastLoopTime = 0;
const unsigned long LOOP_INTERVAL = 10000; // 10 seconds
#define BLYNK_TEMPLATE_ID "TMPL3pzSYueE5" 
#define BLYNK_TEMPLATE_NAME "Iot plant irrigation " 
#define BLYNK_AUTH_TOKEN "zY1k_9SLEGHwoLg-qNOG0iXA06_7N1VC" 
#define RAIN_SENSOR_PIN 25   // Digital output from rain sensor (USE ANY CONVENIENT PIN)
bool rainDetected = false;   // Physical rain status
#define DHTPIN 27     
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321 
//#define DHTTYPE DHT21   // DHT 21 (AM2301) 
const int trigPin = 12; 
const int echoPin = 13; 
//define sound speed in cm/uS 
#define SOUND_SPEED 0.034 
#define CM_TO_INCH 0.393701 
long duration; 
float distanceCm; 
const int pH_pin = 35; // Use an analog-capable pin on ESP32 
// Calibration values 
float offset = 0.00; // You can fine-tune this 
float voltageToPhFactor = 3.5; // Approximate scaling factor 
DHT dht(DHTPIN, DHTTYPE); 
int _moisture, sensor_analog;  
int pin = 32; 
const int soilPin = 34; /* Soil moisture sensor O/P pin  ANALOG 
PIN */ 
unsigned long lastPhReadTime = 0;
const unsigned long PH_INTERVAL = 604800000UL; // 7 days in ms *******60000UL; // 1 minute (TESTING)**********
float lastPhValue = 7.0;
//bool irrigationDone = false;
bool pumpRunning = false;
unsigned long pumpStartTime = 0;
unsigned long currentPumpTime = 0;
String soilType = "Neutral";
// -------- Irrigation parameters --------
int moistureThreshold = 40;   // default %
// -------- Water Saving Calculation --------
float pumpFlowRate = 2.0; // liters per minute (change as per pump)
float totalWaterSaved = 0.0; // liters
unsigned long basePumpTime = 5000; // 5 seconds
unsigned long deltaTime = 2000;    // ±2 seconds
#ifndef BlynkSimpleEsp32_h 
#define BlynkSimpleEsp32_h 
#ifndef ESP32 
#error This code is intended to run on the ESP32 platform! Please 
check your Tools->Board setting. 
#endif 
#define BLYNK_SEND_ATOMIC 
#include <BlynkApiArduino.h> 
#include <Blynk/BlynkProtocol.h> 
#include <Adapters/BlynkArduinoClient.h> 
#include <WiFi.h> 
typedef BlynkArduinoClientGen<WiFiClient> BlynkEsp32Client; 
class BlynkWifi 
    : public BlynkProtocol<BlynkEsp32Client> 
{ 
    typedef BlynkProtocol<BlynkEsp32Client> Base; 
public: 
BlynkWifi(BlynkEsp32Client& transp) 
        : Base(transp) 
{} 
void connectWiFi(const char* ssid, const char* pass) 
{ 
BLYNK_LOG2(BLYNK_F("Connecting to "), ssid); 
WiFi.mode(WIFI_STA); 
if (pass && strlen(pass)) { 
WiFi.begin(ssid, pass); 
} else { 
WiFi.begin(ssid); 
} 
while (WiFi.status() != WL_CONNECTED) { 
BlynkDelay(500); 
} 
BLYNK_LOG1(BLYNK_F("Connected to WiFi")); 
        IPAddress myip = WiFi.localIP(); 
(void)myip; // Eliminate warnings about unused myip 
BLYNK_LOG_IP("IP: ", myip); 
} 
void config(const char* auth, const char* domain = BLYNK_DEFAULT_DOMAIN, uint16_t port= BLYNK_DEFAULT_PORT) 
{ 
   Base::begin(auth); 
this->conn.begin(domain, port);
} 
void config(const char* auth, IPAddress ip, uint16_t port= BLYNK_DEFAULT_PORT)    
{ 
 Base::begin(auth); 
this->conn.begin(ip, port); 
} 
void begin(const char* auth, const char* ssid, const char* pass, const char* domain = BLYNK_DEFAULT_DOMAIN, uint16_t port= BLYNK_DEFAULT_PORT) 
{ 
connectWiFi(ssid, pass); 
config(auth, domain, port); 
while(this->connect() != true) {} 
} 
void begin(const char* auth, const char* ssid, const char* pass, IPAddress ip, uint16_t port= BLYNK_DEFAULT_PORT)     
{ 
connectWiFi(ssid, pass); 
config(auth, ip, port); 
while(this->connect() != true) {} 
} 
}; 
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_BLYNK) 
static WiFiClient _blynkWifiClient; 
static BlynkEsp32Client _blynkTransport(_blynkWifiClient); 
  BlynkWifi Blynk(_blynkTransport); 
#else 
extern BlynkWifi Blynk; 
#endif 
#include <BlynkWidgets.h> 
#endif 
void setup() { 
Blynk.setProperty(V13, "urls", 1, "https://thumbnailer.mixptp.com/get?url=https://drive.google.com/uc?id=1Q3fOLe-4uDSnqxlYrVhZlwpMENP1JUXC");
Blynk.virtualWrite(V13, 1);
analogReadResolution(12); 
Serial.begin(115200); 
pinMode(pin, OUTPUT); /* Set the baudrate to 115200*/ 
dht.begin(); 
pinMode(RAIN_SENSOR_PIN, INPUT);
pinMode(soilPin, INPUT); 
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output 
pinMode(echoPin, INPUT); // Sets the echoPin as an Input 
WiFi.begin(ssid, password); 
while (WiFi.status() != WL_CONNECTED) { 
delay(5000); 
Serial.println("Connecting to WiFi.."); 
} 
Serial.println("Connected to the WiFi network"); 
Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password); 
} 
void readRainSensor() {
  rainDetected = (digitalRead(RAIN_SENSOR_PIN) == LOW);

  if (rainDetected) {
    rainConfirmed = true;
    rainEventLatched = true; 
    rainPredictionStartTime = 0;
    Serial.println("🌧 Rain detected by sensor");
  } else {
    rainConfirmed = false;   // ✅ ADD THIS LINE
  }
}
void updateSoilPh() {
  int sensorValue = analogRead(pH_pin);
  float voltage = sensorValue * (3.3 / 4095.0);
  lastPhValue = voltageToPhFactor * voltage + offset;

  if (lastPhValue < 6.5) {
    soilType = "Acidic";
    moistureThreshold = 45;
  }
  else if (lastPhValue <= 7.5) {
    soilType = "Neutral";
    moistureThreshold = 40;
  }
  else {
    soilType = "Basic";
    moistureThreshold = 35;
  }
  Serial.println("Weekly pH Update:");
  Serial.print("pH = ");
  Serial.println(lastPhValue);
  Serial.print("Soil Type = ");
  Serial.println(soilType);
}
bool checkRainForecast() {
  HTTPClient http;

  String url = "http://api.openweathermap.org/data/2.5/forecast?q="+ city +"&appid="+ weatherApiKey; //UPDATE THE DETAILS

  http.begin(url);
  int httpCode = http.GET();

  if (httpCode != 200) { //200 MEANS SUCCESFULLY DATA FETCHED
    http.end();
    return false; // Fail-safe
  }

  String payload = http.getString();
  http.end();

  StaticJsonDocument<4096> doc;
  DeserializationError error = deserializeJson(doc, payload);
if (error) {
  Serial.println("JSON parse failed");
  return false;
}
for (int i = 0; i < 4; i++) { // next 12 hours
  float rain = doc["list"][i]["rain"]["3h"] | 0;
  if (rain > 0.0) {
    return true;
  }
}
return false;
}
BLYNK_WRITE(V6) {
  if (param.asInt() == 1) {
    updateSoilPh();
    lastPhReadTime = millis(); // reset periodic timer

    Blynk.virtualWrite(V7, lastPhValue);
    Blynk.virtualWrite(V8, soilType);

    Serial.println("Manual pH update triggered from Blynk");
  }
}
void loop() { 
  Blynk.run();
  unsigned long now = millis();
bool doTimedLoop = false;
if (now - lastLoopTime >= LOOP_INTERVAL) { //millis part, remove if required
  lastLoopTime = now;
  doTimedLoop = true;
}
if(doTimedLoop){
  readRainSensor();
if (millis() - lastWeatherCheck >= WEATHER_INTERVAL) {
  rainPredicted = checkRainForecast();
  lastWeatherCheck = millis();
  rainPredicted = rainPredicted || rainDetected;
  if (rainPredicted && rainPredictionStartTime == 0) {
  rainPredictionStartTime = millis();  // start waiting for rain
}
  Serial.println(rainPredicted ? "Rain predicted → Irrigation will be skipped":"No rain predicted");
}
else {
  // Immediate sensor override (no waiting 2 hours)
  if (rainDetected) {
    rainPredicted = true;
  }
}
bool predictionExpired = false;
if (rainPredictionStartTime != 0 && millis() - rainPredictionStartTime > RAIN_GRACE_PERIOD) {
  predictionExpired = true;
}
if (predictionExpired) {
  rainPredicted = false;
  rainPredictionStartTime = 0;
}
 unsigned long currentMillis = millis();
if (currentMillis - lastPhReadTime >= PH_INTERVAL || lastPhReadTime == 0) {
  updateSoilPh();
  lastPhReadTime = currentMillis;
  Blynk.virtualWrite(V7, lastPhValue);
  Blynk.virtualWrite(V8, soilType);
}
digitalWrite(trigPin, LOW); 
delayMicroseconds(2); 
digitalWrite(trigPin, HIGH); 
delayMicroseconds(10); 
digitalWrite(trigPin, LOW); 
duration = pulseIn(echoPin, HIGH, 30000); // 30 ms timeout otherwise may lead to esp32 hang
if (duration == 0) 
{
  Serial.print("Ultrasonic Timeout");
  return;
}
distanceCm = duration * SOUND_SPEED/2;   
Serial.print("Distance (cm): "); 
Serial.println(distanceCm); 
int l = constrain(100 - distanceCm, 0, 100); 
Blynk.virtualWrite(V3,l); 
if (distanceCm <15) { 
tankEmptyLatched = false;   // ✅ RESET latch when water is present
int soilMoistureValue = analogRead(soilPin); 
Serial.print("Soil Moisture: "); 
Serial.println(soilMoistureValue); 
int moisturepercent=map(soilMoistureValue,4095,0,0,100); 
int criticalMoisture = moistureThreshold - CRITICAL_MOISTURE_OFFSET;
Blynk.virtualWrite(V4,moisturepercent); 
// Read temperature & humidity
float h = dht.readHumidity();
float t = dht.readTemperature();
unsigned long pumpTime = basePumpTime;
// Temperature & humidity correction
if (t > 30 && h < 40) {
  pumpTime += deltaTime;
}
else if (t < 20 && h > 70) {
  pumpTime -= deltaTime;
}
if ((rainConfirmed||rainEventLatched) && moisturepercent < moistureThreshold) {
  float waterSaved =(pumpFlowRate / 60.0) * (pumpTime / 1000.0);
  totalWaterSaved += waterSaved;
  Serial.print("Actual rain → Water saved: ");
  Serial.print(waterSaved);
  Serial.println(" L");
  rainConfirmed = false; // consume event
  rainEventLatched = false; 
}
if (rainDetected && !predictionExpired) {
  pumpRunning = false;
  digitalWrite(pin, LOW);
}
if (moisturepercent < criticalMoisture) { //SOIL IS CRITICALLY DRY, WATER SOIL ANYWAY DESPITE PREDICTION
  Serial.println("⚠ Emergency irrigation override");
  soilWasWet = false;   // 🔧 ADD THIS
  rainPredicted = false;
  rainPredictionStartTime = 0;
  pumpRunning = true;
  pumpStartTime = millis();
  currentPumpTime = pumpTime;
  digitalWrite(pin, HIGH);
}
else if (rainPredicted && !predictionExpired) { //AKA RAIN PREDICTION FOR LONG TIME BUT NO RAIN ACTUALLY OCCURED, SOIL IS CRITICALLY DRY NOW, SO WATER ANYWAY, IGNORE PREDICTION
  pumpRunning = false;
  digitalWrite(pin, LOW);
}
else if (moisturepercent < moistureThreshold && soilWasWet && !pumpRunning) {
  soilWasWet = false;
  Serial.println("Irrigation ON");
  pumpRunning = true;
  pumpStartTime = millis();
  currentPumpTime = pumpTime;
  digitalWrite(pin, HIGH);
}
if (pumpRunning && millis() - pumpStartTime >= currentPumpTime) {
  Serial.println("Irrigation OFF");
  pumpRunning = false;
  digitalWrite(pin, LOW);

  // RESET weather logic
  rainPredicted = false;
  rainPredictionStartTime = 0;
}
if (moisturepercent >= moistureThreshold) {
  soilWasWet = true;
  pumpRunning = false;
  digitalWrite(pin, LOW);
}
Serial.print(F("%  Humidity: "));
Serial.print(h); 
Blynk.virtualWrite(V1,h); 
Serial.print(F("%  Temperature: ")); 
Serial.print(t); 
Blynk.virtualWrite(V0,t); 
Serial.print(F("°C ")); 
} 
else 
{ 
Serial.println("Bottle bhoro taratari!!!!!"); 
Blynk.virtualWrite(V5, "TANK IS EMPTY!! REFILL TANK!"); 
if (!tankEmptyLatched) {                 // ✅ LATCH CHECK
    Blynk.logEvent("tank_empty_event", "Tank is empty! Please refill.");
    tankEmptyLatched = true;               // ✅ LATCH SET
  }
digitalWrite(pin, LOW); 
} 
Blynk.virtualWrite(V11, totalWaterSaved);// for total
Blynk.virtualWrite(V12, totalWaterSaved);// for graph
//delay(10000); NOT NEEDED ANYMORE 
}
}
