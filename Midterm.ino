/*
  Notes:
  I utilized (https://www.rapidtables.com/convert/number/binary-to-hex.html)
    to convert binary values to hex values.
  I referenced and pulled code from candle2 with credit to Scott Hudson. 
  I referenced and pulled code from Arduino documentation/built-in examples
    including: Blink, Serial, Debounce, AnalogInOutSerial, AnalogInput.
*/
#define F_CPU 16000000L  // tell code we are 16MHz
#include <avr/delay.h> // import AVR for fine-tune delays

/* Structure to track and debounce the state of a push button switch */
typedef struct switchTracker {
  int lastReading;       // last raw value read
  long lastChangeTime;   // last time the raw value changed
  byte pin;              // the pin this is tracking changes on
  byte switchState;      // debounced state of the switch
} switchTrack;

/* Initializes a switchTrack structure to start tracking/debouncing a switch
   on the given pin.
*/
void initSwitchTrack(struct switchTracker &sw, int swPin) { 
  pinMode(swPin, INPUT); 
  sw.lastReading = digitalRead(swPin);
  sw.lastChangeTime = millis();
  sw.pin = swPin;
  sw.switchState = sw.lastReading;
}

/***GLOBAL VARIABLES***/

// allows easy access to the hex values needed to access a specific row
// (i.e. using rowsB[2] and rowsC[2] will give the pins for 
//    interacting with row 2 of the LED matrix)
long rowsB[8] = {0x10, 0x8, 0x4, 0x2, 0x1, 0, 0, 0}; // for port b
long rowsC[8] = {0, 0, 0, 0, 0, 0x4, 0x2, 0x1}; // for port c

// variables to track switches
switchTrack left; // A4
switchTrack right; // A5
switchTrack up; // A3
switchTrack down; // 13
// for access to all buttons
switchTracker buttons[4] = {up, down, left, right};
int buttonPins[4] = {A3, 13, A4, A5};

// variables to track potentiometer 
int sensorPin = A6;
int sensorValue = 250; // 250 ms as default 

// variables for snake game
int matrix[8][8]; // 1 if snake is there, 0 if not
int snake[64][2]; // an array of snake points [row, col] 
int size = 0; // size of snake
int fruitRow; // row of the next goal
int fruitCol; // column of the next goal
int dir; // snake direction
  // int corresponds to location in buttons[]
  // 0=up, 1=down, 2=left, 3=right

// the setup function runs once when you press reset or power the board
void setup() {
  // sets pins connected to led matrix as output
  DDRD |= B11111111; 
  DDRB |= B011111;
  DDRC |= B000111;
  pinMode(0, OUTPUT); // included because of trouble with the D0 pin

  // initialize all switches
  for (int i=0; i<4; i++) {
    initSwitchTrack(buttons[i], buttonPins[i]);
  }
  startGame(); // initializes game, only call for midterm part 2
}

// runs on loop after initial setup 
void loop() {  
  //midtermPart1(); // runs part 1, scrolling "OK" and a message
  midtermPart2(); // runs part 2, playing the snake game
}

/**HELPER FUNCTIONS**/

/*
Condensed code for just part 2 of the midterm.
Runs the snake game.
No params.
No return.
*/
void midtermPart2() {
  checkButtons();
  drawBoard(); // puts snake and fruit (goal) on led matrix
  checkButtons(); // check after something that takes significant amount of time
  // try to move in the new direction
  int doNext = tryNewHead(); // returns 0 for cont, 1 for hit fruit, 2 for lost
  int newRow = 0; // temp row for new head loc
  int newCol = 0; // temp col for new head loc
  if (doNext == 1) { // case 1: a goal was hit
    size += 1; // add to snake size
    // head is where fruit was
    newRow = fruitRow;
    newCol = fruitCol;
    // get new fruit
    matrix[fruitRow][fruitCol] = 1; // so fruit doesn't appear in same place
    spawnFruit(); 
  }
  if (doNext == 0) { // case 0: snake moves
    // get current head location
    newRow = snake[0][0]; 
    newCol = snake[0][1];
    // update new head location based on direction
    if (dir == 0) newRow -= 1;
    if (dir == 1) newRow += 1;
    if (dir == 2) newCol -= 1;
    if (dir == 3) newCol += 1;
  }
  // move snake in the list up 1
  for (int i = size-1; i > 0; i--) {
    snake[i][0] = snake[i-1][0];
    snake[i][1] = snake[i-1][1];
  }
  // update head
  snake[0][0] = newRow;
  snake[0][1] = newCol;
  updateMatrix(); // matrix needs to match new list of snake posns
  if (doNext == 2) { startGame(); } // case 2: game was lost (restart)
}

/*
Check if any of the buttons have been clicked and update direction accordingly.
Note: this will only update the direction value, not the visual,
  and is a separate function so buttons can be checked more often. 
No params.
No return.
*/
void checkButtons() {
  // check if each of the buttons have been changed (on to off or off to on)
  bool checkUp = switchChange(buttons[0]);
  bool checkDown = switchChange(buttons[1]);
  bool checkLeft = switchChange(buttons[2]);
  bool checkRight = switchChange(buttons[3]);
  // if a button has been pressed and the button was changed on,
  //    update the snake's direction based on which button was pressed
  //    (the snake cannot turn around/turn 180 degrees)
  if (checkUp && buttons[0].switchState == 1 && dir != 1) {
    dir = 0;
  }
  if (checkLeft && buttons[2].switchState == 1 && dir != 3) {
    dir = 2;
  }
  if (checkRight && buttons[3].switchState == 1 && dir != 2) {
    dir = 3;
  } 
  if (checkDown && buttons[1].switchState == 1 && dir != 0) {
    dir = 1;
  } 
}

/*
Sets up the game (including a countdown).
No params.
No returns.
*/
void startGame() {
  countDown(); // visual countdown 

  // initialize snake values
  // chosen to always begin with a length of 2,
  //  start in upper left corner,
  //  moving right
  size = 2; 
  snake[0][0] = 0;
  snake[0][1] = 1;
  snake[1][0] = 0;
  snake[1][1] = 0;
  dir = 3; 

  // initialize visual
  // 8x8 matrix of all 0 except snake in [0,0] and [0,1]
  for (int i=0; i<8; i++) {
    for (int j = 0; j<8; j++) {
      matrix[i][j]=0; } }
  matrix[0][0] = 1;
  matrix[0][1] = 1;
  
  // initialize first fruit
  spawnFruit();
}

/*
Updates 8x8 matrix to match snake list of points.
No params.
No returns.
*/
void updateMatrix() {
  // initialized 8x8 matrix as all 0
  for (int i=0; i<8; i++) { for (int j=0; j<8; j++) { 
    matrix[i][j] = 0;
  }}
  // sets each snake location to be 1 in matrix
  for (int i = 0; i<size; i++) {
    int x = snake[i][0];
    int y = snake[i][1];
    matrix[x][y] = 1;
  }
}

/*
Checks if a goal was met.
@param newRow: int for row to check for fruit.
@param newCol: int for col to check for fruit.
return true if params match location of goal, false otherwise.
*/
bool won(int newRow, int newCol) {
  return ((newRow == fruitRow) && (newCol == fruitCol));
}

/*
Checks if the snake is out of bounds resulting in a game lost.
@param newRow: int for row to check.
@param newCol: int for col to check.
return true if params match an invalid location, false otherwise.  
*/
bool lost(int newRow, int newCol) {
  if (newCol < 0) return true; // hit top
  if (newCol >= 8) return true; // hit bottom
  if (newRow < 0) return true; // hit left
  if (newRow >= 8) return true; // hit right
  if (matrix[newRow][newCol] == 1) return true; // hit self
  return false;
}

/*
Utilizes makeShape(int[]) to turn on LEDs based on matrix values.
No params.
No return.
*/
void drawBoard() {
  long rows[8]; 
  for (int i=0; i<8; i++) {
    long temp = 0; // start at 0
    for (int j=0; j<8; j++) {
      int t = matrix[i][j];
      // turn light on if fruit is there
      if (i==fruitRow && j==fruitCol) t = 1; 
      temp = (temp << 1) | t; // shift next bit into int
    }
    rows[i] = temp;
  }
  makeShape(rows);
}

/*
Sets a random location for the goal.
No params.
No returns.
*/
void spawnFruit() {
  fruitRow = (rand() % 8);
  fruitCol = (rand() % 8);
  // guarentee fruit doesn't appear where snake already is
  while (matrix[fruitRow][fruitCol] == 1) {
    fruitRow = (rand() % 8);
    fruitCol = (rand() % 8);
  }
}

/*
Finds result of moving snake once in current direction.
No params.
return 0 if the game should continue as normal,
  1 if the snake hit a goal, and
  2 if the snake went out of bounds and game is lost.
*/
int tryNewHead() {
  int newRow = snake[0][0];
  int newCol = snake[0][1];
  if (dir == 0) newRow -= 1;
  if (dir == 1) newRow += 1;
  if (dir == 2) newCol -= 1;
  if (dir == 3) newCol += 1;
  if (lost(newRow, newCol)) return 2;
  if (won(newRow, newCol)) return 1;
  return 0;
}

/*
Determines if switch was pressed/unpressed, taking into account "bounce" when button is clicked.
@param sw: reference to switchTracker button to check.
return true if the button was changed for a significant time, false otherwise.
*/
boolean switchChange(struct switchTracker &sw) { 
  // switch must stay stable this long (in msec) to register
  const long debounceTime = 0; // 0 makes it somewhat obsolete but is kept to show code and allow for adjustments in reaction time   
  // default to no change until we find out otherwise
  boolean result = false; 
  // get the current raw reading from the switch
  int reading = digitalRead(sw.pin);
  // if the raw reading has changed (by noise or actual press/release) reset change time to now
  //if (reading != sw.lastReading) sw.lastChangeTime = millis();
  sw.lastReading = reading;
  // if time since the last change is longer than the required dwell
  if ((millis() - sw.lastChangeTime) > debounceTime) {
    // note whether we are changing
    result = (reading != sw.switchState);
    // in any case the value has been stable and so the reported state 
    // should now match the current raw reading
    sw.switchState = reading;
  }
  return result;
}

/*
"Prints" visual 3-2-1 countdown on LED matrix utilixing scroll(long[]).
No params.
No return.
*/
void countDown() {
  long three[8] = {0x7E, 2, 2, 0x3E, 2, 2, 2, 0x7E};
  long two[8] = {0x7E, 2, 2, 0x7E, 0x40, 0x40, 0x40, 0x7E};
  long one[8] = {6, 2, 2, 2, 2, 2, 2, 2};
  scroll(three);
  scroll(two);
  scroll(one);
}

/*
Condensed code for just part 1 of the midterm.
Scrolls messages: "OK," negative of "OK," and "HELLO, WORLD!"
No params.
No return.
*/
void midtermPart1() {
  // initialized all letters used as lists of numbers representing each row in hex
  long O[8] = {0x7E, 0xE7, 0xC3, 0xC3, 0xC3, 0xC3, 0xE7, 0x7E};
  long notO[8] = {~0x7E, ~0xE7, ~0xC3, ~0xC3, ~0xC3, ~0xC3, ~0xE7, ~0x7E};
  long K[8] = {0x63, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0x63, 0x61};
  long notK[8] = {~0x63, ~0x66, ~0x6C, ~0x78, ~0x6C, ~0x66, ~0x63, ~0x61};
  long H[8] = {0xC3, 0xC3, 0xC3, 0xFF, 0xFF, 0xC3, 0xC3, 0xC3};
  long E[8] = {0xFF, 0xFF, 0xC0, 0xFF, 0xFF, 0xC0, 0xFF, 0xFF};
  long L[8] = {0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF};
  long W[8] = {0xC3, 0xC3, 0xC3, 0xDB, 0xDB, 0xFF, 0xFF, 0x66};
  long R[8] = {0x7E, 0x67, 0x63, 0x7E, 0x70, 0x5C, 0x4F, 0x47};
  long D[8] = {0xFC, 0xFE, 0xC7, 0xC3, 0xC3, 0xC7, 0xFE, 0xFC};
  long comma[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x18, 0x38};
  long exclam[8] = {0x3E, 0x3E, 0x1C, 0x1C, 0x08, 0x08, 0x00, 0x08};
  // array of symbols for entire "HELLO, WORLD!" message
  long* helloWorld[12] = {H, E, L, L, O, comma, W, O, R, L, D, exclam};
  
  // call scroll for each of the letters/symbols, one at a time
  scroll(O);
  scroll(K);
  scroll(notO);
  scroll(notK);
  for (int i=0; i<12; i++) {
    scroll(helloWorld[i]);
  }
}

/*
"Prints" (sends to LED matrix as output) a single row of a matrix.
@param d: number for values to use in portd.
@param b: number for values to use in portb.
@param c: number for values to use in portc.
No return.
*/
void oneLine(long d, long b, long c) {
  // turn lights on
  PORTD |= ~d; // set to low because they are cols
  PORTB |= b;
  PORTC |= c;
  _delay_us(100);
  // turn lights off
  PORTD &= d;
  PORTB &= ~b;
  PORTC &= ~c;
  _delay_us(100);
}

/*
"Prints" (sends to LED matrix as output) all values of a matrix 
  using calls to oneLine(long, long, long).
Utilizes input from the potentiometer. 
@param vals: the list of lists of numbers representing which lights 
  to turn on and off in each row for each shape.
No return.
*/
void makeShape(long vals[]) {
  sensorValue = analogRead(sensorPin);
  double start = millis();
  // shape shows for amount of time based on potentiometer input
  while (millis() < start + sensorValue) { 
    for (int i=0; i<8; i++) {
      oneLine(vals[i], rowsB[i], rowsC[i]);
      _delay_us(100);
    }
  }
}

/*
Creates a scrolling effect on an LED matrix utilizing makeShape(long[]). 
@param shape: the list of lists of numbers representing which lights 
  to turn on and off in each row for each shape.
No return.
*/
void scroll(long shape[]) {
  // scroll in
  long temp[8];
  for (int j=8; j>0; j--) {
    // shift each row j times to the right to show first j cols
    for (int i=0; i<8; i++) { temp[i] = (shape[i] >> j); }
    makeShape(temp);
  }
  // scroll out
  for (int j=0; j<8; j++) {
    // shift each row j times to the left to show last j cols
    for (int i=0; i<8; i++) { temp[i] = (shape[i] << j); }
    makeShape(temp);
  }
}



