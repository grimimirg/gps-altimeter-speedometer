# GPS Altimeter & Speedometer

A portable device capable of displaying **GPS coordinates, altitude, and speed** on a small OLED screen.  
Perfect for hiking, cycling, or mountain activities where you want to know your position and pace without relying on a smartphone.

---

## Features
- Reads GPS data (**latitude, longitude, altitude, speed**).  
- Displays information on a **128x64 OLED display** (via I2C).  
- Runs on a battery (3.7 V Li-ion + TP4056 charging module recommended).  
- **Improved altitude readings** by combining GPS and **barometric sensor (BMP280/BME280)**.  
- Implements a **Kalman filter** to fuse GPS and barometric data:  
  - GPS provides the absolute reference altitude.  
  - Barometer provides smooth, fast relative changes (stairs, slopes, climbing).  
  - The Kalman filter merges both, correcting GPS “jumps” and barometric drift.  
- Extendable and customizable for additional sensors or logging functions.

---

## Hardware Components
- **ESP32** (tested on ESP32-S3, but adaptable to other boards)  
- **GPS module** (e.g., NEO-6M, NEO-7M, NEO-8M, u-blox M8N)  
- **OLED Display** (128×64, SSD1306, I2C)  
- **Barometric sensor**: BMP280 or BME280  
- **Battery** (e.g., 18650 Li-ion)  
- **3D printed box**  

---

## Wiring
- **GPS → ESP32**  
  - GPS TX → ESP32 GPIO 48 (RX2)  
  - VCC → 3.3V or 5V (depending on module)  
  - GND → GND  

- **OLED → ESP32**  
  - SDA → GPIO 20  
  - SCL → GPIO 21  
  - VCC → 3.3V  
  - GND → GND  

- **Barometric Sensor (BMP280/BME280) → ESP32 (I2C)**  
  - SDA → GPIO 20  
  - SCL → GPIO 21  
  - VCC → 3.3V  
  - GND → GND  

---

## Software
### Libraries Required
Install these from the Arduino Library Manager:  
- [TinyGPS++](https://github.com/mikalhart/TinyGPSPlus)  
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)  
- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)  
- [Adafruit BMP280](https://github.com/adafruit/Adafruit_BMP280_Library)  
- [SimpleKalmanFilter](https://github.com/denyssene/SimpleKalmanFilter)  

### How the Fusion Works
- The **barometer** gives very stable readings but is affected by weather changes.  
- The **GPS** gives absolute altitude but with noisy, sometimes jumpy data.  
- The **Kalman filter** continuously smooths the barometric altitude and, every few seconds, corrects it towards the GPS reference.  
- The result is a **fused altitude**: smooth like the barometer, but anchored to the true GPS level.

---

## Example Code
The example sketch included in this repo:  
- Initializes GPS on `Serial2` (GPIO 48).  
- Reads coordinates, GPS altitude, and speed with TinyGPS++.  
- Reads barometric altitude from BMP280.  
- Fuses the two values with a Kalman filter.  
- Displays **latitude, longitude, GPS altitude, barometric altitude, and fused altitude** on the OLED.

---

## Future Improvements
- Add the speed feature
