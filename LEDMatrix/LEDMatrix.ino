int latchPin = 12; 
int clockPin = 13; 
int dataPin = 11; 
int LEDPin[] = {2, 3, 4, 5, 6, 7, 8, 9}; 
int x = 0;
int y = 0;
int lastMoved = 0;

int lastX = 0;
int lastY = 0;

typedef unsigned long Uint;

int hexBoard[] = {
  0b00000000, 
  0b00000000, 
  0b00000000, 
  0b00000000, 
  0b00000000, 
  0b00000000, 
  0b00000000, 
  0b00000000
};

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  for (int i = 0; i < 8; i++) {
    pinMode(LEDPin[i], OUTPUT);
  }
}

void loop() {
  if(millis() - lastMoved > 50) {
    move();
  } 


  changeBit(x, y, 1);
  changeBit(lastX, lastY, 1);
  
  byte cols = 0x01; // Assign 0x01(binary 00000001) to the variable, which represents thefirst column is selected.
  for (int i = 0; i < 8; i++) { // display 8 column data by scanning
    matrixColsVal(cols); // select this column
    matrixRowsVal(hexBoard[i]);// display the data in this column
    delay(1); // display them for a period of time
    matrixRowsVal(0x00); // clear the data of this column
    cols <<= 1; // shift "cols" 1 bit left to select the next column
  }

  changeBit(x, y, 0);
  changeBit(lastX, lastY, 0);
}

void matrixRowsVal(int value) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, value);
  digitalWrite(latchPin, HIGH);
}

void matrixColsVal(byte value) {
  byte cols = 0x01;
  for (int i = 0; i < 8; i++) {
    digitalWrite(LEDPin[i], ((value & cols) == cols) ? LOW : HIGH);
    cols <<= 1;
  }
}

inline Uint bit_set_to(Uint number, Uint n, bool x) {
  return (number & ~((Uint)1 << n)) | ((Uint)x << n);
}

void changeBit(int x, int y, bool val) {
  hexBoard[y] = bit_set_to(hexBoard[y], x, val);
}

void move() {
  lastMoved = millis();
  lastX = x;
  lastY = y;
  int dx = map(analogRead(0), 0, 1023, 0, 7);
  int dy = map(analogRead(1), 0, 1023, 0, 7);
  int newX = x;
  int newY = y;
  
  if(dx > 3) {
    newX = x + 1;
  } else if(dx < 3) {
    newX = x - 1;
  }

  if(dy > 3) {
    newY = y + 1;
  } else if(dy < 3) {
    newY = y - 1;
  }

  if(newX >= 0 && newX <= 7) {
    x = newX;
  }

  if(newY >= 0 && newY <= 7) {
    y = newY;
  }

}
