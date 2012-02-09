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
  : pin(pin)
{
  pinMode(pin, INPUT);
}

/**
 * Returns the raw LDR value.
 */
int LDR::readRaw() {
  return analogRead(pin);
}

/**
 * Returns the current LDR value, mapped to values between 0 and 255.
 */
byte LDR::read() {
  return constrain(map(analogRead(pin), LDR_SAMPLE_DARK, LDR_SAMPLE_BRIGHT, 0, 255), 0, 255);
}
