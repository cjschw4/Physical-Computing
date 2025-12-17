/*
  Poketoy Arduino Code

  Codes the poketoy and collects data. 
  The data is then saved to the computer as a CSV file using FinalPyCode.py.

  Written 17 December 2025
  by Claire Schwartz

  Final project code for CS 584 Physical Computing
  with Dr. Kristin Williams
*/

// Temperature Sensor
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float initTemp = 0;

// Lights
const int redPin = 12;
const int greenPin = 11;
const int bluePin = 10;

// Buttons
const int buttonAPin = A3; int buttonA = 0;
const int buttonBPin = A5; int buttonB = 0;
const int buttonCPin = A6; int buttonC = 0;
const int buttonDPin = A2; int buttonD = 0;

// Pressure Sensors
const int pressAPin = 9; float timeA = 0;
const int pressBPin = 8; float timeB = 0;
int stress = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  // OUTPUTS
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  // INPUTS
  pinMode(buttonAPin, INPUT);
  pinMode(buttonBPin, INPUT);
  pinMode(buttonCPin, INPUT);
  pinMode(buttonDPin, INPUT);
  pinMode(pressAPin, INPUT);
  pinMode(pressBPin, INPUT);
  // Temperature initialize
  //    Wait 5 seconds to ensure accurate reading
  dht.begin();
  delay(5000); 
  initTemp = dht.readTemperature(true);
}

// the loop function runs over and over again forever
void loop() {
  // Read button values
  buttonA = digitalRead(buttonAPin);
  buttonB = digitalRead(buttonBPin);
  buttonC = digitalRead(buttonCPin);
  buttonD = digitalRead(buttonDPin);

  // consistently check pressure sensors 
  bool stress = 0;
  if (digitalRead(pressAPin) && timeA == 0) {
    timeA = millis();
  } else if (!digitalRead(pressAPin) && timeA != 0) {
    // pressure must be held for >= 2 sec to count as stress indicator
    timeA = millis() - timeA;
    if (timeA > 2000) { 
      stress += 1;
    }
    timeA = 0;
  }
  if (digitalRead(pressBPin) && timeB == 0) {
    timeB = millis();
  } else if (!digitalRead(pressBPin) && timeB != 0) {
    // pressure must be held for >= 2 sec to count as stress indicator
    timeB = millis() - timeB;
    if (timeB > 2000) {
      stress += 1;
    }
    timeB = 0;
  }

  // check if a button is pressed
  //    if it has been, call buttonPressed(button) to log next person
  if (buttonA == HIGH) { 
    buttonPressed(0);
    digitalWrite(redPin, HIGH); 
  } 
  if (buttonB == HIGH) { 
    buttonPressed(1);
    digitalWrite(greenPin, HIGH); 
  } 
  if (buttonC == HIGH) { 
    buttonPressed(2);
    digitalWrite(bluePin, HIGH); 
  } 
  if (buttonD == HIGH) { 
    buttonPressed(3); 
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, HIGH);  
  } 
  delay(500); // pause for lights
  // turn lights off
  digitalWrite(redPin, LOW); 
  digitalWrite(greenPin, LOW); 
  digitalWrite(bluePin, LOW); 
}

/*
@param int button: the number corresponding to which button was pressed
no return

This function logs the information for one person.
It is logged with each piece of info separated by a comma,
this way it can later be read into a CSV. 
*/
void buttonPressed(int button) {
  int buttons[] = {"A", "B", "C", "D"};
  // start new row of data
  Serial.print(millis()); // time
  Serial.print(",");
  Serial.print(buttons[button]); // button choice
  Serial.print(",");
  printTemp(); // temperature measure
  Serial.print(",");
  printStress(); // stress measure
  Serial.println(); // new line for new person
}

/*
No params. No return. 

This function prints the user's temperature as:
High (if the measured temp is 2-5 degrees over the room temp)
Very High (if the measured temp is 5+ degrees over the room temp)
or Normal (otherwise)
*/
void printTemp() {
  // TEMP SENSOR
  float temp = dht.readTemperature(true); 
  if (temp > initTemp + 2) { 
    Serial.print("High Temperature"); 
  } else if (temp > initTemp + 5) { 
    Serial.print("Very High Temperature"); 
  } else {
    Serial.print("Normal Temperature");
  }
}

/*
No params. No return.

This function prints the user's stress as:
TRUE if both pressure sensors are high for one user.
FALSE otherwise.

Before ending, the pressure sensor is reset for the next person. 
*/
void printStress() {
// PRESSURE SENSOR
  if (stress == 2) { // indicates both sensors were HIGH
    Serial.print("TRUE");
  } else {
    Serial.print("FALSE");
  }
  stress = 0; // reset for next person
}