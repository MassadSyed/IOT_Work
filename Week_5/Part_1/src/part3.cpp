#include <Arduino.h>

// ======== BUZZER SETUP ========
#define BUZZER_PIN  27      // GPIO connected to buzzer
#define BUZZER_CH   0       // PWM channel for buzzer
#define BUZZER_FREQ 2000    // Default frequency
#define BUZZER_RES  10      // Resolution (0–1023)

// ======== LED SETUP ========
#define LED1_PIN 18          // Fading LED
#define LED2_PIN 19          // Blinking LED
#define LED_CH   1           // PWM channel for LED1
#define LED_FREQ 5000
#define LED_RES  8

void setup() {
  // --- Setup Buzzer ---
  ledcSetup(BUZZER_CH, BUZZER_FREQ, BUZZER_RES);
  ledcAttachPin(BUZZER_PIN, BUZZER_CH);

  // --- Setup LEDs ---
  ledcSetup(LED_CH, LED_FREQ, LED_RES);
  ledcAttachPin(LED1_PIN, LED_CH);
  pinMode(LED2_PIN, OUTPUT);

  // --- 1. Simple beep pattern ---
  for (int i = 0; i < 3; i++) {
    ledcWriteTone(BUZZER_CH, 2000 + i * 400);
    digitalWrite(LED2_PIN, HIGH);
    delay(150);
    ledcWrite(BUZZER_CH, 0);
    digitalWrite(LED2_PIN, LOW);
    delay(150);
  }

  // --- 2. Frequency sweep (400Hz → 3kHz) ---
  for (int f = 400; f <= 3000; f += 100) {
    ledcWriteTone(BUZZER_CH, f);
    digitalWrite(LED2_PIN, !digitalRead(LED2_PIN));
    delay(20);
  }
  ledcWrite(BUZZER_CH, 0);
  digitalWrite(LED2_PIN, LOW);
  delay(500);

  // --- 3. Short melody ---
  int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};
  for (int i = 0; i < 8; i++) {
    ledcWriteTone(BUZZER_CH, melody[i]);
    digitalWrite(LED2_PIN, HIGH);
    delay(250);
    digitalWrite(LED2_PIN, LOW);
  }
  ledcWrite(BUZZER_CH, 0);
}

// ======== LOOP – LED Fading Effect ========
void loop() {
  // Fade LED1 up and down continuously
  for (int d = 0; d <= 255; d++) {
    ledcWrite(LED_CH, d);
    delay(10);
  }
  for (int d = 255; d >= 0; d--) {
    ledcWrite(LED_CH, d);
    delay(10);
  }
}
