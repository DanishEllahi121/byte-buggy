#include <SoftwareSerial.h>
#include <NewPing.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define motor pins
const int MotorL_pin1 = 2;
const int MotorL_pin2 = 3;
const int MotorR_pin1 = 4;
const int MotorR_pin2 = 5;

int enA = 9;
int enB = 10;

#define MAX_DISTANCE 500

// Define ultrasonic sensor pins
const int trig1 = 13;  //front
const int echo1 = 12;  //front
const int trig2 = 8;   //left
const int echo2 = 11;  //left
const int trig3 = 7;   //right
const int echo3 = 6;   //right

NewPing sonar1(trig1, echo1, MAX_DISTANCE);
NewPing sonar2(trig2, echo2, MAX_DISTANCE);
NewPing sonar3(trig3, echo3, MAX_DISTANCE);


// Variables for calculating distances
int distance1;  //front
int distance2;  //left
int distance3;  //right

// LCD Display
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// Timing variable for display
unsigned long lastDisplayTime = 0;
const int displayInterval = 1500;

// Function to stop both motors
void Stop() {
  digitalWrite(MotorL_pin1, LOW);
  digitalWrite(MotorL_pin2, LOW);
  digitalWrite(MotorR_pin1, LOW);
  digitalWrite(MotorR_pin2, LOW);
  delay(300);
}


// Function to move both motors reverse
void Reverse() {
  setSpeed();
  digitalWrite(MotorL_pin1, HIGH);
  digitalWrite(MotorL_pin2, LOW);
  digitalWrite(MotorR_pin1, LOW);
  digitalWrite(MotorR_pin2, HIGH);
  delay(300);
}

// 5. Function to move both motors forward
void Forward() {
  setSpeed();
  digitalWrite(MotorL_pin1, LOW);
  digitalWrite(MotorL_pin2, HIGH);
  digitalWrite(MotorR_pin1, HIGH);
  digitalWrite(MotorR_pin2, LOW);
  delay(300);
}

// Function to turn left
void Left() {
  setSpeed();
  digitalWrite(MotorL_pin1, LOW);
  digitalWrite(MotorL_pin2, HIGH);
  digitalWrite(MotorR_pin1, LOW);
  digitalWrite(MotorR_pin2, LOW);
  delay(300);
}

// Function to turn right
void Right() {
  setSpeed();
  digitalWrite(MotorL_pin1, LOW);
  digitalWrite(MotorL_pin2, LOW);
  digitalWrite(MotorR_pin1, HIGH);
  digitalWrite(MotorR_pin2, LOW);
  delay(300);
}

// Function to setSpeed
void setSpeed() {
  analogWrite(enA, 70);  //ENA  pin
  analogWrite(enB, 70);  //ENB pin
}

void setup() {
  Serial.begin(9600);

  // 8. Set motor pins as outputs
  pinMode(MotorL_pin1, OUTPUT);
  pinMode(MotorL_pin2, OUTPUT);
  pinMode(MotorR_pin1, OUTPUT);
  pinMode(MotorR_pin2, OUTPUT);

  pinMode(enA, OUTPUT);
  lcd.begin(16, 2);
  lcd.clear();
}

void loop() {
  // Ultrasonic sensor 1

  unsigned int uS1 = sonar1.ping();
  distance1 = uS1 / US_ROUNDTRIP_CM;

  // Ultrasonic sensor 2
  unsigned int uS2 = sonar2.ping();
  distance2 = uS2 / US_ROUNDTRIP_CM;

  // Ultrasonic sensor 3
  unsigned int uS3 = sonar3.ping();
  distance3 = uS3 / US_ROUNDTRIP_CM;

  // Update display every 1.5s
  unsigned int currentMillis = millis();
  if (currentMillis - lastDisplayTime >= displayInterval) {
    lcd.clear();
    lcd.setCursor(4, 0);  // Col, Row
    lcd.print("F: ");
    lcd.print(distance1);

    lcd.setCursor(0, 1);
    lcd.print("L: ");
    lcd.print(distance2);

    lcd.setCursor(8, 1);
    lcd.print("R: ");
    lcd.print(distance3);

    lastDisplayTime = currentMillis;
  }

  // if (distance1!=0 && distance1< 40) {
  //   Stop();
  //   Reverse();
  // } else if ((distance2!=0 && distance2 < 40) && (distance3!=0 && distance3 < 40)) {
  //   Forward();
  // } else if (distance1==0 || distance1 > 40) {
  //   Forward();
  // } else if (distance2!=0 && distance2 < 40) {
  //   Left();
  // } else if (distance3!=0 && distance3 < 40) {
  //   Right();
  // } else {
  //   Stop();
  //   Reverse();
  // }

  if (distance1 == 0 || distance1 > 40) {
    Forward();
  } else if ((distance1 != 0 && distance1 < 40) && ((distance2 != 0 && distance2 < 40) || (distance3 != 0 && distance3 < 40))) {
    Stop();
    Reverse();
  } else if (distance2 != 0 && distance2 < 40) {
    Left();
  } else if (distance3 != 0 && distance3 < 40) {
    Right();
  } else {
    Stop();
    Reverse();
  }

  delay(50);
}
