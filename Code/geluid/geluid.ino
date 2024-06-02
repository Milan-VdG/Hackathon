#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// Defineer de pinnen 
#define RX_PIN 10
#define TX_PIN 11
#define SENSOR_PIN 5

// Maak een SoftwareSerial-object aan voor communicatie met de DFPlayer Mini
SoftwareSerial mySoftwareSerial(RX_PIN, TX_PIN);

// Maak een DFPlayerMini-object aan
DFRobotDFPlayerMini myDFPlayer;

// Variabele om de gamestate bij te houden
bool gameWon = false;

void setup() {
  // Initialiseer de seriële communicatie
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  // Initialiseer de DFPlayer Mini
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("Unable to begin:");
    Serial.println("1.Please recheck the connection!");
    Serial.println("2.Please insert the SD card!");
    while (true);
  }

  myDFPlayer.volume(20);

  pinMode(SENSOR_PIN, INPUT);
}

void loop() {
  if (digitalRead(SENSOR_PIN) == HIGH) {
    gameWon = true;
    myDFPlayer.play(1); 
  }

  while (gameWon && myDFPlayer.available()) {
    delay(100);
  }

  gameWon = false;