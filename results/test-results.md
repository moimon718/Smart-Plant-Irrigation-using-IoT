# Test Results

## 1. Overview

The Smart Plant Irrigation System was tested under different operating conditions to verify sensor-based irrigation control, environmental adaptation, safety handling, crop-specific operation, and IoT functionality.

All listed test cases were successfully verified during project testing.

**Overall Result: 15/15 PASS**

---

## 2. Test Summary

| ID  | Test Case                          | Expected Behavior                               | Result |
| --- | ---------------------------------- | ----------------------------------------------- | ------ |
| T01 | Sufficient soil moisture           | Pump remains OFF                                | PASS   |
| T02 | Low soil moisture                  | Irrigation activates                            | PASS   |
| T03 | Rain detected                      | Normal irrigation is skipped                    | PASS   |
| T04 | Rain forecast                      | Irrigation is avoided when appropriate          | PASS   |
| T05 | Critical soil moisture             | Emergency irrigation overrides rain restriction | PASS   |
| T06 | Empty water tank                   | Pump operation is blocked                       | PASS   |
| T07 | Normal tank level                  | Pump operation is permitted when required       | PASS   |
| T08 | High temperature / low humidity    | Pump runtime increases                          | PASS   |
| T09 | Low temperature / high humidity    | Pump runtime decreases                          | PASS   |
| T10 | Crop selection                     | Crop-specific parameters are applied            | PASS   |
| T11 | Soil pH measurement                | Soil condition is classified                    | PASS   |
| T12 | pH-based fertilizer logic          | Fertilizer recommendation is generated          | PASS   |
| T13 | Blynk connectivity                 | Monitoring data is updated remotely             | PASS   |
| T14 | Fault detection                    | Fault condition is detected/handled             | PASS   |
| T15 | Pump dry-run / low-water condition | Unsafe pump operation is prevented/handled      | PASS   |

---

## 3. Detailed Test Results

### T01 — Sufficient Soil Moisture

**Condition:** Soil moisture is above the required threshold.

**Expected behavior:**

```text
Soil Moisture ≥ Required Threshold
              ↓
          Pump OFF
```

**Result:** PASS

**Observation:** The system correctly kept the irrigation pump OFF when sufficient soil moisture was available.

---

### T02 — Low Soil Moisture

**Condition:** Soil moisture falls below the required threshold for the selected crop.

**Expected behavior:**

```text
Low Soil Moisture
       ↓
Irrigation Required
       ↓
Pump Activated
```

**Result:** PASS

**Observation:** The system correctly detected insufficient soil moisture and activated irrigation.

---

### T03 — Rain Detection

**Condition:** Rain sensor detects rainfall.

**Expected behavior:**

```text
Rain Detected
      ↓
Skip Normal Irrigation
```

**Result:** PASS

**Observation:** The system correctly prevented unnecessary irrigation when rainfall was detected.

---

### T04 — Weather Forecast

**Condition:** Weather API indicates rainfall is expected.

**Expected behavior:**

```text
Rain Forecast
     ↓
Avoid Normal Irrigation
```

**Result:** PASS

**Observation:** The system correctly used weather forecast information to avoid unnecessary irrigation when appropriate.

---

### T05 — Critical Soil Moisture

**Condition:** Soil moisture reaches a critical level while rain is detected or predicted.

**Expected behavior:**

```text
Critical Moisture
       ↓
Emergency Irrigation
       ↓
Pump Activated
```

**Result:** PASS

**Observation:** The emergency irrigation condition correctly overrode the normal rain-based irrigation restriction.

---

### T06 — Empty Water Tank

**Condition:** The ultrasonic sensor indicates that the tank is empty.

**Expected behavior:**

```text
Tank Empty
    ↓
Pump Blocked
    ↓
User Alert
```

**Result:** PASS

**Observation:** The system correctly prevented pump operation when sufficient water was unavailable.

---

### T07 — Normal Water-Tank Level

**Condition:** Sufficient water is available in the tank.

**Expected behavior:**

The pump is permitted to operate when the irrigation decision requires watering.

**Result:** PASS

**Observation:** The system correctly allowed irrigation when adequate water was available.

---

### T08 — High Temperature / Low Humidity

**Condition:** Temperature is relatively high and humidity is relatively low.

**Expected behavior:**

Higher estimated evapotranspiration results in increased pump operating time.

**Result:** PASS

**Observation:** Pump operating time increased under conditions associated with higher evapotranspiration.

---

### T09 — Low Temperature / High Humidity

**Condition:** Temperature is relatively low and humidity is relatively high.

**Expected behavior:**

Lower estimated evapotranspiration results in reduced pump operating time.

**Result:** PASS

**Observation:** Pump operating time decreased under conditions associated with lower evapotranspiration.

---

### T10 — Crop-Specific Irrigation

**Condition:** Different crops are selected.

**Expected behavior:**

The irrigation threshold and base pump time change according to the selected crop.

Supported crop categories:

* Rice
* Potato
* Wheat
* Onion
* Tomato
* Others

**Result:** PASS

**Observation:** Crop selection correctly changed the irrigation parameters used by the controller.

---

### T11 — Soil pH Classification

**Condition:** Soil pH is measured.

**Expected behavior:**

The system classifies the soil condition as:

* Acidic
* Neutral
* Basic

**Result:** PASS

**Observation:** The system correctly classified the soil condition based on the measured pH.

---

### T12 — Fertilizer Recommendation

**Condition:** Soil pH is measured.

**Expected behavior:**

The system generates an appropriate fertilizer recommendation based on the soil condition.

**Result:** PASS

**Observation:** The fertilizer recommendation logic correctly responded to the detected soil pH condition.

---

### T13 — Blynk Monitoring

**Condition:** ESP32 is connected to Wi-Fi and Blynk.

**Expected behavior:**

Sensor and system information is transmitted to the Blynk dashboard.

**Result:** PASS

**Observation:** The system successfully transmitted monitoring information to the Blynk dashboard.

---

### T14 — Fault Detection

**Condition:** A supported sensor, connectivity, or system fault condition is introduced.

**Expected behavior:**

The fault-detection logic identifies or handles the abnormal condition.

**Result:** PASS

**Observation:** The implemented fault-handling logic correctly responded to the tested fault conditions.

---

### T15 — Pump Dry-Run / Low-Water Protection

**Condition:** An insufficient-water or pump dry-run condition occurs.

**Expected behavior:**

Unsafe pump operation is prevented or handled by the safety logic.

**Result:** PASS

**Observation:** The pump protection logic correctly prevented unsafe operation under the tested condition.

---

## 4. Test Environment

| Parameter               | Configuration                        |
| ----------------------- | ------------------------------------ |
| Microcontroller         | ESP32                                |
| Development Environment | Arduino IDE                          |
| IoT Platform            | Blynk                                |
| Connectivity            | Wi-Fi                                |
| Soil Moisture Sensor    | Capacitive Soil Moisture Sensor v1.2 |
| Temperature / Humidity  | DHT11                                |
| Rain Detection          | Rain Sensor                          |
| Soil pH                 | pH Sensor                            |
| Tank Level              | HC-SR04                              |
| Motor Driver            | L298N                                |
| Actuator                | Water Pump                           |

---

## 5. Validation Coverage

The testing covered the major functional areas of the system:

### Sensor Processing

* Soil moisture
* Temperature
* Humidity
* Rain
* Soil pH
* Water-tank level

### Irrigation Decision Logic

* Moisture threshold
* Crop-specific control
* Rain detection
* Weather forecast
* Critical-moisture override
* Environmental compensation

### Safety

* Empty-tank protection
* Pump protection
* Fault handling

### IoT

* Wi-Fi connectivity
* Blynk monitoring
* User alerts

---

## 6. Overall Result

```text
Total Test Cases : 15
Passed           : 15
Failed           : 0

Success Rate     : 100%
```

```text
┌──────────────────────────────────────┐
│          VALIDATION SUMMARY          │
├──────────────────────────────────────┤
│                                      │
│   Test Cases        15               │
│   Passed            15               │
│   Failed             0               │
│                                      │
│   Overall Result    PASS             │
│                                      │
└──────────────────────────────────────┘
```

---

## 7. Supporting Results

The project also includes graphical analysis of:

* Soil moisture variation
* Pump operating time versus temperature and humidity
* Crop-specific soil-moisture thresholds

These results provide additional evidence of the adaptive behavior implemented in the irrigation controller.

See:

* `soil-moisture.png`
* `pump-runtime.png`
* `crop-thresholds.png`

---

## 8. Conclusion

The functional testing verified that the Smart Plant Irrigation System correctly responds to the tested irrigation, environmental, safety, crop-selection, pH, fault-handling, and IoT conditions.

All 15 defined test cases passed during project validation.

**Final Validation Result: 15/15 PASS**
