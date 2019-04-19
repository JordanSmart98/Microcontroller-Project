#include <Wire.h>
#include <Servo.h>
#define switchPin 3
#define intPin 4

// Variable Setup // 
int mode = 0;
int azRead;
int elRead;
volatile bool exitLoop = false;
int routineDelay = 0;

// Servo Setup //
const int azimuthServoPin = 9, elevationServoPin = 10;
Servo azimuthServo;
Servo elevationServo;

// Joystick Setup //
const int joyX = A0, joyY = A1;

// This function sets up the servos //
void setupServos(){
  azimuthServo.attach(azimuthServoPin);
  azimuthServo.write(90);
  elevationServo.attach(elevationServoPin);
  elevationServo.write(90);
  delay(1000);
}

// This function controlls the Joystick mode of the system
void joystickRoutine(){
  // Setup
  Serial.println("joystick");
  int currentXPosition = azimuthServo.read();
  int currentYPosition = elevationServo.read();
  // Loop
  while(exitLoop == false){
    int rateX = map(analogRead(joyX), 0,1023, -4, 5);
    int rateY = map(analogRead(joyY), 0,1023, -4, 5);
    // If Servo's currentposition + the rate demand is in operating range, allow the demand.
    if(currentXPosition+rateX>0 and currentXPosition+rateX<181){
      currentXPosition+=rateX;
      azimuthServo.write(currentXPosition);
    }
    // Else do nothing
    else{
      Serial.println("OUT OF  BOUNDS X"); 
    }
    // If Servo's currentposition + the rate demand is in operating range, allow the demand.
    if(currentYPosition+rateY>0 and currentYPosition+rateY<181){
      currentYPosition+=rateY;
      elevationServo.write(currentYPosition);   
    }
    // Else do nothing
    else{
      Serial.println("OUT OF  BOUNDS Y");
    }
    // For debug / operation monitoring
    Serial.println(currentXPosition);
    Serial.println(currentYPosition);
    delay(50);
    delay(routineDelay);
    }
}

// This function controls the headset mode of the system
void gyroRoutine(){
  // Setup
  Serial.println("gyro");
  volatile bool l = false;
  azimuthServo.write(90);
  elevationServo.write(90);
  delay(2000);
  // Loop
  while(exitLoop == false){
    // When 2 bytes or more are in the serial buffer
    int i = 0;
    if(Serial.available() >=  2){
      // Break the loop every 100 iterations to check for exitLoop condition
      i++;
      if(i==100){
      break;
      }
      // Outputs
      Serial.print("El: ");
      elRead = Serial.read();
      Serial.println(elRead);
      elevationServo.write(elRead);
      Serial.print("Az: ");
      azRead = Serial.read();
      Serial.println(azRead);
      azimuth Servo.write(azRead);
    }
  }
}

// This is my interrupt function. Sets mode then sets exitLoop to true to stop Joystick or Gyro while loops.
void loopChange(){
  // Check what way switch is set and set mode accordingly.
  mode = digitalRead(switchPin);
  exitLoop = true;
}

void setup() {
  Serial.begin(38400);
  pinMode(switchPin, INPUT);
  pinMode(intPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(switchPin), loopChange, CHANGE);
  mode = digitalRead(switchPin);
  setupServos();
}

void loop() {
  exitLoop = false;
  if(mode == 0){
    exitLoop = false;
    joystickRoutine();
  }
  if(mode == 1){
    exitLoop = false;
    gyroRoutine();
  }
}
