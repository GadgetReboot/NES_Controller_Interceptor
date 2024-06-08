/*
  NES Controller Interceptor/Repeater
  Uses Arduino Nano to read NES controller button states and set 8 output pins to those states.
  A 4021 shift register reads in the 8 button states and has its data clocked into the NES.

  This allows a NES controller to operate as usual while allowing the Nano to manipulate the
  button states, for example to add auto-fire when a button is held.

  NES controller pinout:
               _________
             /          |
            /        O  | Ground
           /            |
     +VCC  |  O      O  | Clock
           |            |
     N.C.  |  O      O  | Latch
           |            |
     N.C.  |  O      O  | Data Out (From Controller To NES/reader)
           |____________|

  youtube.com/@GadgetReboot
*/

#include "NES_Controller_Reader.h"

// button data available from the NES controller class
#define BTN_A 0
#define BTN_B 1
#define BTN_SELECT 2
#define BTN_START 3
#define BTN_UP 4
#define BTN_DOWN 5
#define BTN_LEFT 6
#define BTN_RIGHT 7

const int clock = 2;   // NES controller shift register pins used to read buttons out of the controller
const int latch = 3;
const int data = 4;

const int btnA = 5;    // NES button outputs to hold the state of each controller button so the 4021 can shift it into the NES
const int btnB = 6;
const int btnSelect = 7;
const int btnStart = 8;
const int btnUp = 9;
const int btnDown = 10;
const int btnLeft = 11;
const int btnRight = 12;

const int pot1Pin = A0;  // turbo fire control pots for A/B buttons
const int pot2Pin = A1;


Controller controller(latch, clock, data);  // NES controller interface to read buttons in and make data available

void setup() {
  // output pins to indicate the state of each decoded controller button
  pinMode(btnA, OUTPUT);
  pinMode(btnB, OUTPUT);
  pinMode(btnSelect, OUTPUT);
  pinMode(btnStart, OUTPUT);
  pinMode(btnUp, OUTPUT);
  pinMode(btnDown, OUTPUT);
  pinMode(btnLeft, OUTPUT);
  pinMode(btnRight, OUTPUT);
}

void loop() {
  controller.latch();  // read in controller buttons
  setOutputs();        // write button states to output pins to be read by 4021 shift register
}

void setOutputs() {

  const byte turboMinRate = 1000;          // turbo fire timing intervals in mS (button is toggled on or off at these intervals)
  const byte turboMaxRate = 300;
  int pot1Position = analogRead(pot1Pin);  // read turbo fire pot settings
  int pot2Position = analogRead(pot2Pin);
  int turboRate1 = map(pot1Position, 0, 1023, turboMinRate, turboMaxRate);   // define min/max turbo fire rates in mS based on scaled pot readings
  int turboRate2 = map(pot2Position, 0, 1023, turboMinRate, turboMaxRate);

  static unsigned long turboFireTimer1 = millis();   // timers for auto-toggling button presses when in turbo fire mode
  static unsigned long turboFireTimer2 = millis();
  unsigned long turboFireInterval1_mS = turboRate1;
  unsigned long turboFireInterval2_mS = turboRate2;

  static boolean turboStateA = HIGH;  // contains current button state (to be periodically toggled) when turbo firing
  static boolean turboStateB = HIGH;


  // get the state of each controller button and set the output pins for the 4021 to use
  // check if the A and B buttons have turbo fire enabled (pot is set above a min voltage threshold)
  // and handle turbo button press toggling at the rate defined by the pot if a button is being held on the controller

  if (controller.pressed(BTN_A)) {
    if (pot1Position < 50) digitalWrite(btnA, LOW);  // if not in turbo fire mode, just activate the button press
    else {                                           // if in turbo fire mode, toggle the button at the pot rate setting
      if ((millis() - turboFireTimer1) > turboFireInterval1_mS) {
        turboFireTimer1 = millis();
        turboStateA = !turboStateA;
        digitalWrite(btnA, turboStateA);
      }
    }
  }
  else
    digitalWrite(btnA, HIGH); {
  }

  if (controller.pressed(BTN_B)) {
    if (pot2Position < 50) digitalWrite(btnB, LOW);  // if not in turbo fire mode, just activate the button press
    else {                                           // if in turbo fire mode, toggle the button at the pot rate setting
      if ((millis() - turboFireTimer2) > turboFireInterval2_mS) {
        turboFireTimer2 = millis();
        turboStateB = !turboStateB;
        digitalWrite(btnB, turboStateB);
      }
    }
  }
  else
    digitalWrite(btnB, HIGH); {
  }

  if (controller.pressed(BTN_START))
    digitalWrite(btnStart, LOW);
  else
    digitalWrite(btnStart, HIGH);

  if (controller.pressed(BTN_SELECT))
    digitalWrite(btnSelect, LOW);
  else
    digitalWrite(btnSelect, HIGH);

  if (controller.pressed(BTN_UP))
    digitalWrite(btnUp, LOW);
  else
    digitalWrite(btnUp, HIGH);

  if (controller.pressed(BTN_DOWN))
    digitalWrite(btnDown, LOW);
  else
    digitalWrite(btnDown, HIGH);


  if (controller.pressed(BTN_LEFT))
    digitalWrite(btnLeft, LOW);
  else
    digitalWrite(btnLeft, HIGH);


  if (controller.pressed(BTN_RIGHT))
    digitalWrite(btnRight, LOW);
  else
    digitalWrite(btnRight, HIGH);

}
