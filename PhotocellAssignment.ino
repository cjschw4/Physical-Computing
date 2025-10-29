// GLOBAL VARIABLES
const int LED_A = 5;
const int LED_B = 4;
const int LED_C = 3;
const int LED_D = 2;
const byte debugPin = 13;   
const int pcellPin = A2;
const int LEDs[4] = {LED_A, LED_B, LED_C, LED_D};

int pcellValue;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pins as outputs
  pinMode(LED_A, OUTPUT); 
  pinMode(LED_B, OUTPUT); 
  pinMode(LED_C, OUTPUT); 
  pinMode(LED_D, OUTPUT); 
  pinMode(debugPin, OUTPUT);
  // initialize photocell as input
  pinMode(pcellPin, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  // get photocell value
  pcellValue = analogRead(pcellPin);
  // turn on each LED based on pcellValue 
  if (pcellValue > 10) {
    digitalWrite(LEDs[0], HIGH);
  }
  if (pcellValue > 25) {
    digitalWrite(LEDs[1], HIGH);
  }
  if (pcellValue > 40) {
    digitalWrite(LEDs[2], HIGH);
  }
  if (pcellValue > 55) {
    digitalWrite(LEDs[3], HIGH);
  }
  delay(500);
  // turn all off
  for (int i = 0; i < 4; i++) {
    digitalWrite(LEDs[i], LOW);
  }
}



