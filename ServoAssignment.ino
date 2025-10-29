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
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(debugPin, OUTPUT);
  pinMode(potPin, INPUT);
  motor.attach(motorPin);
}

// the loop function runs over and over again forever
void loop() {
  potVal = analogRead(potPin);
  potVal = map(potVal, 0, 1023, 0, 180);
  motor.write(potVal);
  delay(15);
}


