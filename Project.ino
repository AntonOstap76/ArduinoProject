#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define Password_Length 9
char userInput[Password_Length];
char Master[Password_Length] = "1";
char customKey;
byte pressCount = 0;

const byte ROWS = 4; // Rows and columns for the 4x4 keypad
const byte COLS = 4;

char twelveKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; // Row pins on Arduino
byte colPins[COLS] = {5, 4, 3, 2}; // Column pins on Arduino

const int buzzerPin = 11; // Define buzzer pin
const int greenLedPin = 10; // Define green LED pin
const int redLedPin = 12; // Define red LED pin

Keypad customKeypad = Keypad(makeKeymap(twelveKeys), rowPins, colPins, ROWS, COLS);

void setup() {
  Wire.begin();
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Enter Password:");

  pinMode(buzzerPin, OUTPUT); // Set buzzer pin as output
  pinMode(greenLedPin, OUTPUT); // Set green LED pin as output
  pinMode(redLedPin, OUTPUT); // Set red LED pin as output
}

void loop() {
  customKey = customKeypad.getKey();

  if (customKey != NO_KEY && customKey != 'C' && customKey != 'D') {
    userInput[pressCount] = customKey;
    lcd.setCursor(pressCount, 1);
    lcd.print("*");
    pressCount++;
  }
  else if (customKey == 'C') {
    lcd.clear();
    clearData(); 
    lcd.print("Enter Password:");
  }
  else if (customKey == 'D') {
    lcd.clear();
    lcd.print("Checking Password...");
    delay(1000);
    if (!strcmp(userInput, Master)) {
      lcd.clear();
      lcd.print("Access Granted");
      digitalWrite(greenLedPin, HIGH); // Turn on green LED
      buzz(1000, 500); // Buzz for 1 second
      delay(2000);
    } else {
      lcd.clear();
      lcd.print("Access Denied");
      digitalWrite(redLedPin, HIGH); // Turn on red LED
      buzz(200, 200); // Buzz for 200 ms
      delay(2000);
    }
    clearData();
    lcd.clear();
    lcd.print("Enter Password:");
    digitalWrite(greenLedPin, LOW); // Turn off green LED
    digitalWrite(redLedPin, LOW); // Turn off red LED
  }
}

void clearData() {
  for (int i = 0; i < Password_Length; i++) {
    userInput[i] = '\0';
  }
  pressCount = 0;
}

void buzz(unsigned int frequency, unsigned int duration) {
  tone(buzzerPin, frequency);
  delay(duration);
  noTone(buzzerPin);
}
