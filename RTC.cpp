/**
 * RTC.cpp
 * Class for access to the DS1340 Real Time Clock.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#include "RTC.h"
#include <Wire.h>


/**
 * Initialization with the I2C address of the DS1340 Real Time Clock.
 */
RTC::RTC(byte address)
  : address(address)
{}
  
/**
 * Read the current time and return it.
 */
TimeStamp RTC::read() const {
  Wire.beginTransmission(address);
  Wire.write((byte)0x00); // register address to start reading from
  Wire.endTransmission();

  TimeStamp t;
  Wire.requestFrom(address, 7u);
  // A few of these need masks because certain bits are control bits
  t.setSeconds(bcdToDec(Wire.read() & 0x7f));
  t.setMinutes(bcdToDec(Wire.read() & 0x7f));
  t.setHours  (bcdToDec(Wire.read() & 0x3f));

  t.setDayOfWeek(bcdToDec(Wire.read() & 0x07));
  t.setDay  (bcdToDec(Wire.read() & 0x3f));
  t.setMonth(bcdToDec(Wire.read() & 0x1f));
  t.setYear (bcdToDec(Wire.read()));

  return t;
}

/**
 * Write the time from the given TimeStamp object to the DS1340.
 */
void RTC::write(const TimeStamp &t) const {
  Wire.beginTransmission(address);
  Wire.write((byte)0x00); // register address to start writing to
  Wire.write(decToBcd(t.getSeconds()));
  Wire.write(decToBcd(t.getMinutes()));
  Wire.write(decToBcd(t.getHours()));
  Wire.write(decToBcd(t.getDayOfWeek()));
  Wire.write(decToBcd(t.getDay()));
  Wire.write(decToBcd(t.getMonth()));
  Wire.write(decToBcd(t.getYear()));
  Wire.endTransmission();
}

/**
 * Convert Decimal to "Binary Coded Decimal"
 */
byte RTC::decToBcd(const byte val) const {
  return ((val / 10 * 16) + (val % 10));
}

/**
 * Convert "Binary Coded Decimal" to Decimal
 */
byte RTC::bcdToDec(const byte val) const {
  return ((val / 16 * 10) + (val % 16));
}
