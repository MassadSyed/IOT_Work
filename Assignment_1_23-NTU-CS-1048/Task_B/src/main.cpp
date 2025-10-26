#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- Pin Definitions ---
#define BTN_PIN   25      // Button input
#define LED_PIN   18      // LED output (PWM)
#define BUZZER    19      // Buzzer output

// --- LED PWM Setup (ESP32) ---
#define CH_LED    0       // PWM channel for LED
#define PWM_FREQ  5000    // PWM frequency 5kHz
#define PWM_RES   8       // 8-bit resolution (0-255)

// --- Debounce and Press Detection ---
bool lastButtonState = HIGH;       // Last read state of button
bool buttonPressed = false;        // Track if button is currently pressed
unsigned long pressStartTime = 0;  // Time when button press started
unsigned long lastDebounceTime = 0; 
const unsigned long DEBOUNCE_MS = 30;      // Debounce time in ms
const unsigned long LONG_PRESS_MS = 1500;  // Long press threshold

// --- LED & Buzzer states ---
bool ledState = false;             // LED on/off state
bool buzzerOn = false;             // Buzzer on/off state
unsigned long buzzerStartTime = 0; // Time when buzzer was turned on
const unsigned long BUZZER_DURATION = 200; // Buzzer duration in ms

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);  // Set button as input with pull-up
  pinMode(BUZZER, OUTPUT);         // Set buzzer as output
  digitalWrite(BUZZER, LOW);       // Ensure buzzer is off

  // --- Setup LED PWM ---
  ledcSetup(CH_LED, PWM_FREQ, PWM_RES);  // Configure PWM
  ledcAttachPin(LED_PIN, CH_LED);        // Attach PWM to LED pin
  ledcWrite(CH_LED, 0);                  // Start with LED off

  // --- Setup OLED ---
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;); // Halt if OLED not found
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("Ready");  // Initial message
  display.display();
}

void loop() {
  unsigned long currentMillis = millis();
  bool reading = digitalRead(BTN_PIN);  // Read button

  // --- Debounce button ---
  if (reading != lastButtonState) {     // State changed
    lastDebounceTime = currentMillis;   // Reset debounce timer
    lastButtonState = reading;          // Save new state
  }

  if ((currentMillis - lastDebounceTime) > DEBOUNCE_MS) {
    // --- Button pressed ---
    if (!buttonPressed && reading == LOW) {
      buttonPressed = true;             // Mark as pressed
      pressStartTime = currentMillis;   // Save press start time
    }

    // --- Button released ---
    if (buttonPressed && reading == HIGH) {
      unsigned long pressDuration = currentMillis - pressStartTime;
      buttonPressed = false;            // Reset press flag

      if (pressDuration >= LONG_PRESS_MS) {
        // --- Long Press Action ---
        digitalWrite(BUZZER, HIGH);     // Turn on buzzer
        buzzerOn = true;
        buzzerStartTime = currentMillis;

        display.clearDisplay();
        display.setCursor(0, 20);
        display.println("Long Press");  // Show message
        display.display();
      } else {
        // --- Short Press Action ---
        ledState = !ledState;           // Toggle LED
        ledcWrite(CH_LED, ledState ? 255 : 0);

        display.clearDisplay();
        display.setCursor(0, 20);
        display.println(ledState ? "LED ON" : "LED OFF"); // Show LED status
        display.display();
      }
    }
  }

  // --- Turn off buzzer after duration ---
  if (buzzerOn && (currentMillis - buzzerStartTime >= BUZZER_DURATION)) {
    digitalWrite(BUZZER, LOW);   // Turn off buzzer
    buzzerOn = false;
  }
}
