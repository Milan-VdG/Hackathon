const int dataPin = 2;
const int clockPin = 3;
const int latchPin = 4;

const byte digits[] = {
  0b00111111,  // 0
  0b00000110,  // 1
  0b01011011,  // 2
  0b01001111,  // 3
  0b01100110,  // 4
  0b01101101,  // 5
  0b01111101,  // 6
  0b00000111,  // 7
  0b01111111,  // 8
  0b01101111   // 9
};

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
}

void shiftOutByte(byte data) {
  shiftOut(dataPin, clockPin, MSBFIRST, data);
}

void displayNumber(int number) {
  int tens = number / 10;
  int ones = number % 10;

  digitalWrite(latchPin, 0);
  shiftOutByte(digits[ones]);
  shiftOutByte(digits[tens]);
  digitalWrite(latchPin, 1);
}

void loop() {
  for (int i = 15; i >= 0; i--) {
    displayNumber(i);
    delay(1000);
  }
  delay(10000);
}

