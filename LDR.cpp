/**
 * LDR.cpp
 * Class to read out the LDR.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#include "LDR.h"
#include "Config.h"


/**
 * Initializes with the LDR's analog input pin.
 */
LDR::LDR(byte pin)
  : pin(pin),
    smoothedValue(0)
{
  pinMode(pin, INPUT);
}

/**
 * Returns the current LDR value, mapped between 0 and 1023 using LDR_SAMPLE_DARK and LDR_SAMPLE_BRIGHT.
 */
int LDR::read() const {
  return constrain(map(analogRead(pin), LDR_SAMPLE_DARK, LDR_SAMPLE_BRIGHT, 0, 1023), 0, 1023);
}

/**
 * Returns a smoothed LDR value.
 */
int LDR::readSmoothed() {
  smoothedValue += LDR_BRIGHTNESS_GAIN * ((float)read() - smoothedValue);
  return round(smoothedValue);
}
