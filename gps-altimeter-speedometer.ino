#include <Wire.h>
#include <TinyGPSPlus.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- DISPLAY CONFIG ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Override of the standard pins for I2C
// SDA = 20
// SCL = 21 
//
// NOTE! Please use SDA = 21 and SCL = 22 on non ESP32-S3
#define SDA 20
#define SCL 21

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- GPS CONFIG ---
TinyGPSPlus gps;
HardwareSerial serialGPS(2);   // UART number 2 is usually used for GPS

// GPS receiver talks to the ESP32 on this pin via TX pin
#define RX 48
#define GPS_BAUD 9600

#define LINE_HEIGHT 10

int currentLine = 0;

bool acqSignal = false;

void clearDisplay() {
  display.clearDisplay();
}

void printLine(const String& text, int line, int size = 1) {
  display.setCursor(0, line * LINE_HEIGHT * size);
  display.setTextSize(size);
  display.setTextColor(SSD1306_WHITE);
  display.println(text);
}

void refreshDisplay() {
  display.display();
}

// log methods
void clearLog() {
  display.clearDisplay();
  currentLine = 0;
}

void logLine(const String& text, int size = 1) {
  display.setCursor(0, currentLine * LINE_HEIGHT * size);
  display.setTextSize(size);
  display.setTextColor(SSD1306_WHITE);
  display.println(text);
  display.display();
  currentLine++;
}

//--------------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);

  Wire.begin(SDA, SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Display not found!");
    for (;;);
  }

  clearLog();
  logLine("Display found");

  serialGPS.begin(GPS_BAUD, SERIAL_8N1, RX, -1);
  logLine("GPS found");
}

void loop() {
  while (serialGPS.available() > 0) {
    gps.encode(serialGPS.read());
  }

  if (gps.location.isValid()) {
    clearDisplay();

    printLine("Lat: " + String(gps.location.lat(), 6), 0);
    printLine("Lon: " + String(gps.location.lng(), 6), 1);
    printLine("Alt: " + String(gps.altitude.meters()), 2);

    refreshDisplay();
  } else {
    if (!acqSignal) {
      acqSignal = true;
      logLine("Acq. GPS signal...");
    }
  }

  delay(500);
}

