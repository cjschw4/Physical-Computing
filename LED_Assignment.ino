/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://docs.arduino.cc/hardware/

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://docs.arduino.cc/built-in-examples/basics/Blink/
*/
const int LED_A = 5;
const int LED_B = 4;
const int LED_C = 3;
const int LED_D = 2;
const byte debugPin = 13;        // pin that we put debug output on (set to 255 to disable)

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_A, OUTPUT); 
  pinMode(LED_B, OUTPUT); 
  pinMode(LED_C, OUTPUT); 
  pinMode(LED_D, OUTPUT); 
  pinMode(debugPin, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  // first LED for 2 seconds
  digitalWrite(LED_A,HIGH); 
  delay(2000);
  digitalWrite(LED_A,LOW);

  // second LED for 1 second
  digitalWrite(LED_B,HIGH); 
  delay(1000);
  digitalWrite(LED_B,LOW); 

  // third LED for for 1/2 sec
  digitalWrite(LED_C,HIGH); 
  delay(500);
  digitalWrite(LED_C,LOW);

  // fourth LED for 1/4 sec
  digitalWrite(LED_D,HIGH);
  delay(250);
  digitalWrite(LED_D,LOW);

  // my pattern
  flash_once(LED_B); 
  int out[] = {LED_A, LED_D};
  int in[] = {LED_B, LED_C};
  int odds[] = {LED_A, LED_C};
  int evens[] = {LED_B, LED_D};
  flash_n(out); 
  flash_n(out);   
  flash_n(in);
  flash_n(in);
  flash_n(odds);
  flash_n(odds);
  flash_n(out);
  flash_n(out);
  flash_n(in);
  flash_n(in);
  flash_n(out);
  flash_n(evens);
  flash_n(odds);
  delay(1000);               // wait for a second
}

void flash_once(int LED_X) {
  digitalWrite(LED_X,HIGH);
  delay(250);
  digitalWrite(LED_X,LOW);
  delay(250);
}

void flash_n(int LEDS[]) {
  for (int i = 0; i < sizeof(LEDS); i++) {
    digitalWrite(LEDS[i], HIGH);
  }
  delay(500);
  for (int i = 0; i < sizeof(LEDS); i++) {
    digitalWrite(LEDS[i], LOW);
  }
  delay(500);
}


