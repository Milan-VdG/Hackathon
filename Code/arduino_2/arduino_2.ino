#include "SevSegShift.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#define lightSensPin A2

uint8_t incomingByte = 0;
SevSeg sevseg;
SoftwareSerial connection(A0, A1);

// Create DF player mini connection
static const uint8_t PIN_MP3_TX = A0;  // Connects to module's RX
static const uint8_t PIN_MP3_RX = A1;  // Connects to module's TX
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
DFRobotDFPlayerMini player;
bool isPlaying = false;
bool good = true;
unsigned long startTime = 0;

void setup() {
  byte numDigits = 4;
  byte digitPins[] = { 10, 11, 12, 13 };        
  byte segmentPins[] = { 2, 3, 4, 5, 6, 7, 8 };  
  bool resistorsOnSegments = true;               // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE;          // See README.md for options
  bool updateWithDelays = false;                 // Default 'false' is Recommended
  bool leadingZeros = false;                     // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = true;                   // Use 'true' if your decimal point doesn't exist or isn't connected

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(100);
  Serial.begin(9600);
  pinMode(lightSensPin, INPUT);
  softwareSerial.begin(9600);

  // Start communication with DFPlayer Mini
  if (player.begin(softwareSerial)) {
    // Set volume to maximum (0 to 30).
    player.volume(20);
  }
}

void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    Serial.println(incomingByte);
  }

  if (incomingByte == 1 && !isPlaying) {
    player.play(1);  // This will initiate playback
    isPlaying = true;
    startTime = millis();  // Start the timer
    
  }

  if (isPlaying) {
    unsigned long elapsedTime = millis() - startTime;
    int secondsLeft = 15 - (elapsedTime / 1000);

    if (secondsLeft >= 0) {
      sevseg.setNumber(secondsLeft, 1);  // Display the number of seconds left
    } else {
      secondsLeft = 0;
      Serial.write(0);
      sevseg.setChars("END ");
      isPlaying = false;
      incomingByte = 0;  // Reset for the next round
      good = false;
    }
  } else if (player.available() && player.readType() == DFPlayerPlayFinished) {
    isPlaying = false;
  }


  if (analogRead(lightSensPin) < 600 && !isPlaying && !good) {
    sevseg.setChars("GOOD");
    sevseg.refreshDisplay();
    good = true;
  }



  sevseg.refreshDisplay();  // Must run repeatedly
}
