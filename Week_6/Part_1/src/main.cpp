#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ------------------- Pin Configuration -------------------
#define DHTPIN 14          // DHT22 data pin
#define DHTTYPE DHT11      // DHT sensor type
#define LDR_PIN 36         // LDR analog input pin
#define SDA_PIN 21         // I2C SDA
#define SCL_PIN 22         // I2C SCL

// ------------------- OLED Configuration -------------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ------------------- Sensor Objects -------------------
DHT dht(DHTPIN, DHTTYPE);

// ------------------- Setup -------------------
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Sensor Monitor Starting...");

  // Initialize I2C on custom pins
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed!");
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Initializing Sensors...");
  display.display();

  // Initialize DHT sensor
  dht.begin();
  delay(1500);

  Serial.println("Setup complete.");
}

// ------------------- Main Loop -------------------
void loop() {
  // --- Read DHT22 ---
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // --- Read LDR ---
  int adcValue = analogRead(LDR_PIN);
  float voltage = (adcValue / 4095.0) * 3.3; // ESP32 ADC 12-bit, 3.3V ref

  // --- Validate DHT Readings ---
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error reading from DHT sensor!");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("DHT Error!");
    display.display();
    delay(2000);
    return;
  }

  // --- Calculate LDR Resistance & Light Intensity ---
  float R_fixed = 10000.0; // 10k ohm resistor
  float R_ldr = (3.3 - voltage) * R_fixed / voltage;
  float lux = 500 / pow((R_ldr / 1000), 1.4); // approximate formula

  // --- Print to Serial Monitor ---
  Serial.printf("Temp: %.2f °C | Humidity: %.2f %% | LDR ADC: %d | Voltage: %.2f V | Lux: %.2f\n",
                temperature, humidity, adcValue, voltage, lux);

  // --- Display on OLED ---
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("== ESP32 Sensor Monitor ==");

  display.setCursor(0, 12);
  display.printf("Temp: %.1f C\n", temperature);

  display.setCursor(0, 24);
  display.printf("Humidity: %.1f %%\n", humidity);

  display.setCursor(0, 36);
  display.printf("LDR: %d (%.2fV)\n", adcValue, voltage);

  display.setCursor(0, 48);
  display.printf("Light: %.1f lux\n", lux);

  display.display();

  delay(2000);
}
