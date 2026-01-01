#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Servos
Servo servoD10; // XY rotation
Servo servoD9;  // ZY rotation

// Button
const int buttonPin = 2;
int buttonState = 0;
int lastButtonState = 0;

// D9 (ZY) servo settings
int servoD9Angle = 60;       // Start at 60°
const int d9Step = 10;       // Degrees per button press
const int d9Max = 150;       // Reset after 150°

// Photoresistors (XY direction)
const int photoPins[3] = {A2, A1, A3}; // Front=A2, Middle=A1, Back=A3
int lightLevels[3];

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// D10 (XY) timing
unsigned long lastMoveTime = 0;
const unsigned long moveInterval = 30000; // 30 seconds

void setup() {
  // Attach servos
  servoD10.attach(10);
  servoD9.attach(9);

  // Initialize servos
  servoD9.write(servoD9Angle); // ZY start
  servoD10.write(90);          // XY start at middle

  // Button
  pinMode(buttonPin, INPUT_PULLUP);

  // LCD setup
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  // -----------------------
  // Handle button for D9 (ZY)
  // -----------------------
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && lastButtonState == HIGH) {
    servoD9Angle += d9Step;
    if (servoD9Angle > d9Max) servoD9Angle = 0; // Reset after 150°
    servoD9.write(servoD9Angle);
    delay(200); // debounce
  }
  lastButtonState = buttonState;

  // -----------------------
  // Read light sensors
  // -----------------------
  for (int i = 0; i < 3; i++) {
    lightLevels[i] = analogRead(photoPins[i]);
  }

  // -----------------------
  // Update LCD
  // -----------------------
  lcd.setCursor(0, 0);
  lcd.print("F:");
  lcd.print(lightString(lightLevels[0]));
  lcd.print(" M:");
  lcd.print(lightString(lightLevels[1]));

  lcd.setCursor(0, 1);
  lcd.print("B:");
  lcd.print(lightString(lightLevels[2]));
  lcd.print("    "); // Clear extra chars

  // -----------------------
  // Move D10 (XY) every 30 seconds
  // -----------------------
  if (millis() - lastMoveTime >= moveInterval) {
    lastMoveTime = millis();

    // Find brightest sensor
    int maxIndex = 0;
    int maxValue = lightLevels[0];
    for (int i = 1; i < 3; i++) {
      if (lightLevels[i] > maxValue) {
        maxValue = lightLevels[i];
        maxIndex = i;
      }
    }

    // Map sensor to XY angle
    int targetAngle;
    if (maxIndex == 0) targetAngle = 0;   // Front
    else if (maxIndex == 1) targetAngle = 90; // Middle
    else targetAngle = 180;              // Back

    // Smoothly move D10 to target
    int currentAngle = servoD10.read();
    while (currentAngle != targetAngle) {
      if (currentAngle < targetAngle) currentAngle++;
      else if (currentAngle > targetAngle) currentAngle--;
      servoD10.write(currentAngle);
      delay(15); // Smooth movement
    }
  }

  delay(50); // Small delay for LCD refresh
}

// Convert analog value to readable text
String lightString(int val) {
  if (val < 200) return "LOW ";
  else if (val < 600) return "MED ";
  else return "HIGH";
}
