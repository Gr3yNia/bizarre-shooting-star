#include <Servo.h>
#include <Adafruit_CircuitPlayground.h>

Servo servo360;   // TF90R
Servo servo180;   // SG90

int lightPin = A7;
int lightValue = 0;

// --- SG90 ---
int angle = 90;
int minAngle = 45;
int maxAngle = 170;
int direction = 1;

// --- LIGHT ---
int pixelOffset = 0;

void setup() {
  CircuitPlayground.begin();

  servo360.attach(A1);
  servo180.attach(A4);

  servo360.write(100); // your stop center
  servo180.write(angle);
}

void loop() {

  lightValue = analogRead(lightPin);

  // 🔁 activity for SG90 + lights (dark = active)
  int activity = map(lightValue, 552, 925, 100, 0);
  activity = constrain(activity, 0, 100);

  // =========================
  // --- 360 SERVO CONTROL ---
  // =========================

  // --- 360 SERVO (ultra slow using pulse control) ---

  int targetSpeed = map(lightValue, 552, 925, 160, 101);
  targetSpeed = constrain(targetSpeed, 101, 160);

  // smoothing
  static float currentSpeed = 145;
  currentSpeed += (targetSpeed - currentSpeed) * 0.05;

  // pulse control (THIS is the key)
  if (lightValue > 800) {  
  // very bright → move briefly then stop
  servo360.write((int)currentSpeed);
  delay(20);
  servo360.write(100);   // stop
  delay(120);            // pause → slower effect
  } 
  else {
  // darker → normal movement
  servo360.write((int)currentSpeed);
  }

  // =========================
  // --- 180 SERVO CONTROL ---
  // =========================

  int stepSize = map(activity, 0, 100, 1, 4);

  angle += direction * stepSize;

  if (angle >= maxAngle || angle <= minAngle) {
    direction *= -1;
  }

  servo180.write(angle);

  // =========================
  // --- LIGHT SHOW ---
  // =========================

  int brightness = map(activity, 0, 100, 5, 50);
  brightness = constrain(brightness, 5, 50);

  for (int i = 0; i < 10; i++) {
    int pos = (i + pixelOffset) % 10;

    int r = brightness;
    int g = brightness * 0.4;
    int b = brightness * 0.1;

    CircuitPlayground.setPixelColor(pos, r, g, b);
  }

  pixelOffset++;

  delay(50);
}