// Note: I referenced the toneKeyboard built in example. 
// GLOBAL VARIABLES
const int threshold = 10;
// note that the frequencies match notes in "Twinkle Twinkle Little Star"
double freqs[] = {
146.83, 220, 246.96, 220, 
196, 185, 164.81, 146.83, 
220, 196, 185, 164.81, 146.83, 
220, 196, 185, 164.81, 146.83, 
146.83, 220, 246.96, 220, 
196, 185, 164.81, 146.83
};

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // plays each frequency twice
  for (int i = 0; i<26; i++) {
    tone(6, freqs[i], 500);
    delay(1000);
    tone(6, freqs[i], 500);
    delay(1000);
  }
}
