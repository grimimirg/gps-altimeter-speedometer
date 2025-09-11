#include <Wire.h>
#include <TinyGPSPlus.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- DISPLAY DIMENSIONS ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// --- DISPLAY CONFIG ---
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- GPS CONFIG ---
TinyGPSPlus gps;
HardwareSerial serialGPS(2);   // UART2 is usually used for GPS

// GPS receiver talks to the ESP32 on this pin
#define RX 16
#define GPS_BAUD 9600

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");

  // Override of the standard pins for I2C
  // !!! Verify that your ESP32 actually exposes GPIO20 and GPIO21 !!!
  // If not, use other free pins like (4, 5) or (18, 19)
  Wire.begin(20, 21); // SDA = 20, SCL = 21

  // Initialize display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.println("Display not found!");
    display.display();
    for (;;); // stop here if no display
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.println("Display found!");
  display.display();

  delay(1000);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Starting GPS...");
  display.display();

  // Start GPS (only RX pin used, TX not needed)
  serialGPS.begin(GPS_BAUD, SERIAL_8N1, RX, -1);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("GPS started!");
  display.display();

  delay(1000);
}

void loop() {
  // Reads incoming data from the GPS module
  while (serialGPS.available() > 0) {
    gps.encode(serialGPS.read());
  }

  if (gps.location.isUpdated()) {
    double lat = gps.location.lat();
    double lon = gps.location.lng();

    // show data on the display
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("GPS Position:");

    display.setTextSize(1);
    display.print("Lat: ");
    display.println(lat, 6);
    display.print("Lon: ");
    display.println(lon, 6);

    display.display();

    // debug also on serial
    Serial.print("Lat: "); Serial.print(lat, 6);
    Serial.print("  Lon: "); Serial.println(lon, 6);
  }
}
