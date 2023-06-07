#include <LiquidCrystal.h>

// Pin numbers for the pushbuttons
const int interrupt_pin = 11;
const int increase_pin = 7;
const int decrease_pin = 8;

// Pin numbers for the L293D IC
const int enable_pin = 6;   // Enable pin 1 of L293D
const int input1_pin = 9;  // Input 1 pin of L293D
const int input2_pin = 10;  // Input 2 pin of L293D

// Variables to store the current RPM and target RPM
int currentSpeed = 0;
const int minSpeed = 0;
const int maxSpeed = 255;
const int turnoffSpeed = 20;
int motorspeed = 0;

// LCD module connections (change according to your wiring)
const int rs = 2;
const int en = 3;
const int d4 = 4;
const int d5 = 5;
const int d6 = 12;
const int d7 = 13;
LiquidCrystal lcd(2, 3, 4, 5, 12, 13);

bool inputMode = true;

void setup() {
  pinMode(interrupt_pin, INPUT_PULLUP);
  pinMode(increase_pin, INPUT_PULLUP);
  pinMode(decrease_pin, INPUT_PULLUP);
  
  pinMode(enable_pin, OUTPUT);
  pinMode(input1_pin, OUTPUT);
  pinMode(input2_pin, OUTPUT);

  // Initialize the LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Press K1 For");  // Display initial message on LCD
  lcd.setCursor(0, 1);
  lcd.print(" Input Mode");

  Serial.begin(9600); // Initialize Serial Monitor
}

void loop() {
  // Check if K1 is pressed to enable input mode
  if (digitalRead(interrupt_pin) == LOW) {
    inputMode = !inputMode;
    delay(100); // Delay to debounce the button
  }

  if (inputMode) {
    // Input mode is enabled, allow speed adjustment

    // Poll the increase pin to check if it is pressed to increase the speed
    if (digitalRead(increase_pin) == LOW) {
      increaseSpeed();
    }

    // Poll the decrease pin to check if it is pressed to decrease the speed
    if (digitalRead(decrease_pin) == LOW) {
      decreaseSpeed();
    }
  }
  
  analogWrite(enable_pin, currentSpeed);
  delay(10); // Delay to stabilize the motor
  
  if (currentSpeed < turnoffSpeed) {
    analogWrite(enable_pin, 0);  // Turn off the motor
  }
  
  // Check if K1 is pressed again to display current speed
  if (digitalRead(interrupt_pin) == LOW) {
    printCurrentSpeed();
    delay(500); // Delay to debounce the button
  }
}

void printCurrentSpeed() {
  int currentRPM = map(currentSpeed, minSpeed, maxSpeed, 0, 255);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Motor Speed:");
  lcd.setCursor(0, 1);
  lcd.print(currentSpeed);
  Serial.print("Current Speed: ");
  Serial.println(currentSpeed);
}

void increaseSpeed() {
  digitalWrite(input1_pin, HIGH);
  digitalWrite(input2_pin, LOW);
  digitalWrite(enable_pin, currentSpeed);
  currentSpeed += 10;
  if (currentSpeed > maxSpeed) {
    currentSpeed = maxSpeed;
  }

  Serial.print("Motor Speed: ");
  Serial.println(currentSpeed);
  delay(200);
}

void decreaseSpeed() {
  digitalWrite(input1_pin, HIGH);
  digitalWrite(input2_pin, LOW);
  digitalWrite(enable_pin, currentSpeed);
  currentSpeed -= 10;
  if (currentSpeed < minSpeed) {
    currentSpeed = minSpeed;
  }

  Serial.print("Motor Speed: ");
  Serial.println(currentSpeed);
  delay(200);
}
