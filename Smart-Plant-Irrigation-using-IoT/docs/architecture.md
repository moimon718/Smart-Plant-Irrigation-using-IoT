# System Architecture

## 1. Overview

The Smart Plant Irrigation System is an ESP32-based IoT system designed to automate irrigation using real-time sensor data, environmental conditions, soil pH, crop-specific requirements, rainfall information, and water-tank monitoring.

The system combines a sensing layer, ESP32 control layer, irrigation decision logic, pump control, and IoT connectivity.

---

## 2. System Architecture

```text
                         ┌──────────────────────────┐
                         │        ESP32 MCU         │
                         │                          │
                         │  Sensor Processing       │
                         │  Irrigation Logic        │
                         │  Safety / Fault Logic    │
                         └────────────┬─────────────┘
                                      │
              ┌───────────────────────┼───────────────────────┐
              │                       │                       │
              ▼                       ▼                       ▼
       ┌──────────────┐       ┌──────────────┐       ┌──────────────┐
       │ Sensor Layer │       │ Decision     │       │ Connectivity │
       │              │       │ Layer        │       │ Layer        │
       │ Soil Moisture│       │ Crop Logic   │       │ Wi-Fi        │
       │ DHT11        │       │ Rain Logic   │       │ BLYNK        │
       │ Rain Sensor  │       │ Weather      │       │ Weather API  │
       │ pH Sensor    │       │ pH Logic     │       └──────────────┘
       │ Ultrasonic   │       │ Safety Logic │
       └──────────────┘       └──────┬───────┘
                                     │
                                     ▼
                            ┌──────────────────┐
                            │ Pump Control     │
                            │                  │
                            │ Motor Driver     │
                            │ L298N            │
                            └────────┬─────────┘
                                     │
                                     ▼
                            ┌──────────────────┐
                            │ Irrigation Pump  │
                            └──────────────────┘
```

---

## 3. Major System Components

### 3.1 ESP32 Microcontroller

The ESP32 acts as the central controller of the system.

Its responsibilities include:

* Reading sensor data
* Processing sensor measurements
* Selecting crop-specific irrigation parameters
* Evaluating soil moisture conditions
* Processing temperature and humidity information
* Checking rainfall conditions
* Processing weather forecast information
* Monitoring water-tank level
* Controlling the irrigation pump
* Sending monitoring data through Blynk

---

### 3.2 Sensor Layer

The system uses multiple sensors to collect information about the plant and its environment.

| Sensor                          | Purpose                           |
| ------------------------------- | --------------------------------- |
| Capacitive Soil Moisture Sensor | Measures soil moisture            |
| DHT11                           | Measures temperature and humidity |
| Rain Sensor                     | Detects rainfall                  |
| pH Sensor                       | Determines soil pH / soil type    |
| HC-SR04 Ultrasonic Sensor       | Monitors water-tank level         |

The sensors continuously provide information to the ESP32 for irrigation decision-making.

---

### 3.3 Decision Layer

The decision layer determines whether irrigation is required and how long the pump should operate.

The decision process considers:

* Soil moisture
* Selected crop
* Soil pH
* Temperature
* Humidity
* Rain detection
* Weather forecast
* Critical soil-moisture condition
* Water-tank level

The system uses crop-specific irrigation parameters rather than applying one fixed threshold to every crop.

---

### 3.4 Rain and Weather Layer

The system uses both local rain sensing and weather forecast information.

The rain sensor provides information about current rainfall conditions.

The Weather API provides forecast information that can be used to avoid unnecessary irrigation when rainfall is expected.

If rain is detected or rainfall is predicted, irrigation can be skipped unless the soil reaches a critical moisture condition.

---

### 3.5 Soil pH Layer

The pH sensor is used to classify the soil condition as acidic, neutral, or basic.

The pH information is used to:

* Adjust irrigation-related moisture thresholds
* Determine soil type
* Provide fertilizer recommendations

---

### 3.6 Irrigation Control Layer

The irrigation control layer controls the water pump through the motor driver.

The pump is activated when the irrigation decision indicates that watering is required.

Pump operating time can be modified using environmental conditions such as temperature and humidity.

---

### 3.7 Tank Safety Layer

The HC-SR04 ultrasonic sensor monitors the water level in the irrigation tank.

If the tank becomes empty:

```text
Tank Empty
    ↓
Pump Operation Blocked
    ↓
User Alert
```

This prevents the pump from operating without sufficient water.

---

### 3.8 IoT Connectivity Layer

The ESP32 uses Wi-Fi for communication with the IoT platform.

Blynk is used for remote monitoring and user alerts.

The dashboard can display information such as:

* Soil moisture
* Tank level
* Soil pH
* Soil type
* Irrigation status
* Other system information

---

## 4. High-Level Data Flow

```text
Sensors
   │
   ▼
ESP32 Sensor Acquisition
   │
   ▼
Data Processing
   │
   ├── Soil Moisture
   ├── Temperature / Humidity
   ├── Rain
   ├── Soil pH
   └── Tank Level
   │
   ▼
Irrigation Decision
   │
   ├── Crop Parameters
   ├── Rain Condition
   ├── Weather Forecast
   ├── Environmental Compensation
   └── Safety Checks
   │
   ▼
Pump Control
   │
   ▼
Irrigation
   │
   ▼
Blynk Monitoring / Alerts
```

---

## 5. Design Goals

The architecture is designed to provide:

* Automated irrigation
* Reduced water wastage
* Crop-specific irrigation control
* Rain-aware irrigation
* Weather-aware irrigation
* Environmental adaptation
* Water-tank protection
* Remote monitoring
* Fault and safety handling

---

## 6. Hardware and Software Boundary

### Hardware

* ESP32
* Capacitive soil moisture sensor
* DHT11
* Rain sensor
* pH sensor
* HC-SR04 ultrasonic sensor
* L298N motor driver
* Water pump
* Power/buck converter
* Breadboard and wiring

### Software

* Arduino IDE
* ESP32 firmware
* Blynk
* Wi-Fi connectivity
* Weather API

---

## 7. Architecture Summary

The ESP32 acts as the central control unit connecting the sensor layer, irrigation decision logic, pump-control system, and IoT connectivity.

Sensor data is combined with crop-specific parameters, soil pH, environmental conditions, rainfall information, and water-level safety conditions to make irrigation decisions.

The resulting architecture provides an adaptive irrigation system rather than a simple moisture-triggered pump controller.
