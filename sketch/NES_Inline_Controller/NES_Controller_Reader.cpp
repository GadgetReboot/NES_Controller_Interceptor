#include "NES_Controller_Reader.h"
#include <Arduino.h>
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

#define BTN_A 0
#define BTN_B 1
#define BTN_SELECT 2
#define BTN_START 3
#define BTN_UP 4
#define BTN_DOWN 5
#define BTN_LEFT 6
#define BTN_RIGHT 7

Controller::Controller(int latch, int pulse, int data) {
  pinMode(latch, OUTPUT);
  pinMode(pulse, OUTPUT);
  pinMode(data, INPUT);

  digitalWrite(latch, LOW);

  _latch = latch;
  _pulse = pulse;
  _data = data;
  _latched = false;
}

void Controller::latch() {
  digitalWrite(_latch, HIGH);
  delayMicroseconds(12);
  digitalWrite(_latch, LOW);
  
  _latched = true;
  
  _a = false;
  _b = false;
  _up = false;
  _down = false;
  _left = false;
  _right = false;
  _start = false;
  _select = false;
  
  for(int i = 0; i < 8; i++) {
    readState(i);
    digitalWrite(_pulse, HIGH);
    delayMicroseconds(6);
    digitalWrite(_pulse, LOW);
    delayMicroseconds(6);
  }
  
  _latched = false;
}

void Controller::readState(int pos) {
  if(digitalRead(_data) == LOW) {
    if(pos == BTN_A) {
      _a = true;
    }else if(pos == BTN_B) {
      _b = true;
    }else if(pos == BTN_SELECT) {
      _select = true;
    }else if(pos == BTN_START) {
      _start = true;
    }else if(pos == BTN_UP) {
      _up = true;
    }else if(pos == BTN_DOWN) {
      _down = true;
    }else if(pos == BTN_LEFT) {
      _left = true;
    }else if(pos = BTN_RIGHT) {
      _right = true;
    }
  }
}

bool Controller::isLatched() {
  return _latched;
}

bool Controller::pressed(int button) {
  if(button == BTN_A)
    return _a;
  if(button == BTN_B)
    return _b;
  if(button == BTN_SELECT)
    return _select;
  if(button == BTN_START)
    return _start;
  if(button == BTN_UP)
    return _up;
  if(button == BTN_DOWN)
    return _down;
  if(button == BTN_LEFT)
    return _left;
  if(button == BTN_RIGHT)
    return _right;
}
