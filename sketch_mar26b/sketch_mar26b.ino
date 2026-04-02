//From LAB 5 PWM Motor.c.txt
// Pins for all inputs, keep in mind the PWM defines must be on PWM pins
#define MotorPWM_A 4 //left motor
#define MotorPWM_B 5 //right motor
#define BLUETOOTH_BAUD_RATE 38400

// Motor pins
#define MotorPWM_L 4   // left motor PWM
#define MotorPWM_R 5   // right motor PWM
#define INA1A 32
#define INA2A 34
#define INA1B 30
#define INA2B 36

//Lights
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
// Light controls
// ============================
void LightsOff(){
  
}

void RightLights(){

}

void LeftLights(){

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

void Left(int speed) 
{
  analogWrite(MotorPWM_L, speed);
  analogWrite(MotorPWM_R, speed - 20);
  
  // Left motor backward
  digitalWrite(INA1A, LOW);
  digitalWrite(INA2A, HIGH);

  // Right motor forward
  digitalWrite(INA1B, HIGH);
  digitalWrite(INA2B, LOW);
}

void Right(int speed)
{
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

  // Encoder pins
  pinMode(ENCODER_LEFT, INPUT_PULLUP);
  pinMode(ENCODER_RIGHT, INPUT_PULLUP);

  // Lights
  pinMode(LEFTREAR, OUTPUT);
  pinMode(LEFTFRONT, OUTPUT);
  pinMode(RIGHTREAR, OUTPUT);
  pinMode(RIGHTFRONT, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT), ISRMotorLeft, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT), ISRMotorRight, FALLING);

  StopMotors();
  delay(1000);

  Serial.println("PWM,RPM_Left,RPM_Right");
  Serial2.println("PWM,RPM_Left,RPM_Right");
}

// ============================
// Loop
// ============================
void loop() {
  if (Serial2.available()) {
    char cmd = Serial2.read();

    if (cmd == 'F') {
      Forward(128);
    }
    else if (cmd == 'B') {
      Backward(128);
    }
    else if (cmd == 'S') {
      StopMotors();
    }
    else if (cmd == 'L') {
      Left(100);
    }
    else if (cmd == 'R') {
      Right(100);
    }
    else if (cmd == 'Q'){ //these were all calibrated using a dead ish battery
      int speed = 200;
      Forward(speed);
      delay(330);

      Halt(speed);
      delay(500);
      Turn(speed, 100);
      delay(100);
      Accelerate(speed);
      delay(320);

      Halt(speed);
      delay(500);
      Turn(speed, 80);
      delay(100);
      // Accelerate(speed);      
      // delay(330);

      // Halt(speed);
      // delay(500);
      // Turn(speed, 100);
      // delay(100);
      // Accelerate(speed);      
      // delay(320);

      // Halt(speed);
      // delay(500);
      // Turn(speed, 100);
      // delay(100);
      // Accelerate(speed);      
      // delay(340);

      // Halt(speed);
    }
  }
}