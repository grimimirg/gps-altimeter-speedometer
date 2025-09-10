# GPS Altimeter & Speedometer

A portable device capable of displaying **GPS coordinates, altitude, and speed** on a small OLED screen.  
Perfect for hiking, cycling, or mountain activities where you want to know your position and pace without relying on a smartphone.

---

## Features
- Reads GPS data (latitude, longitude, altitude, speed).
- Displays information on a **128x64 OLED display** (via I2C).
- Runs on a battery.
- Extendable -> you can easily add a barometric sensor (BMP280/BME280) for more accurate altitude readings.

---

## Hardware Components
- **ESP32**
- **GPS module** (e.g., NEO-6M, NEO-7M, NEO-8M, u-blox M8N)  
- **OLED Display** (128×64, SSD1306)  
- **Battery**
- **3D printed box**  

---

## Wiring
- **GPS → ESP32**  
  - GPS TX → ESP32 GPIO 16 (RX2)  
  - GPS RX → ESP32 GPIO 17 (TX2, optional)  
  - VCC → 3.3V or 5V (depending on module)  
  - GND → GND  

- **OLED → ESP32**  
  - SDA → GPIO 21  
  - SCL → GPIO 22  
  - VCC → 3.3V  
  - GND → GND  

---

## Software
### Libraries Required
- [TinyGPS++](https://github.com/mikalhart/TinyGPSPlus)  
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)  
- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)  

Install them from the Arduino Library Manager.

### Example Code
The example sketch:  
- Initializes GPS on `Serial2` (pins 16 & 17).  
- Reads latitude and longitude with TinyGPS++.  
- Displays them on the OLED screen.  
- NOTE: Can be extended to show altitude and speed.  

---

## Future Improvements
- Add **barometric sensor** (BMP280/BME280) for more stable altitude readings.

---

## License
This project is open-source and free to use, modify, and share.  
