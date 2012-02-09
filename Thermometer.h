/**
 * Thermometer.h
 * Class for access to the DS1621 Digital Thermometer.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#ifndef Thermometer_h
#define Thermometer_h

#include <Wiring.h>

struct FixedPoint8 {
  char hi; // signed
  byte lo; // unsigned

  FixedPoint8(char hi, byte lo)
    : hi(hi), lo(lo) {};

  float toFloat() {
    return (float) hi + (lo >> 7) * 0.5;
  };
  
  byte toString(char *str) { // str has to be at least 7 bytes long
    byte pos = 0;
    byte absHi;
    if (hi < 0) {
      str[pos++] = '-';
      absHi = -hi;
      if (lo)
        absHi -= 1;
    } else {
      absHi = hi;
    }
    if (absHi > 100)
      str[pos++] = '0' + (absHi / 100);
    if (absHi > 10)
      str[pos++] = '0' + (absHi / 10);
    str[pos++] = '0' + (absHi % 10);
    str[pos++] = '.';
    str[pos++] = (lo ? '5' : '0');
    str[pos++] = '\0';
    return pos;
  }
};

class Thermometer {
  public:
    Thermometer(byte address);

    void startConversion() const;
    void stopConversion() const;
    FixedPoint8 read() const;

  private:
    byte address;
};

#endif
