//From LAB 5 PWM Motor.c.txt
// Pins for all inputs, keep in mind the PWM defines must be on PWM pins

#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

#include <SoftwareSerial.h>

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define LOGO_WIDTH 8 // OLED display width, in pixels
#define LOGO_HEIGHT 8 // OLED display height, in pixels

#define BLUETOOTH_BAUD_RATE 38400

// Motor pins
#define MotorPWM_L 4   // left motor PWM
#define MotorPWM_R 5   // right motor PWM
#define INA1A 32
#define INA2A 34
#define INA1B 30
#define INA2B 36

//LED pins
#define LEFTREAR 31
#define LEFTFRONT 49
#define RIGHTREAR 37
#define RIGHTFRONT 43

// Encoder pins
#define ENCODER_LEFT  2
#define ENCODER_RIGHT 3

// ============================
// Encoder counters
// ============================
volatile long count_left = 0;
volatile long count_right = 0;

// ============================
// RPM conversion
// For 100 ms sample time:
// RPM = counts * 3.125
// because 60/0.1 / (48*4) = 3.125
// ============================
float rotation = 3.125;
int pwm = 0;

// sample time in ms
const unsigned long sampleTime = 100;

// ============================
// Interrupt service routines
// ============================
void ISRMotorLeft() {
  count_left++;
}

void ISRMotorRight() {
  count_right++;
}
// ============================
// Light control
// ============================
void OFFLights(){ //all lights turn off
  digitalWrite(LEFTREAR, LOW);
  digitalWrite(RIGHTREAR, LOW);
  digitalWrite(LEFTFRONT, LOW);
  digitalWrite(RIGHTFRONT, LOW);  
}

void ONLights(){ //all lights turn on 
  digitalWrite(LEFTREAR, HIGH);
  digitalWrite(RIGHTREAR, HIGH);
  digitalWrite(LEFTFRONT, HIGH);
  digitalWrite(RIGHTFRONT, HIGH);
}

void BREAKLights(){ //rear lights are on
  digitalWrite(LEFTREAR, HIGH);
  digitalWrite(RIGHTREAR, HIGH);
}

void RIGHTLights(){ //right side lights are on
  digitalWrite(RIGHTREAR, HIGH);
  digitalWrite(RIGHTFRONT, HIGH);
}

void LEFTLights(){ //left side lights are on
  digitalWrite(LEFTREAR, HIGH);
  digitalWrite(LEFTFRONT, HIGH);
}

// ============================
// Motor control
// ============================
void Forward(int speed) {
  analogWrite(MotorPWM_L, speed);
  analogWrite(MotorPWM_R, speed);

  // Left motor forward
  digitalWrite(INA1A, HIGH);
  digitalWrite(INA2A, LOW);

  // Right motor forward
  digitalWrite(INA1B, HIGH);
  digitalWrite(INA2B, LOW);
}

void Backward(int speed) {
  analogWrite(MotorPWM_L, speed);
  analogWrite(MotorPWM_R, speed);

  // Left motor backward
  digitalWrite(INA1A, LOW);
  digitalWrite(INA2A, HIGH);

  // Right motor backward
  digitalWrite(INA1B, LOW);
  digitalWrite(INA2B, HIGH);
}

void Left(int speed) //left turn function
{
  OFFLights();
  LEFTLights();
  analogWrite(MotorPWM_L, speed);
  analogWrite(MotorPWM_R, speed - 20);
  
  // Left motor backward
  digitalWrite(INA1A, LOW);
  digitalWrite(INA2A, HIGH);

  // Right motor forward
  digitalWrite(INA1B, HIGH);
  digitalWrite(INA2B, LOW);
}

void Right(int speed) //right turn function
{
  OFFLights();
  RIGHTLights();
  analogWrite(MotorPWM_L, speed - 20);
  analogWrite(MotorPWM_R, speed);
  
  // Left motor forward
  digitalWrite(INA1A, HIGH);
  digitalWrite(INA2A, LOW);

  // Right motor backward
  digitalWrite(INA1B, LOW);
  digitalWrite(INA2B, HIGH);
}

void StopMotors() {
  analogWrite(MotorPWM_L, 0);
  analogWrite(MotorPWM_R, 0);

  digitalWrite(INA1A, LOW);
  digitalWrite(INA2A, LOW);
  digitalWrite(INA1B, LOW);
  digitalWrite(INA2B, LOW);
}

void Halt(int startSpeed)
{
  BREAKLights();
  for (int s = startSpeed; s > 0; s -= 20) {
    Forward(s);
    delay(20);
  }
  StopMotors();
  
}

void Turn(int maxSpeed, int turnTime)
{
  for (int s = 80; s <= maxSpeed; s += 20) {
    Right(s);
    delay(20);
  }
  delay(turnTime);  // actual turning duration
  StopMotors();
}

void Accelerate(int maxSpeed)
{
  OFFLights();
  for (int s = 80; s <= maxSpeed; s += 20) {
    Forward(s);
    delay(20);
  }
}

//Setup function.
void setup() {
  Serial.begin(9600);
  Serial2.begin(BLUETOOTH_BAUD_RATE);

  // Motor pins
  pinMode(MotorPWM_L, OUTPUT);
  pinMode(MotorPWM_R, OUTPUT);
  pinMode(INA1A, OUTPUT);
  pinMode(INA2A, OUTPUT);
  pinMode(INA1B, OUTPUT);
  pinMode(INA2B, OUTPUT);

  //SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;); //Don't proceed, loop forever
  }

  // Encoder pins
  pinMode(ENCODER_LEFT, INPUT_PULLUP);
  pinMode(ENCODER_RIGHT, INPUT_PULLUP);

  // Light Control
  pinMode(LEFTFRONT, OUTPUT);
  pinMode(RIGHTFRONT, OUTPUT);
  pinMode(LEFTREAR, OUTPUT);
  pinMode(RIGHTREAR, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT), ISRMotorLeft, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT), ISRMotorRight, FALLING);

  StopMotors();
  delay(1000);

  display.clearDisplay();
  display.display();
}

// ============================
// Loop
// ============================
void loop() {
  // put your main code here, to run repeatedly:
    if (Serial2.available()) {
    char cmd = Serial2.read();

    if (cmd == 'F') { //forward
      Forward(128);
    }
    else if (cmd == 'B') { //backward
      Backward(128);
    }
    else if (cmd == 'S') { //stop
      StopMotors();
    }
    else if (cmd == 'L') { //left turn
      Left(100);
    }
    else if (cmd == 'R') { //right turn
      Right(100);
    }
    else if (cmd == 'Q'){ //square run code
      int speed = 200;
      Forward(speed);
      delay(360);
      
      Halt(speed); //First turn
      delay(500);
      Turn(speed, 85);
      delay(105);
      Accelerate(speed);
      delay(290); 

      Halt(speed); //Second turn
      delay(500);
      Turn(speed, 95);
      delay(105);
      Accelerate(speed);      
      delay(280);

      Halt(speed); //Third turn
      delay(500);
      Turn(speed, 90);
      delay(105);
      Accelerate(speed);      
      delay(270);

      Halt(speed);
      StopMotors();
    }
  }
}