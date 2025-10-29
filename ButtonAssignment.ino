#define F_CPU 16000000L  // tell code we are 16MHz
#include <avr/delay.h> // import AVR for fine-tune delays

/***GLOBAL VARIABLES***/

// current direction of lights (start left)
bool goRight = false; 

// track what light is on
int lightOn = 0;

void setup() {
  // set LED pins to output
  DDRB |= B00111100;
  pinMode(13, OUTPUT);
  // set button pins to input
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
}

void loop() {
  // check buttons (only need to check for opposite direction)
  if (goRight) {
    if (digitalRead(A5) == HIGH) {
      goRight = false;
    }
  } else {
    if (digitalRead(A4) == HIGH) {
      goRight = true;
    }
  }
  // move the index of which light is on
    // wrap around if on an edge
  if (goRight) {
    lightOn += 1;
    if (lightOn >= 4) lightOn = 0;
  } else {
    lightOn -= 1;
    if (lightOn < 0) lightOn = 3;
  }
  // blink current light
  int time = 250; 
  PORTD |= (1 << (lightOn+2));
  delay(time);
  PORTD &= ~(1 << (lightOn+2));
  delay(time);
}
