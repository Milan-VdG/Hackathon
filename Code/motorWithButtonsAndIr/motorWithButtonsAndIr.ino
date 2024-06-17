// Include the AccelStepper Library
#include <AccelStepper.h>
#include <IRremote.hpp>
#include "/Users/milanvandegeuchte/Downloads/PinDefinitionsAndMore.h"
#include <Arduino.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// Define step constant
#define MotorInterfaceType 4
#define ButtonCW 6
#define ButtonLR 7
#define IRreceive A4
#define ButtonStart 3

// Creates an instance
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
AccelStepper myStepper(MotorInterfaceType, 8, 10, 9, 11);
AccelStepper lrStepper(MotorInterfaceType, 13, 12, 5, 4);
AccelStepper pushStepper(MotorInterfaceType, A0, A2, A1, A3);

bool running = false;
long initialPositionZ;
long initialPositionY;



void setup() {
  // set the maximum speed, acceleration factor,
  // initial speed and the target position
  myStepper.setMaxSpeed(300.0);
  myStepper.setAcceleration(100.0);
  myStepper.setSpeed(250);
  lrStepper.setMaxSpeed(400.0);
  lrStepper.setAcceleration(200.0);
  lrStepper.setSpeed(-300);
  pushStepper.setMaxSpeed(600.0);
  pushStepper.setAcceleration(150.0);
  pushStepper.setSpeed(-100);
  pinMode(ButtonCW, INPUT);
  pinMode(ButtonLR, INPUT);
  pinMode(ButtonStart, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  Serial.begin(9600);
  initialPositionZ = myStepper.currentPosition();
  initialPositionY = lrStepper.currentPosition();

  //IR setup
  //Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
  IrReceiver.begin(IRreceive, DISABLE_LED_FEEDBACK);
}

void loop() {
  // check for start button press, give 1 to arduino 2
  if (running == false && digitalRead(ButtonStart) == LOW) {
    running = true;
    Serial.println(1);
    Serial.write(1);
  }
  // move motors only when start is presssed
  if (running == true) {
    if (digitalRead(ButtonCW) == HIGH) {
      myStepper.setSpeed(-300);
      myStepper.runSpeed();

    } else if (digitalRead(ButtonLR) == HIGH) {
      lrStepper.setSpeed(400);
      lrStepper.runSpeed();

    } else {
      lrStepper.disableOutputs();
    }
  }
  if (digitalRead(A5) == LOW) {
    myStepper.setSpeed(200);
    myStepper.runSpeed();
  }
  // arduino 2 gives back 0 when 15 seconds have elapsed, push, move back to start position and disable outputs
  if (Serial.available() > 0) {
    if (Serial.read() == 0) {
      running = false;
      pushStepper.moveTo(pushStepper.currentPosition() - 1700);  // Assuming 200 steps for one rotation
      while (pushStepper.isRunning()) {
        pushStepper.run();
      }
      pushStepper.moveTo(pushStepper.currentPosition() + 1700);  // Assuming 200 steps for one rotation
      while (pushStepper.isRunning()) {
        pushStepper.run();
      }
      myStepper.moveTo(initialPositionZ);
      lrStepper.moveTo(initialPositionY);
      while (myStepper.currentPosition() != initialPositionZ) {
        myStepper.run();
      }
      while (lrStepper.currentPosition() != initialPositionY) {
        lrStepper.run();
      }
      myStepper.disableOutputs();
      lrStepper.disableOutputs();
      pushStepper.disableOutputs();
    }
  }
  // when not in game, move motors with IRremote
  if (IrReceiver.decode()) {
    if (running == false) {
      if (IrReceiver.decodedIRData.command == 0x15) {
        myStepper.setSpeed(-300);
        for (int i = 0; i < 20000; i++) {
          myStepper.runSpeed();
        }
      } else if (IrReceiver.decodedIRData.command == 0x46) {
        myStepper.setSpeed(300);
        for (int i = 0; i < 20000; i++) {
          myStepper.runSpeed();
        }
      } else if (IrReceiver.decodedIRData.command == 0x44) {
        lrStepper.setSpeed(-300);
        for (int i = 0; i < 30000; i++) {
          lrStepper.runSpeed();
        }
      } else if (IrReceiver.decodedIRData.command == 0x43) {
        lrStepper.setSpeed(300);
        for (int i = 0; i < 20000; i++) {
          lrStepper.runSpeed();
        }
      } else if (IrReceiver.decodedIRData.command == 0x16) {
        pushStepper.setSpeed(300);
        for (int i = 0; i < 20000; i++) {
          pushStepper.runSpeed();
        }
      } else if (IrReceiver.decodedIRData.command == 0x19) {
        pushStepper.setSpeed(-300);
        for (int i = 0; i < 20000; i++) {
          pushStepper.runSpeed();
        }
      } else if (IrReceiver.decodedIRData.command == 0x42) {
        myStepper.disableOutputs();
        lrStepper.disableOutputs();
        pushStepper.disableOutputs();
        initialPositionZ = myStepper.currentPosition();
        initialPositionY = lrStepper.currentPosition();
      }
    }
    IrReceiver.resume();  // Receive the next value
  }
}
