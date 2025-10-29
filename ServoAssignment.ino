/*
 Note: I referenced the built in Servo Motors example. 
*/
#include <Servo.h>

Servo motor;

const byte debugPin = 13;   
const int potPin = A4;
const int motorPin = 6;

int potVal;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(debugPin, OUTPUT);
  // initialize servo
  motor.attach(motorPin);
  // initialize potentiometer pin as input
  pinMode(potPin, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  // get potentiometer value
  potVal = analogRead(potPin);
  // set servo value based on potVal
  potVal = map(potVal, 0, 1023, 0, 180);
  motor.write(potVal);
  delay(15);
}



