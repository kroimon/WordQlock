/**
 * Button.cpp
 * Class to read and debounce push buttons.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#include "Button.h"

#define DEBUG

#define CURRENT            1
#define LAST               2
#define LONGPRESS_HAPPENED 3

/**
 * Initialize with the button's input pin.
 */
Button::Button(byte pin)
  : pin(pin),
    state(0),
    downTime(0),
    upTime(0),
    clickCount(0),
    onClick(NULL),
    onLongPress(NULL)
{
  pinMode(pin, INPUT);
}

/**
 * Returns true every BUTTON_THRESHOLD milliseconds if the button is pressed
 * or if simulatePress() has been called since the last call.
 */
boolean Button::update() {
  unsigned long now = millis();

  bitWrite(state, LAST, bitRead(state, CURRENT));
  boolean currentState = (digitalRead(pin) == HIGH);
  bitWrite(state, CURRENT, currentState);

  if (currentState) {
    // Button pressed
    if (!bitRead(state, LAST) && (now - upTime) > BUTTON_DEBOUNCE_INTERVAL) {
      downTime = now;

      bitWrite(state, LONGPRESS_HAPPENED, 0);

      if ((now - upTime) < BUTTON_DOUBLECLICK_DELAY) {
        clickCount++;
      } else {
        clickCount = 1;
      }
    }

    if ((now - downTime) >= BUTTON_LONGPRESS_DELAY) {
      if (!bitRead(state, LONGPRESS_HAPPENED)) {
        if (onLongPress != NULL)
          onLongPress(*this);
        bitWrite(state, LONGPRESS_HAPPENED, 1);
      }
    }

  } else {
    // Button released
    if (bitRead(state, LAST) && (now - downTime) > BUTTON_DEBOUNCE_INTERVAL) {
      upTime = now;

      if (!bitRead(state, LONGPRESS_HAPPENED)) {
        if (onClick != NULL)
          onClick(*this);
      }
    }
  }
}
