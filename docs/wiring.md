# Hardware Wiring

## 1. Overview

This document describes the hardware connections used in the Smart Plant Irrigation System.

The ESP32 acts as the main controller and interfaces with the soil moisture sensor, DHT11, ultrasonic sensor, rain sensor, pH sensor, and pump control circuit.

---

## 2. Hardware Components

The prototype uses:

* ESP32
* Capacitive Soil Moisture Sensor v1.2
* DHT11 Temperature and Humidity Sensor
* HC-SR04 Ultrasonic Sensor
* Rain Sensor
* pH Sensor
* L298N Motor Driver
* Water Pump
* Buck Converter
* Breadboard
* Connecting wires

---

## 3. ESP32 Pin Mapping

| Component            | Signal        |     ESP32 Pin |
| -------------------- | ------------- | ------------: |
| DHT11                | DATA          |       GPIO 27 |
| HC-SR04              | TRIG          |       GPIO 12 |
| HC-SR04              | ECHO          |       GPIO 13 |
| Rain Sensor          | AO            |       GPIO 33 |
| Soil Moisture Sensor | AO            |       GPIO 34 |
| Soil Moisture Sensor | DO            | Not connected |
| pH Sensor            | PO            |       GPIO 35 |
| Pump Control         | Control Input |       GPIO 32 |

For a quick reference, see [`../hardware/pinout.md`](../hardware/pinout.md).

---

## 4. DHT11 Sensor

The DHT11 is used to measure temperature and humidity.

### Connection

| DHT11 Pin | Connection    |
| --------- | ------------- |
| VCC       | ESP32 3.3V    |
| GND       | ESP32 GND     |
| DATA      | ESP32 GPIO 27 |

```text
DHT11
 ┌─────────────┐
 │ VCC  ───────┼──── 3.3V
 │ GND  ───────┼──── GND
 │ DATA ───────┼──── GPIO 27
 └─────────────┘
```

---

## 5. HC-SR04 Ultrasonic Sensor

The HC-SR04 is used to monitor the water level in the irrigation tank.

### Connection

| HC-SR04 Pin | Connection    |
| ----------- | ------------- |
| VCC         | 5V            |
| GND         | GND           |
| TRIG        | ESP32 GPIO 12 |
| ECHO        | ESP32 GPIO 13 |

```text
HC-SR04
 ┌─────────────┐
 │ VCC  ───────┼──── 5V
 │ GND  ───────┼──── GND
 │ TRIG ───────┼──── GPIO 12
 │ ECHO ───────┼──── GPIO 13
 └─────────────┘
```

The ultrasonic sensor is used as a tank-level safety input. If the tank becomes empty, pump operation is blocked.

---

## 6. Rain Sensor

The rain sensor provides an analog signal representing the rainfall condition.

### Connection

| Rain Sensor Pin | Connection    |
| --------------- | ------------- |
| AO              | ESP32 GPIO 33 |

```text
Rain Sensor
      │
      │ AO
      ▼
 ESP32 GPIO 33
```

The digital output is not used in the documented configuration.

---

## 7. Capacitive Soil Moisture Sensor

The capacitive soil moisture sensor measures the soil moisture condition.

### Connection

| Soil Moisture Pin | Connection    |
| ----------------- | ------------- |
| VCC               | ESP32 3.3V    |
| GND               | ESP32 GND     |
| AO                | ESP32 GPIO 34 |
| DO                | Not connected |

```text
Soil Moisture Sensor
 ┌─────────────┐
 │ VCC  ───────┼──── 3.3V
 │ GND  ───────┼──── GND
 │ AO   ───────┼──── GPIO 34
 │ DO   ───────┼──── Not Connected
 └─────────────┘
```

The analog output is used by the ESP32 for soil-moisture measurement.

---

## 8. pH Sensor

The pH sensor provides an analog signal used to determine the soil pH condition.

### Connection

| pH Sensor Pin | Connection    |
| ------------- | ------------- |
| VCC           | 5V            |
| GND           | GND           |
| PO            | ESP32 GPIO 35 |

```text
pH Sensor
 ┌─────────────┐
 │ VCC  ───────┼──── 5V
 │ GND  ───────┼──── GND
 │ PO   ───────┼──── GPIO 35
 └─────────────┘
```

The measured pH is used for soil classification and fertilizer recommendation logic.

---

## 9. Pump Control

The irrigation pump is controlled through the L298N motor driver.

The ESP32 provides the control signal to the motor-driver circuit.

### Control connection

| Signal             | Connection    |
| ------------------ | ------------- |
| Pump Control Input | ESP32 GPIO 32 |

```text
ESP32
 GPIO 32
    │
    ▼
┌──────────────┐
│ L298N Driver │
└──────┬───────┘
       │
       ▼
  Water Pump
```

The motor driver provides the interface between the ESP32 control signal and the pump.

---

## 10. Power Connections

The documented prototype uses different supply levels for different components.

### 3.3V Devices / Connections

* DHT11 VCC
* Soil Moisture Sensor VCC

### 5V Connections

* HC-SR04 VCC
* pH Sensor VCC

The pump and motor-driver power path should use the appropriate external supply for the selected pump and driver configuration.

---

## 11. Ground Connections

All components that interface with the ESP32 should have a common reference ground where required by the circuit design.

```text
ESP32 GND
   │
   ├── DHT11 GND
   ├── Soil Moisture GND
   ├── HC-SR04 GND
   ├── Rain Sensor GND
   ├── pH Sensor GND
   └── Motor Driver GND
```

---

## 12. Complete Connection Summary

```text
                         ┌─────────────────┐
                         │      ESP32      │
                         │                 │
 GPIO 27 ◄───────────────┤ DHT11 DATA      │
 GPIO 12 ───────────────►├ HC-SR04 TRIG    │
 GPIO 13 ◄───────────────┤ HC-SR04 ECHO    │
 GPIO 33 ◄───────────────┤ Rain Sensor AO  │
 GPIO 34 ◄───────────────┤ Moisture AO     │
 GPIO 35 ◄───────────────┤ pH Sensor PO    │
 GPIO 32 ───────────────►├ Pump Control    │
                         │                 │
                         └─────────────────┘
                                  │
                                  ▼
                           ┌─────────────┐
                           │    L298N    │
                           │ Motor Driver│
                           └──────┬──────┘
                                  │
                                  ▼
                              Pump
```

---

## 13. Wiring Reference

The project hardware architecture consists of the ESP32, sensors, motor driver, pump, power circuitry, and supporting connections.

Refer to the following files for additional hardware information:

* [`../hardware/pinout.md`](../hardware/pinout.md)
* [`../hardware/block-diagram.png`](../hardware/block-diagram.png)
* [`../hardware/circuit-diagram.png`](../hardware/circuit-diagram.png)
* [`../hardware/wiring-diagram.png`](../hardware/wiring-diagram.png)

---

## 14. Important Notes

* Verify the supply voltage required by each sensor before powering the circuit.
* Keep sensor grounds connected correctly to the system ground.
* The pump should not be powered directly from an ESP32 GPIO.
* The motor driver is used to interface the ESP32 control signal with the pump.
* The water-tank safety logic should prevent pump operation when sufficient water is unavailable.
* Verify the actual hardware connections against the prototype before applying power.

---

## 15. Wiring Summary

The ESP32 serves as the central interface between the sensing components and the irrigation actuator.

Analog sensors provide measurements through ESP32 ADC-capable inputs, digital sensors use GPIO connections, and the pump is controlled through the L298N motor driver.

This wiring configuration allows the controller to collect environmental and soil information and use it for automated irrigation decisions.
