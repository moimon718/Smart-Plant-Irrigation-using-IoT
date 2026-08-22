# 🌱 Smart Plant Irrigation System using IoT

An ESP32-based smart irrigation system that automates plant watering using soil moisture, temperature, humidity, rainfall, soil pH, water-tank level, crop-specific requirements, and weather forecast information.

The system combines sensor-based monitoring, adaptive irrigation logic, safety mechanisms, and IoT-based remote monitoring through Blynk.

---

## 🚀 Key Features

- 🌱 Soil-moisture-based automatic irrigation
- 🌾 Crop-specific irrigation parameters
- 🌡️ Temperature and humidity monitoring
- 💧 Adaptive pump runtime based on environmental conditions
- 🌧️ Rain detection
- 🌦️ Weather forecast integration
- 🧪 Soil pH monitoring and soil classification
- 🚰 Water-tank level monitoring
- 🛡️ Pump safety and fault handling
- 📱 Blynk-based remote monitoring
- 💾 Water-saving estimation
- 🌿 pH-based fertilizer recommendation

---

## 🎯 Project Objective

The objective of this project is to develop an automated irrigation system that provides water according to actual plant and environmental conditions instead of relying on fixed irrigation schedules.

The ESP32 collects sensor data and uses crop-specific parameters, soil conditions, rainfall information, weather forecasts, and water availability to determine when irrigation is required.

---

## 🧠 System Architecture

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
       │ DHT11        │       │ Rain Logic   │       │ Blynk        │
       │ Rain Sensor  │       │ Weather      │       │ Weather API  │
       │ pH Sensor    │       │ pH Logic     │       └──────────────┘
       │ Ultrasonic   │       │ Safety Logic │
       └──────────────┘       └──────┬───────┘
                                     │
                                     ▼
                            ┌──────────────────┐
                            │ Pump Control     │
                            │ L298N Driver     │
                            └────────┬─────────┘
                                     │
                                     ▼
                            ┌──────────────────┐
                            │ Irrigation Pump  │
                            └──────────────────┘
