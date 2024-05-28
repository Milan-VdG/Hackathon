// Include the AccelStepper Library
#include <AccelStepper.h>
#include <IRremote.hpp>
#include "/Users/milanvandegeuchte/Downloads/PinDefinitionsAndMore.h"
#include <Arduino.h>

// Define step constant
#define MotorInterfaceType 4
#define ButtonCW 6
#define ButtonCCW 7
#define IRreceive 2

// Creates an instance
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
AccelStepper myStepper(MotorInterfaceType, 8, 10, 9, 11);

void setup() {
  // set the maximum speed, acceleration factor,
  // initial speed and the target position
  myStepper.setMaxSpeed(1000.0);
  myStepper.setAcceleration(1000.0);
  myStepper.setSpeed(-600);
  pinMode(ButtonCW, INPUT_PULLUP);
  pinMode(ButtonCCW, INPUT_PULLUP);

  //IR setup
  // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
  if (digitalRead(ButtonCW) == LOW) {
    myStepper.setSpeed(-600);
    myStepper.runSpeed();
  } else if (digitalRead(ButtonCCW) == LOW) {
    myStepper.setSpeed(600);
    myStepper.runSpeed();
  } else if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.command == 0x15) {
      myStepper.runToNewPosition(100);
      myStepper.setCurrentPosition(0);
      IrReceiver.resume();
    } else if (IrReceiver.decodedIRData.command == 0x46) {
      myStepper.runToNewPosition(-100);
      myStepper.setCurrentPosition(0);
      IrReceiver.resume();
    }} else{
    myStepper.disableOutputs();
  }
  
}