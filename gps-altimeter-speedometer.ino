#include <Wire.h>
#include <TinyGPSPlus.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include <Adafruit_BMP280.h>
#include <SimpleKalmanFilter.h>

// --- DISPLAY CONFIG ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define SDA 20
#define SCL 21

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- GPS CONFIG ---
TinyGPSPlus gps;
HardwareSerial serialGPS(2);

#define RX 48
#define GPS_BAUD 9600

#define LINE_HEIGHT 10

// --- BAROMETRO CONFIG ---
//Adafruit_BMP280 bmp; // I2C
float baroAltitude = 0;

// --- KALMAN FILTER CONFIG ---
SimpleKalmanFilter kalmanAltitude(2, 2, 0.01);
float gpsAltitude = 0;
float fusedAltitude = 0;

unsigned long lastGPSUpdate = 0;

// Time lapse for corrections
const unsigned long gpsUpdateInterval = 5000;

int currentLine = 0;
bool acqSignal = false;

// --- SPEED ---
double prevLat = 0.0;
double prevLon = 0.0;

long prevTime = 0;

// --- DISPLAY UTILS ---
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

  // --- DISPLAY ---
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Display not found!");
    for (;;);
  }

  clearLog();
  logLine("Display found");

  // --- GPS ---
  serialGPS.begin(GPS_BAUD, SERIAL_8N1, RX, -1);
  logLine("GPS found");

  // --- Barometer ---
  //if (!bmp.begin(0x76)) {
  //  logLine("Barometer not found");
  //  while (1);
  //}
  //
  //logLine("BMP280 found");

  delay(4000);
}

void loop() {
  // --- Read from GPS ---
  while (serialGPS.available() > 0) {
    gps.encode(serialGPS.read());
  }

  if (gps.location.isValid()) {
    gpsAltitude = gps.altitude.meters();
  }

  // --- Read from Barometer ---
  //baroAltitude = bmp.readAltitude(1013.25);

  //fusedAltitude = kalmanAltitude.updateEstimate(baroAltitude);

  // --- GPS correction ---
  //if (millis() - lastGPSUpdate > gpsUpdateInterval && gps.altitude.isValid()) {
  //  float correction = 0.2 * (gpsAltitude - fusedAltitude);
  //  fusedAltitude += correction;
  //  lastGPSUpdate = millis();
  //}

  clearDisplay();

  if (gps.location.isValid()) {
    printLine("Lat: " + String(gps.location.lat(), 6), 0);
    printLine("Lon: " + String(gps.location.lng(), 6), 1);
  } else {
    printLine("Lat: --", 0);
    printLine("Lon: --", 1);
  }
  
  printLine("Alt GPS: " + String(gpsAltitude, 1) + " m", 2);
  //printLine("Alt Baro: " + String(baroAltitude, 1) + " m", 3);
  //printLine("Alt Fused: " + String(fusedAltitude, 1) + " m", 4);

  float speed = gps.speed.kmph();
  printLine("Speed: " + String(speed, 1) + " km/h", /*5*/3);

  refreshDisplay();

  delay(500);
}
