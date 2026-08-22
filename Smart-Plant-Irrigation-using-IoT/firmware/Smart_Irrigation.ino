/*********************** DEFINES & GLOBAL VARIABLES ************************/
#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID" 
#define BLYNK_TEMPLATE_NAME "YOUR_BLYNK_TEMPLATE_NAME" 
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"
bool contributorsShown = false; // All flags are by default false
bool fertilizerShownOnce = false;
bool fertilizerCalculated = false;
bool soilSensorFault = false;
bool dhtFault = false;
bool ultrasonicFault = false;
bool phSensorFault = false;
bool pumpFault = false;
bool rainSensorFault = false;
bool offlineMode = false;
unsigned long lastReconnectAttempt = 0;
const unsigned long RECONNECT_INTERVAL = 10000; // 10 sec
String fertilizerName = "Not calculated";
String fertilizerBrand = "N/A";
String selectedCrop = "Others";
float fertilizerAmount = 0.0;   // kg per acre
bool tankEmptyLatched = false;
bool rainEventLatched = false;
bool soilWasWet = true;// TO STOP PUMP TRIGERRING REPEATEDLY
unsigned long rainPredictionStartTime = 0;
const unsigned long RAIN_GRACE_PERIOD = 3 * 60 * 60 * 1000; // 3 hours
int CRITICAL_MOISTURE_OFFSET = 10; // % below threshold
unsigned long lastWeatherCheck = 0;
const unsigned long WEATHER_INTERVAL = 2 * 60 * 60 * 1000; // 2 hours // MAX 1000 CALLS PER DAY 
bool rainPredicted = false;
bool rainConfirmed = false;
unsigned long lastLoopTime = 0;
const unsigned long LOOP_INTERVAL = 10000;// 10 seconds
#define RAIN_AO_PIN 33     
#define RAIN_SAMPLES 15
int rainThreshold = 3900;  
bool rainDetected = false;   
#define DHTPIN 27     
#define DHTTYPE DHT11   
const int trigPin = 12; 
const int echoPin = 13; 
#define SOUND_SPEED 0.034 
long duration; 
float distanceCm; 
const int pH_pin = 35; 
float offset = 0.00;  
float voltageToPhFactor = 3.5; // Approximate scaling factor   
int pin = 32; 
const int soilPin = 34; /* Soil moisture sensor O/P pin  ANALOG PIN */ 
unsigned long lastPhReadTime = 0;
const unsigned long PH_INTERVAL = 604800000UL; 
float lastPhValue = 7.0;
bool pumpRunning = false;
unsigned long pumpStartTime = 0;
unsigned long currentPumpTime = 0;
String soilType = "Neutral";  //Default value
int moistureThreshold = 40;   // default %
float pumpFlowRate = 2.0; // liters per minute (change as per pump)
float totalWaterSaved = 0.0; // liters
unsigned long basePumpTime = 5000;

/*********************** CONFIGURATION — DO NOT COMMIT SECRETS ************************/

String weatherApiKey = "YOUR_OPENWEATHERMAP_API_KEY"; //ENTER API KEY OF WEATHER APP
String city = "YOUR_CITY"; //SELECT CITY
const char* ssid = "YOUR_WIFI_SSID"; //INPUT SSID
const char* password = "YOUR_WIFI_PASSWORD"; //INPUT PASSWORD
float lat = YOUR_LATITUDE; // INPUT LATITUDE AND LONGITUDE OF FARM
float lon = YOUR_LONGITUDE;

/*********************** LIBRARIES ************************/
#include "WiFi.h" 
#include <BlynkSimpleEsp32.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> 
#include "DHT.h" 

/*********************** OBJECTS ************************/
DHT dht(DHTPIN, DHTTYPE);
WidgetTerminal contributorsTerminal(V18);
WidgetTerminal terminal(V9); //FOR FERTILIZER DISPLAY ON BLYNK 


/*********************** UTILITY FUNCTIONS ************************/
void calculateFertilizer(float pH) // FERILIZER RECOMMENDATION BLOCK
{
  if (pH <= 6.0) {
    fertilizerName = "Agricultural Lime";
    fertilizerBrand = "Tata Agrico / Coromandel Lime";
    fertilizerAmount = 250;  // kg per acre
  }
  else if (pH <= 6.7) {
    fertilizerName = "Dolomite Lime";
    fertilizerBrand = "IPL Dolomite / Rashtriya Chemicals";
    fertilizerAmount = 150;
  }
  else if (pH <= 7.5) {
    fertilizerName = "No fertilizer needed";
    fertilizerBrand = "—";
    fertilizerAmount = 0;
  }
  else if (pH <= 8.0) {
    fertilizerName = "Gypsum / Sulfur";
    fertilizerBrand = "Coromandel Gypsum / Tata Sulphur";
    fertilizerAmount = 100;
  }
  else {
    fertilizerName = "Elemental Sulfur";
    fertilizerBrand = "Zuari Sulphur / IFFCO Sulphur";
    fertilizerAmount = 200;
  }
  fertilizerCalculated = true;
}
void sendToTerminal() 
{
  if (offlineMode) return;
  if (!fertilizerCalculated)
  {
    Serial.println("Fertilizer cannot be determined\n");
  }
  terminal.clear();
  String msg = "";
  msg+="\n\n\n\n\n\n\n\n";
  msg+="Fertilizer     : " + fertilizerName+"\n";
  msg+="Brand          : " + fertilizerBrand+"\n";
  msg+="Amount         : " + String(fertilizerAmount,1) + " kg/acre";
  terminal.print(msg);
  terminal.flush();
}
void showContributors() //CREDIT BLOCK
{
  if (offlineMode) return;
  contributorsTerminal.clear();
  contributorsTerminal.println("\n\n");
  contributorsTerminal.println("=== PROJECT CONTRIBUTORS ===");
  contributorsTerminal.println("Contributor information");
  contributorsTerminal.flush();
}

BLYNK_CONNECTED()
{
 delay(500);

  if (!contributorsShown && !offlineMode)
  {
    showContributors();
    contributorsShown = true;
  }
  sendToTerminal();
}
void updateCropParameters() // SPECIFIC CROP SELECTION LOGIC
{
  if (selectedCrop == "Rice") {
    moistureThreshold = 55;
    basePumpTime = 7000;
    soilType = "Clayey";
  }
  else if (selectedCrop == "Potato") {
    moistureThreshold = 45;
    basePumpTime = 5000;
    soilType = "Loamy";
  }
  else if (selectedCrop == "Wheat") {
    moistureThreshold = 50;
    basePumpTime = 5500;
    soilType = "Loamy";
  }
  else if (selectedCrop == "Onion") {
    moistureThreshold = 43;
    basePumpTime = 4800;
    soilType = "Sandy Loam";
  }
  else if (selectedCrop == "Tomato") {
    moistureThreshold = 48;
    basePumpTime = 5200;
    soilType = "Loamy";
  }
  else {
    
    if (lastPhValue < 6.5) moistureThreshold = 45;
    else if (lastPhValue <= 7.5) moistureThreshold = 40;
    else moistureThreshold = 35;
    basePumpTime = 5000;
  }
}
void reportPriorityFault() // FAULT DETECTION DISPLAY
{
    if(!offlineMode)
    {
    if (soilSensorFault)
    {
        Blynk.virtualWrite(V25, "Soil Moisture Error");
    }
    else if (ultrasonicFault)
    {
        Blynk.virtualWrite(V25, "Ultrasonic Error");
    }
    else if (phSensorFault)
    {
        Blynk.virtualWrite(V25, "pH Sensor Error");
    }
    else if (rainSensorFault)
    {
        Blynk.virtualWrite(V25, "Rain Sensor Error");
    }
    else if (dhtFault)
    {
        Blynk.virtualWrite(V25, "DHT11 Error");
    }
    else if (pumpFault)
    {
        Blynk.virtualWrite(V25, "Pump Dry Run Error");
    }
    else
    {
        Blynk.virtualWrite(V25, "All Sensors OK");
    }
    }
}
void handleConnectivity() {
  // Check WiFi
  if (WiFi.status() != WL_CONNECTED) {
    offlineMode = true;

    if (millis() - lastReconnectAttempt > RECONNECT_INTERVAL) {
      Serial.println("Reconnecting WiFi...");
      WiFi.disconnect();
      WiFi.begin(ssid, password);
      lastReconnectAttempt = millis();
    }
    return; // Skip Blynk if WiFi is down
  }

  // WiFi is OK, check Blynk
  if (!Blynk.connected()) {
    Serial.println("Reconnecting Blynk...");
    if (Blynk.connect(3000)) {
      Serial.println("Blynk reconnected");
      offlineMode = false;
    } else {
      offlineMode = true;
    }
  } else {
    offlineMode = false;
  }
}

/*********************** SETUP WIZARD ************************/
void setup() { 
updateCropParameters();
analogReadResolution(12); 
Serial.begin(115200); 
pinMode(pin, OUTPUT); 
dht.begin(); 
pinMode(RAIN_AO_PIN, INPUT);
pinMode(soilPin, INPUT); 
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output 
pinMode(echoPin, INPUT); // Sets the echoPin as an Input 
WiFi.begin(ssid, password);
unsigned long startAttemptTime = millis();
while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 15000) { //CHANGE IF REQUIRED
  delay(500);
  Serial.println("Connecting to WiFi...");
}

if (WiFi.status() == WL_CONNECTED) {
Serial.println("Connected to the WiFi network"); 
Blynk.config(BLYNK_AUTH_TOKEN);   
Blynk.connect(3000);
}
else {
  Serial.println("Starting in OFFLINE MODE");
  offlineMode = true;
} 
lastWeatherCheck = millis() - WEATHER_INTERVAL;
} 

/***********************RAIN SENSOR BLOCK ************************/
void readRainSensor() {

  long total = 0;

  for (int i = 0; i < RAIN_SAMPLES; i++) {
    total += analogRead(RAIN_AO_PIN);
    delay(3);
  }

  int avgRainValue = total / RAIN_SAMPLES;

  Serial.print("Rain Analog Value: ");
  Serial.println(avgRainValue);
  if (avgRainValue <= 5 || avgRainValue >= 4090)
  {
      if (!rainSensorFault)
      {
          if(!offlineMode)
          Blynk.logEvent("sensor_fault", "Rain sensor fault");
          rainSensorFault = true;
      }

      rainDetected = false;
      return;
  }
  else
  {
      rainSensorFault = false;
  }
  if (avgRainValue < rainThreshold) { // Detect rain if value drops below threshold
    rainDetected = true;
    rainConfirmed = true;
    rainEventLatched = true;
    rainPredictionStartTime = 0;

    Serial.println(" Rain detected by rain sensor");
  } else {
    rainDetected = false;
    rainConfirmed = false;
  }
}

/*********************** pH SENSOR BLOCK ************************/
void updateSoilPh() {
  int sensorValue = analogRead(pH_pin);
  if(sensorValue <= 5 || sensorValue >= 4090)
  {
      if (!phSensorFault)
      {
          if(!offlineMode)
          Blynk.logEvent("sensor_fault", "pH sensor fault");
          phSensorFault = true;
      }
      return;
  }
  else
  {
      phSensorFault = false;
  }
  float voltage = sensorValue * (3.3 / 4095.0);
  lastPhValue = voltageToPhFactor * voltage + offset;
if (selectedCrop == "Others")
{
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
}
  calculateFertilizer(lastPhValue);
  sendToTerminal();
  Serial.println("Weekly pH Update:");
  Serial.print("pH = ");
  Serial.println(lastPhValue);
  Serial.print("Soil Type = ");
  Serial.println(soilType);
}

/*********************** WEATHER PREDICTION BLOCK ************************/
bool checkRainForecast() {

  Serial.println("Checking weather forecast...");

  HTTPClient http;
  WiFiClient client;
  String url ="http://api.openweathermap.org/data/2.5/forecast?lat="+String(lat, 6)+"&lon="+String(lon, 6)+"&appid="+weatherApiKey+"&units=metric";

  http.begin(client,url);
  int httpCode = http.GET();

  if (httpCode != 200) {
    Serial.print("HTTP Error: ");
    Serial.println(httpCode);
    http.end();
    return false;
  }

  String payload = http.getString();
  http.end();

  StaticJsonDocument<8192> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.println("JSON parse failed");
    return false;
  }

  bool rainComing = false;

  // Check next 12 hours (4 x 3-hour blocks)
  for (int i = 0; i < 4; i++) {
    float rain = doc["list"][i]["rain"]["3h"] | 0;

    Serial.print("Latitude: ");
  Serial.println(lat);
  Serial.print("Longitude: ");
  Serial.println(lon);
    Serial.print("Block ");
    Serial.print(i);
    Serial.print(" rain: ");
    Serial.println(rain);

    if (rain > 0) {
      rainComing = true;
      break;
    }
  }

  if (rainComing) {
    Serial.println("Rain predicted in next 12 hours!");
  } else {
    Serial.println(" No rain in next 12 hours.");
  }

  return rainComing;   
}

/*********************** BLYNK HANDLERS ************************/
BLYNK_WRITE(V6) {
  if (offlineMode) return;
  if (param.asInt() == 1) {
    updateSoilPh();
    lastPhReadTime = millis(); // reset periodic timer

    Blynk.virtualWrite(V7, lastPhValue);
    Blynk.virtualWrite(V8, soilType);
    Blynk.virtualWrite(V14, fertilizerName);
    Blynk.virtualWrite(V15, fertilizerAmount);
    Blynk.virtualWrite(V16, fertilizerBrand);
    sendToTerminal();
    Serial.println("Manual pH + Fertilizer + Brand update");

  }
}
BLYNK_WRITE(V31)
{
  if (offlineMode) return;
  if (param.asInt() == 1)
  {
    selectedCrop = "Rice";
    updateCropParameters();

    Blynk.virtualWrite(V32, 0);
    Blynk.virtualWrite(V33, 0);
    Blynk.virtualWrite(V34, 0);
    Blynk.virtualWrite(V35, 0);
    Blynk.virtualWrite(V36, 0);

    Serial.println("Selected Crop: Rice");
    Serial.print("Moisture Threshold: ");
    Serial.println(moistureThreshold);
  }
}
BLYNK_WRITE(V32)
{
  if (offlineMode) return;
  if (param.asInt() == 1)
  {
    selectedCrop = "Potato";
    updateCropParameters();

    Blynk.virtualWrite(V31, 0);
    Blynk.virtualWrite(V33, 0);
    Blynk.virtualWrite(V34, 0);
    Blynk.virtualWrite(V35, 0);
    Blynk.virtualWrite(V36, 0);

    Serial.println("Selected Crop: Potato");
    Serial.print("Moisture Threshold: ");
    Serial.println(moistureThreshold);
  }
}
BLYNK_WRITE(V33)
{
  if (offlineMode) return;
  if (param.asInt() == 1)
  {
    selectedCrop = "Wheat";
    updateCropParameters();

    Blynk.virtualWrite(V31, 0);
    Blynk.virtualWrite(V32, 0);
    Blynk.virtualWrite(V34, 0);
    Blynk.virtualWrite(V35, 0);
    Blynk.virtualWrite(V36, 0);

    Serial.println("Selected Crop: Wheat");
    Serial.print("Moisture Threshold: ");
    Serial.println(moistureThreshold);
  }
}
BLYNK_WRITE(V34)
{
  if (offlineMode) return;
  if (param.asInt() == 1)
  {
    selectedCrop = "Onion";
    updateCropParameters();

    Blynk.virtualWrite(V31, 0);
    Blynk.virtualWrite(V32, 0);
    Blynk.virtualWrite(V33, 0);
    Blynk.virtualWrite(V35, 0);
    Blynk.virtualWrite(V36, 0);

    Serial.println("Selected Crop: Onion");
    Serial.print("Moisture Threshold: ");
    Serial.println(moistureThreshold);
  }
}
BLYNK_WRITE(V35)
{
  if (offlineMode) return;
  if (param.asInt() == 1)
  {
    selectedCrop = "Tomato";
    updateCropParameters();

    Blynk.virtualWrite(V31, 0);
    Blynk.virtualWrite(V32, 0);
    Blynk.virtualWrite(V33, 0);
    Blynk.virtualWrite(V34, 0);
    Blynk.virtualWrite(V36, 0);

    Serial.println("Selected Crop: Tomato");
    Serial.print("Moisture Threshold: ");
    Serial.println(moistureThreshold);
  }
}
BLYNK_WRITE(V36)
{
  if (offlineMode) return;
  if (param.asInt() == 1)
  {
    selectedCrop = "Others";
    updateCropParameters();

    Blynk.virtualWrite(V31, 0);
    Blynk.virtualWrite(V32, 0);
    Blynk.virtualWrite(V33, 0);
    Blynk.virtualWrite(V34, 0);
    Blynk.virtualWrite(V35, 0);

    Serial.println("Selected Crop: Others");
    Serial.print("Moisture Threshold: ");
    Serial.println(moistureThreshold);
  }
}

/*********************** MAIN LOOP ************************/
void loop() { 
   handleConnectivity(); 
  if (!offlineMode) {
    Blynk.run();
  }
  unsigned long now = millis();
bool doTimedLoop = false;
if (now - lastLoopTime >= LOOP_INTERVAL) { 
  lastLoopTime = now;
  doTimedLoop = true;
}
/************* RAIN DETECTION AND PREDICTION LOGIC *************/
if(doTimedLoop){
  readRainSensor();
  if (offlineMode) {
  rainPredicted = rainDetected; // fallback to physical sensor in case of no wifi
}
else if (!offlineMode && millis() - lastWeatherCheck >= WEATHER_INTERVAL) {
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
/************* SOIL PH DETECTION *************/
 unsigned long currentMillis = millis();
if (currentMillis - lastPhReadTime >= PH_INTERVAL || lastPhReadTime == 0) {
  updateSoilPh();
  lastPhReadTime = currentMillis;
  if(!offlineMode)
  {
  Blynk.virtualWrite(V7, lastPhValue);
  Blynk.virtualWrite(V8, soilType);
  }
}
/************* ULTRASONIC SENSOR DETECTING TANK WATER LEVEL *************/
digitalWrite(trigPin, LOW); 
delayMicroseconds(2); 
digitalWrite(trigPin, HIGH); 
delayMicroseconds(10); 
digitalWrite(trigPin, LOW); 
duration = pulseIn(echoPin, HIGH, 30000); // 30MS TIMEOUT TO PREVENT ESP32 HANG
if (duration == 0)
{
    if (!ultrasonicFault)
    {
        if(!offlineMode)
        Blynk.logEvent("sensor_fault", "Ultrasonic timeout");
        ultrasonicFault = true;
    }

    distanceCm = 100;
}
else
{
    ultrasonicFault = false;
    distanceCm = duration * SOUND_SPEED / 2;
} 
Serial.print("Distance (cm): "); 
Serial.println(distanceCm); 
int l = constrain(100 - distanceCm, 0, 100); 
if(!offlineMode)
Blynk.virtualWrite(V3,l); 
if (distanceCm <15) { 
if(!offlineMode)
Blynk.virtualWrite(V5, "Tank Has Sufficient Water");
tankEmptyLatched = false;   //  RESET latch when water is present

/************* SOIL MOISTURE DETECTION *************/
int soilMoistureValue = analogRead(soilPin); 
if(soilMoistureValue <= 5 || soilMoistureValue >= 4090)
{
    if (!soilSensorFault)
    {
        if(!offlineMode)
        Blynk.logEvent("sensor_fault", "Soil moisture sensor fault");
        soilSensorFault = true;
    }
    digitalWrite(pin, LOW);
    pumpRunning = false;
    reportPriorityFault();
    return;
}
else
{
    soilSensorFault = false;
}
Serial.print("Soil Moisture: "); 
Serial.println(soilMoistureValue); 
int moisturepercent=map(soilMoistureValue,4095,0,0,100); 
int criticalMoisture = moistureThreshold - CRITICAL_MOISTURE_OFFSET;
Blynk.virtualWrite(V4,moisturepercent); 
/************* DHT SENSOR READING TEMPERATURE AND HUMIDITY*************/
float h = dht.readHumidity();
float t = dht.readTemperature();
if (isnan(h) || isnan(t))
{
    if (!dhtFault)
    {
        
        Blynk.logEvent("sensor_fault", "DHT11 sensor fault");
        dhtFault = true;
    }

    h = 50; //DEFAULT VALUES IN CASE OF FAULT
    t = 25; //DEFAULT VALUES IN CASE OF FAULT
}
else
{
    dhtFault = false;
}

/************* PUMP ON TIME CALCULATION *************/
unsigned long pumpTime = basePumpTime;
float tempFactor = 1.0;
float humFactor = 1.0;
if (t > 25) {
  tempFactor += (t - 25) * 0.03;
}
if (h < 50) {
  humFactor += (50 - h) * 0.02;
}
pumpTime = basePumpTime * tempFactor * humFactor; //PUMPTIME DEPENDS ON BOTH TEMPERATURE AND HUMIDITY
if (pumpTime > 10000) pumpTime = 10000; //TO PREVENT OVERWATERING
if (pumpTime < 3000) pumpTime = 3000; // TO PREVENT UNDERWATERING

/************* WATER SAVING CALCULATION *************/
if ((rainConfirmed||rainEventLatched) && moisturepercent < moistureThreshold) {
  float waterSaved =(pumpFlowRate / 60.0) * (pumpTime / 1000.0);
  totalWaterSaved += waterSaved;
  Serial.print("Actual rain → Water saved: ");
  Serial.print(waterSaved);
  Serial.println(" L");
  rainConfirmed = false; 
  rainEventLatched = false; 
}

/************* MAIN IRRIGATION LOGIC *************/
if (rainDetected && !predictionExpired) { // RAIN DETECTED, NO WATERING REQUIRED
  pumpRunning = false;
  digitalWrite(pin, LOW);
}
else if (moisturepercent < criticalMoisture) { //SOIL IS CRITICALLY DRY, WATER SOIL ANYWAY DESPITE PREDICTION
  Serial.println("⚠ Emergency irrigation override");
  soilWasWet = false;   
  rainPredicted = false;
  rainPredictionStartTime = 0;
  pumpRunning = true;
  pumpStartTime = millis();
  currentPumpTime = pumpTime;
  digitalWrite(pin, HIGH);
}
else if (rainPredicted && !predictionExpired) { // PREDICTION TIMER EXPIRED, SO WATER ANYWAY
  pumpRunning = false;
  digitalWrite(pin, LOW);
}
else if (moisturepercent < moistureThreshold && soilWasWet && !pumpRunning) { //ALL CONDITIONS MET FOR WATERING PLANT
  soilWasWet = false;
  Serial.println("Irrigation ON");
  pumpRunning = true;
  pumpStartTime = millis();
  currentPumpTime = pumpTime;
  digitalWrite(pin, HIGH);
}

/************* PUMP SAFETY *************/
if (pumpRunning)
{
    // Dry run protection: tank empty while pump running
    if (distanceCm >= 15)
    {
        if (!pumpFault)
        {
            if(!offlineMode)
            Blynk.logEvent("pump_fault", "Dry run detected");
            pumpFault = true;
        }

        Serial.println("Pump stopped: Tank empty");
        digitalWrite(pin, LOW);
        pumpRunning = false;
    }
    // Normal stop after irrigation complete
    else if (millis() - pumpStartTime >= currentPumpTime)
    {
        Serial.println("Irrigation OFF");
        pumpRunning = false;
        digitalWrite(pin, LOW);

        rainPredicted = false;
        rainPredictionStartTime = 0;

        pumpFault = false;
    }
}
if (moisturepercent >= moistureThreshold) {
  soilWasWet = true;
  pumpRunning = false;
  digitalWrite(pin, LOW);
}
Serial.print(F("%  Humidity: "));
Serial.print(h); 
Serial.print(F("%  Temperature: ")); 
Serial.print(t); 
if(!offlineMode)
{
Blynk.virtualWrite(V1,h);
Blynk.virtualWrite(V0,t);
} 
Serial.print(F("°C ")); 
} 
else 
{ 
  pumpFault = false;
Serial.println("Please fill up the tank"); 
if(!offlineMode)
Blynk.virtualWrite(V5, "TANK IS EMPTY!! REFILL TANK!"); 
if (!tankEmptyLatched) {                 //  LATCH CHECK
    if(!offlineMode)
    Blynk.logEvent("tank_empty_event", "Tank is empty! Please refill.");
    tankEmptyLatched = true;               //  LATCH SET
  }
digitalWrite(pin, LOW); 
} 

/************* FINAL UPDATES *************/
if(!offlineMode)
{
Blynk.virtualWrite(V11, totalWaterSaved);     // for total
Blynk.virtualWrite(V12, totalWaterSaved);     // for graph
}
Serial.println("Fertilizer: " + fertilizerName + " | Brand: " + fertilizerBrand +  " | Amount: " + String(fertilizerAmount)); 
reportPriorityFault();
}
}