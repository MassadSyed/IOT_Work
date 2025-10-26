#include <Arduino.h>

// ======== BUZZER SETUP ========
#define BUZZER_PIN  27
#define BUZZER_CH   0
#define BUZZER_FREQ 2000
#define BUZZER_RES  10

// ======== LED SETUP ========
#define LED1_PIN 18          // LED1 (Fade LED)
#define LED2_PIN 19          // LED2 (Blink/Fade with different freq)
#define LED1_CH  1
#define LED2_CH  2
#define LED1_FREQ 5000       // 5kHz
#define LED2_FREQ 1000       // 1kHz
#define LED_RES   8

void setup() {
  // --- Setup Buzzer ---
  ledcSetup(BUZZER_CH, BUZZER_FREQ, BUZZER_RES);
  ledcAttachPin(BUZZER_PIN, BUZZER_CH);

  // --- Setup LEDs with different PWM frequencies ---
  ledcSetup(LED1_CH, LED1_FREQ, LED_RES);
  ledcAttachPin(LED1_PIN, LED1_CH);

  ledcSetup(LED2_CH, LED2_FREQ, LED_RES);
  ledcAttachPin(LED2_PIN, LED2_CH);

  // --- Buzzer Beep Pattern ---
  for (int i = 0; i < 3; i++) {
    ledcWriteTone(BUZZER_CH, 2000 + i * 400);
    delay(150);
    ledcWrite(BUZZER_CH, 0);
    delay(150);
  }

  // --- Frequency Sweep ---
  for (int f = 400; f <= 3000; f += 100) {
    ledcWriteTone(BUZZER_CH, f);
    delay(20);
  }
  ledcWrite(BUZZER_CH, 0);
  delay(500);

  // --- Simple Melody ---
  int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};
  for (int i = 0; i < 8; i++) {
    ledcWriteTone(BUZZER_CH, melody[i]);
    delay(250);
  }
  ledcWrite(BUZZER_CH, 0);
}

void loop() {
  // ===== LED1: Smooth fade =====
  for (int d = 0; d <= 255; d++) {
    ledcWrite(LED1_CH, d);
    delay(5);
  }
  for (int d = 255; d >= 0; d--) {
    ledcWrite(LED1_CH, d);
    delay(5);
  }

  // ===== LED2: Different frequency, faster fade =====
  for (int d = 0; d <= 255; d += 15) {
    ledcWrite(LED2_CH, d);
    delay(10);
  }
  for (int d = 255; d >= 0; d -= 15) {
    ledcWrite(LED2_CH, d);
    delay(10);
  }
}
