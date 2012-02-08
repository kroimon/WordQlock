/**
 * Thermometer.cpp
 * Class for access to the DS1621 Digital Thermometer.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#include "Thermometer.h"
#include <Wire.h>


/**
 * Initialization with the I2C address of the DS1621 Digital Thermometer.
 */
Thermometer::Thermometer(byte address)
  : address(address)
{}

/**
 * Starts temperature conversion.
 */
void Thermometer::startConversion() const {
  Wire.beginTransmission(address);
  Wire.write(0xEE); // Start Convert T
  Wire.endTransmission();
}

/**
 * Halts temperature conversion.
 */
void Thermometer::stopConversion() const {
  Wire.beginTransmission(address);
  Wire.write(0x22); // Stop Convert T
  Wire.endTransmission();
}

/**
 * Reads the last converted temperature value.
 */
FixedPoint8 Thermometer::read() const {
  Wire.beginTransmission(address);
  Wire.write(0xAA); // Read Temperature
  Wire.endTransmission();

  Wire.requestFrom(address, 2u);
  return FixedPoint8(Wire.read(), Wire.read());
}

