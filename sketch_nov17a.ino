#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Define the pins
#define TRIG_PIN 8
#define ECHO_PIN 9
#define PROXIMITY_SENSOR_PIN 10
#define SERVO_PIN 11

// Initialize servo and LCD
Servo sortingServo;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust I2C address if necessary

// Variables to track counts
int metalCount = 0;
int nonMetalCount = 0;

// Function to measure distance using Ultrasonic Sensor
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2; // Convert to cm
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Initialize pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PROXIMITY_SENSOR_PIN, INPUT);
  
  // Initialize servo motor
  sortingServo.attach(SERVO_PIN);
  sortingServo.write(90); // Neutral position
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Garbage Bin");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Measure distance
  long distance = getDistance();

  if (distance < 10) { // Adjust this threshold for your setup
    delay(500); // Debounce delay
    int isMetal = digitalRead(PROXIMITY_SENSOR_PIN);

    if (isMetal == HIGH) { // Metal detected
      metalCount++;
      sortingServo.write(0); // Turn servo to the right
      delay(1000);
      sortingServo.write(90); // Reset position
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Metal Count:");
      lcd.setCursor(0, 1);
      lcd.print(metalCount);
    } else { // Non-metal detected
      nonMetalCount++;
      sortingServo.write(180); // Turn servo to the left
      delay(1000);
      sortingServo.write(90); // Reset position
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Non-Metal Count:");
      lcd.setCursor(0, 1);
      lcd.print(nonMetalCount);
    }

    delay(1000); // Wait before processing the next object
  }
}

