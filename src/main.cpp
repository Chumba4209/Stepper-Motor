#include <Arduino.h>
#include <AccelStepper.h>

// Define stepper motor (Driver mode: step pin 7, direction pin 6)
AccelStepper stepper(AccelStepper::DRIVER, 9, 10);

// Pin definitions
const int JOYSTICK_X = A0;      // Joystick X-axis (horizontal)
// Y-axis intentionally unused

// Joystick calibration
const int JOY_CENTER  = 512;   // Center position of joystick (0–1023)
const int JOY_DEADZONE = 100;  // Deadzone to prevent drift

// Speed settings
const int MAX_SPEED = 400;     // Max speed (steps/sec)
const int MIN_SPEED = 50;      // Min speed when joystick just moves

void setup() {
  Serial.begin(9600);

  // Stepper configuration
  stepper.setMaxSpeed(MAX_SPEED);
  stepper.setAcceleration(200);

  Serial.println("Robotic Arm Base Control Ready");
  Serial.println("Joystick RIGHT  → Clockwise");
  Serial.println("Joystick LEFT   → Counter-clockwise");
}

void loop() {
  // Read joystick X-axis
  int joyX = analogRead(JOYSTICK_X);

  // Distance from center
  int deltaX = joyX - JOY_CENTER;

  float targetSpeed = 0;

  // X-axis controls direction and speed
  if (abs(deltaX) > JOY_DEADZONE) {
    if (deltaX > 0) {
      // Right → Clockwise
      targetSpeed = map(deltaX, JOY_DEADZONE, 512, MIN_SPEED, MAX_SPEED);
    } else {
      // Left → Counter-clockwise
      targetSpeed = -map(abs(deltaX), JOY_DEADZONE, 512, MIN_SPEED, MAX_SPEED);
    }
  }

  // 🔁 Direction correction applied here
  stepper.setSpeed(-targetSpeed);

  // Run motor only when joystick is moved
  if (targetSpeed != 0) {
    stepper.runSpeed();
  }

  // Debug output
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 500) {
    Serial.print("Joystick X: ");
    Serial.print(joyX);
    Serial.print(" | Speed: ");
    Serial.print(abs(targetSpeed));
    Serial.print(" | Direction: ");
    Serial.println(
      targetSpeed > 0 ? "Clockwise" :
      targetSpeed < 0 ? "Counter-clockwise" : "Stopped"
    );
    lastPrint = millis();
  }

  delay(10);
}