# Working Principle

## 1. Overview

The Smart Plant Irrigation System continuously collects information from multiple sensors and uses the ESP32 to make irrigation decisions.

The irrigation decision is based on soil moisture, selected crop, soil pH, temperature, humidity, rainfall, weather forecast, and water-tank level.

The system is designed to provide water only when required while preventing unnecessary irrigation and unsafe pump operation.

---

## 2. Overall Working Flow

```text
                START
                  │
                  ▼
        Initialize ESP32
                  │
                  ▼
          Read Sensor Data
                  │
        ┌─────────┼─────────┐
        ▼         ▼         ▼
     Moisture   DHT11     Rain
        │      Temp/Hum    │
        │         │        │
        └─────────┼────────┘
                  │
                  ▼
             Read pH
                  │
                  ▼
          Check Tank Level
                  │
          ┌───────┴───────┐
          │               │
        EMPTY             OK
          │               │
          ▼               ▼
      Block Pump       Continue
          │               │
          │               ▼
          │        Check Rain
          │               │
          │        ┌──────┴──────┐
          │        │             │
          │      RAIN          NO RAIN
          │        │             │
          │        ▼             ▼
          │   Skip Irrigation  Check Weather
          │                      │
          │                      ▼
          │                Check Moisture
          │                      │
          │                      ▼
          │                Crop Parameters
          │                      │
          │                      ▼
          │             Environmental Adjustment
          │                      │
          │                      ▼
          │              Calculate Pump Time
          │                      │
          └──────────────┬───────┘
                         ▼
                   Pump Control
                         │
                         ▼
                  Update Blynk
                         │
                         ▼
                 Continue Monitoring
```

---

## 3. Sensor Data Acquisition

The ESP32 reads data from the connected sensors.

### Soil Moisture

The capacitive soil moisture sensor provides an analog measurement representing the soil moisture condition.

The measured value is converted into a moisture percentage used by the irrigation decision logic.

---

### Temperature and Humidity

The DHT11 measures:

* Temperature
* Relative humidity

These values are used to modify the irrigation pump operating time.

Higher temperature and lower humidity can increase water demand, while lower temperature and higher humidity can reduce the required pump operating time.

---

### Rain Sensor

The rain sensor is used to detect current rainfall conditions.

If rain is detected, normal irrigation can be skipped to avoid unnecessary water usage.

---

### Soil pH

The pH sensor measures the soil condition.

The system uses pH information to classify the soil as:

* Acidic
* Neutral
* Basic

The soil condition is also used in the irrigation and fertilizer recommendation logic.

---

### Water-Tank Level

The HC-SR04 ultrasonic sensor measures the water level in the irrigation tank.

The tank-level information is used as a safety condition before operating the pump.

---

## 4. Crop Selection

The system supports crop-specific irrigation parameters.

Different crops can require different soil-moisture conditions.

The selected crop is therefore used to determine the appropriate irrigation threshold and base pump operating time.

The supported crop categories include:

* Rice
* Potato
* Wheat
* Onion
* Tomato
* Others

The crop parameters are applied before the final irrigation decision is made.

---

## 5. Soil Moisture Decision

After reading the sensor values, the ESP32 compares the measured soil moisture with the required threshold for the selected crop.

```text
Measured Moisture
       │
       ▼
Compare with Crop Threshold
       │
   ┌───┴────┐
   │        │
 Sufficient  Low
   │        │
   ▼        ▼
Pump OFF  Continue
          Decision
```

If the soil moisture is sufficient, irrigation remains OFF.

If the soil moisture falls below the required level, the system continues evaluating the remaining irrigation conditions.

---

## 6. Rain-Aware Irrigation

The system checks the local rain sensor before starting irrigation.

```text
Rain Detected?
      │
 ┌────┴────┐
YES        NO
 │          │
 ▼          ▼
Skip      Continue
Irrigation Decision
```

This prevents the pump from operating unnecessarily during rainfall.

---

## 7. Weather Forecast Integration

The system also uses weather forecast information.

The Weather API is used to determine whether rainfall is expected in the upcoming period.

If rainfall is predicted, irrigation can be avoided because natural rainfall may provide the required water.

However, the system includes a critical-moisture condition.

```text
Rain Forecast?
      │
 ┌────┴────┐
YES        NO
 │          │
 ▼          ▼
Check      Continue
Critical   Irrigation
Moisture   Decision
```

If the soil reaches a critical moisture condition, emergency irrigation can override the normal rain-based irrigation restriction.

---

## 8. Soil pH and Irrigation Logic

The soil pH measurement is used to classify the soil condition.

```text
pH Measurement
      │
      ▼
Soil Classification
      │
 ┌────┼─────┐
 ▼    ▼     ▼
Acidic Neutral Basic
 │     │      │
 └─────┼──────┘
       ▼
Adjust Irrigation Parameters
       │
       ▼
Fertilizer Recommendation
```

The system also provides fertilizer recommendations according to the soil pH condition.

---

## 9. Temperature and Humidity Compensation

The system modifies pump operating time using temperature and humidity information.

The principle used by the system is:

```text
Higher Temperature
        +
Lower Humidity
        ↓
Higher Evapotranspiration
        ↓
Longer Pump Runtime
```

Conversely:

```text
Lower Temperature
        +
Higher Humidity
        ↓
Lower Evapotranspiration
        ↓
Shorter Pump Runtime
```

This allows irrigation to respond to environmental conditions rather than relying only on a fixed pump duration.

---

## 10. Tank Safety

Before activating the pump, the system checks the available water level.

```text
Check Tank Level
       │
  ┌────┴────┐
Empty       Available
  │             │
  ▼             ▼
Block Pump   Continue
  │             │
  ▼             ▼
Send Alert   Pump Decision
```

If the tank is empty, the pump is blocked and the user can be notified.

This protects the irrigation system from operating without sufficient water.

---

## 11. Pump Control

After all decision conditions are evaluated, the ESP32 determines whether irrigation should occur.

If irrigation is required:

```text
Irrigation Required
        │
        ▼
Calculate Pump Runtime
        │
        ▼
Activate Pump
        │
        ▼
Irrigate Plant
        │
        ▼
Stop Pump
```

If irrigation is not required:

```text
Irrigation Not Required
        │
        ▼
Pump remains OFF
```

---

## 12. Water-Saving Logic

The system attempts to reduce unnecessary irrigation by considering natural and predicted rainfall.

Water can be conserved when:

* Rain is currently detected
* Rainfall is predicted
* Soil moisture is already sufficient
* The crop does not require immediate irrigation

The system also calculates water saved due to rainfall detection and weather prediction.

---

## 13. Fault and Safety Handling

The system includes fault and safety handling for conditions such as:

* Sensor failures
* Pump dry-run conditions
* Connectivity issues
* Empty water tank

The safety logic prevents inappropriate pump operation and provides user alerts where applicable.

---

## 14. Remote Monitoring

The ESP32 communicates with the Blynk platform through Wi-Fi.

The user can monitor system information remotely, including sensor values and irrigation-related status.

The system can also provide alerts for conditions such as low tank level and detected faults.

---

## 15. Complete Decision Sequence

The complete irrigation decision can be summarized as:

```text
1. Read sensors
       ↓
2. Determine selected crop
       ↓
3. Determine crop-specific moisture threshold
       ↓
4. Read soil pH and classify soil
       ↓
5. Check water-tank level
       ↓
6. If tank is empty → block pump
       ↓
7. Check current rainfall
       ↓
8. Check weather forecast
       ↓
9. Check critical soil moisture
       ↓
10. Compare soil moisture with required threshold
       ↓
11. Apply temperature/humidity compensation
       ↓
12. Calculate pump operating time
       ↓
13. Activate pump if irrigation is required
       ↓
14. Stop pump after required operating time
       ↓
15. Update Blynk monitoring information
       ↓
16. Continue monitoring
```

---

## 16. Operating Modes

The system is designed to support both online and offline operation.

### Online Mode

```text
Sensors
   ↓
ESP32
   ↓
Local Irrigation Logic
   ↓
Wi-Fi
   ↓
Blynk / Weather API
```

Weather forecast information and remote monitoring are available when network connectivity is available.

### Offline Mode

```text
Sensors
   ↓
ESP32
   ↓
Local Irrigation Logic
   ↓
Pump Control
```

The local irrigation logic can continue operating without relying entirely on cloud connectivity.

---

## 17. Working Principle Summary

The system combines multiple sensor inputs and environmental information to make an adaptive irrigation decision.

Instead of using only soil moisture, the controller considers crop requirements, soil pH, temperature, humidity, rainfall, weather forecast, and water availability.

The final decision determines whether the pump should remain OFF, operate normally, or be prevented from operating because of a safety condition.

This approach provides automated, rain-aware, crop-specific and safety-aware irrigation control.
