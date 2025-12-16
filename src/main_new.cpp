#include <Arduino.h>
#include <AccelStepper.h>

// Define stepper motor (Driver mode: step pin 7, direction pin 6)
AccelStepper stepper(1, 7, 6);

// Pin definitions
const int JOYSTICK_X = A0;      // Joystick X-axis (horizontal)
const int JOYSTICK_Y = A1;      // Joystick Y-axis (vertical)
const int POT_PIN = A2;         // Potentiometer for speed control (if still using)

// Joystick calibration
const int JOY_CENTER = 512;     // Center position of joystick (0-1023 range)
const int JOY_DEADZONE = 100;   // Deadzone around center to prevent drift

// Speed settings
const int MAX_SPEED = 400;      // Maximum speed for steady movement (steps/sec)
const int MIN_SPEED = 50;       // Minimum speed when joystick barely moved

void setup() {
  // Initialize serial for debugging (optional)
  Serial.begin(9600);
  
  // Configure stepper motor
  stepper.setMaxSpeed(MAX_SPEED);
  stepper.setAcceleration(200);   // Smooth acceleration/deceleration
  
  Serial.println("Robotic Arm Base Control Ready");
  Serial.println("Move joystick X-axis (right) for clockwise rotation");
  Serial.println("Move joystick Y-axis (up) for counter-clockwise rotation");
}

void loop() {
  // Read joystick values
  int joyX = analogRead(JOYSTICK_X);
  int joyY = analogRead(JOYSTICK_Y);
  
  // Calculate distance from center for each axis
  int deltaX = joyX - JOY_CENTER;
  int deltaY = joyY - JOY_CENTER;
  
  float targetSpeed = 0;
  
  // Check X-axis (right = clockwise)
  if (abs(deltaX) > JOY_DEADZONE) {
    // Map joystick position to speed (right = positive/clockwise)
    if (deltaX > 0) {
      targetSpeed = map(deltaX, JOY_DEADZONE, 512, MIN_SPEED, MAX_SPEED);
    } else {
      targetSpeed = -map(abs(deltaX), JOY_DEADZONE, 512, MIN_SPEED, MAX_SPEED);
    }
  }
  // Check Y-axis (up = counter-clockwise) - only if X is in deadzone
  else if (abs(deltaY) > JOY_DEADZONE) {
    // Map joystick position to speed (up = negative/counter-clockwise)
    if (deltaY > 0) {
      targetSpeed = -map(deltaY, JOY_DEADZONE, 512, MIN_SPEED, MAX_SPEED);
    } else {
      targetSpeed = map(abs(deltaY), JOY_DEADZONE, 512, MIN_SPEED, MAX_SPEED);
    }
  }
  
  // Set motor speed (0 when joystick is centered)
  stepper.setSpeed(targetSpeed);
  
  // Run the motor - only moves if speed is non-zero
  if (targetSpeed != 0) {
    stepper.runSpeed();
  }
  
  // Optional: Print status for debugging
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 500) {
    if (targetSpeed != 0) {
      Serial.print("Speed: ");
      Serial.print(targetSpeed);
      Serial.print(" | Direction: ");
      Serial.println(targetSpeed > 0 ? "Clockwise" : "Counter-clockwise");
    }
    lastPrint = millis();
  }
  
  // Small delay for stability
  delay(10);
}