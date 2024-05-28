#define segB2 1;
#define segA2 0;
#define segC2 2;
#define segD2 3;
#define segE2 4;
#define segF2 5;
#define segG2 6;
#define segA1 7;
#define segB1 8;
#define segC1 9;
#define segD1 10;
#define segE1 11;
#define segF1 12;
#define segG1 13;

int segments1[] = {segA1, segB1, segC1, segD1, segE1, segF1, segG1};
int segments2[] = {segA2, segB2, segC2, segD2, segE2, segF2, segG2};

byte digits[16] = {
  0b1111101, // 0
  0b0110000, // 1
  0b1101110, // 2
  0b1111010, // 3
  0b0110011, // 4
  0b1011011, // 5
  0b1011111, // 6
  0b1110000, // 7
  0b1111111, // 8
  0b1111011, // 9
};

void setup(){
    for (int i = 0; i < 7; i++) {
    pinMode(segments1[i], OUTPUT);
    pinMode(segments2[i], OUTPUT);
  }
}

void loop(){
    for (int i = 15; i >= 0; i--) {
    int tens = i / 10;
    int units = i % 10;
    displayDigit(tens, units);
    delay(1000);
  }
  delay(10000);
}

void displayDigit(int tens, int units) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segments1[i], (digits[tens] >> (6 - i)) & 0x01);
  }
  
  for (int i = 0; i < 7; i++) {
    digitalWrite(segments2[i], (digits[units] >> (6 - i)) & 0x01);
  }
}
