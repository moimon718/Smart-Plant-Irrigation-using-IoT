# ESP32 Pinout

## Pin Assignment

The following table shows the GPIO assignments used by the Smart Plant Irrigation System.

| ESP32 Pin | Connected Device     | Signal  | Function                      |
| --------- | -------------------- | ------- | ----------------------------- |
| GPIO 27   | DHT11                | DATA    | Temperature and humidity data |
| GPIO 12   | HC-SR04              | TRIG    | Ultrasonic trigger            |
| GPIO 13   | HC-SR04              | ECHO    | Ultrasonic echo               |
| GPIO 33   | Rain Sensor          | AO      | Rainfall detection            |
| GPIO 34   | Soil Moisture Sensor | AO      | Soil moisture measurement     |
| GPIO 35   | pH Sensor            | PO      | Soil pH measurement           |
| GPIO 32   | L298N                | Control | Pump control                  |

---

## Power Connections

| Component            | Supply |
| -------------------- | ------ |
| DHT11                | 3.3V   |
| Soil Moisture Sensor | 3.3V   |
| HC-SR04              | 5V     |
| pH Sensor            | 5V     |

---

## Unused Signals

| Component            | Signal | Status        |
| -------------------- | ------ | ------------- |
| Soil Moisture Sensor | DO     | Not connected |

---

## GPIO Usage Summary

```text
GPIO 12  → HC-SR04 TRIG
GPIO 13  → HC-SR04 ECHO

GPIO 27  → DHT11 DATA

GPIO 32  → Pump / L298N Control

GPIO 33  → Rain Sensor AO
GPIO 34  → Soil Moisture Sensor AO
GPIO 35  → pH Sensor PO
```

---

## Quick Reference

```text
                 ESP32
        ┌─────────────────────┐
        │                     │
GPIO 12 │──────► HC-SR04 TRIG │
GPIO 13 │◄────── HC-SR04 ECHO │
GPIO 27 │◄────── DHT11 DATA   │
GPIO 32 │──────► PUMP CONTROL │
GPIO 33 │◄────── RAIN AO      │
GPIO 34 │◄────── MOISTURE AO  │
GPIO 35 │◄────── pH PO        │
        │                     │
        └─────────────────────┘
```

For detailed physical connections, refer to [`../docs/wiring.md`](../docs/wiring.md).
