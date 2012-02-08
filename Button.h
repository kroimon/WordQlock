/**
 * Button.h
 * Class to read and debounce push buttons.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#ifndef Button_H
#define Button_H

#include <Wiring.h>

#define BUTTON_DEBOUNCE_INTERVAL 20
#define BUTTON_DOUBLECLICK_DELAY 250
#define BUTTON_LONGPRESS_DELAY 1000

class Button {
  public:
    Button(byte pin);
    boolean update();
    byte getClickCount() { return clickCount; }

    void (*onClick)(Button&);
    void (*onLongPress)(Button&);

  private:
    byte pin;

    // Saves the current and last state and if a longpress event happened
    boolean state;

    // Times of the last button down and up events
    unsigned long downTime;
    unsigned long upTime;

    // Number of consecutive clicks with a maximum delay of BUTTON_DOUBLECLICK_DELAY (e.g. 2 for a doubleclick)
    byte clickCount;
};

#endif
