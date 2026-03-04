# RPi Energy Monitor

This project is a **C application developed for the Raspberry Pi** (specifically designed for the **RPi 4**). The system acts as an **energy data acquisition gateway**, reading electrical measurements from an energy meter via the **Modbus protocol** and periodically publishing this information to an **MQTT broker** in **JSON format**.

The system is intended for **industrial and residential automation and monitoring applications**, enabling analysis of **energy consumption, power factor, and power quality**.

---

## Features

The application sequentially reads the following electrical quantities, using **100 ms intervals between requests** to ensure bus stability:

* Voltage (V)
* Current (A)
* Frequency (Hz)
* Active Power (W)
* Reactive Power (VAr)
* Apparent Power (VA)
* Power Factor

---

## Software Architecture

The main source file (`main.c`) relies on two custom modules that abstract hardware and network communication:

* `meter.h`  
  Responsible for **initializing and handling Modbus communication** with the physical energy meter.

* `mqtt.h`  
  Responsible for **managing the MQTT broker connection** and publishing messages.

---

## Data Format (MQTT Payload)

After performing the readings, the data is displayed in the **local console** and formatted into a **JSON string**. The message is published every **5 seconds** to the topic `energy/7m`.

**MQTT Topic:** `energy/7m`  
**Client ID:** `rpi4-energy`

**Example JSON Payload:**

```json
{
  "voltage": 220,
  "current": 5.42,
  "frequency": 60,
  "active_power": 1100,
  "reactive_power": 150,
  "apparent_power": 1110,
  "power_factor": 0.95
}
```
