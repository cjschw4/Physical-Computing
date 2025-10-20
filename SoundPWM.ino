// Note: I referenced the toneKeyboard built in example. 
const int threshold = 10;
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
  // put your main code here, to run repeatedly:
  for (int i = 0; i<26; i++) {
    tone(6, freqs[i], 500);
    delay(1000);
    tone(6, freqs[i], 500);
    delay(1000);
  }
}
