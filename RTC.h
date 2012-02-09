/**
 * RTC.h
 * Class for access to the DS1340 Real Time Clock.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#ifndef RTC_h
#define RTC_h

#include <Wiring.h>
#include "TimeStamp.h"

class RTC {
  public:
    RTC(byte address);

    TimeStamp read() const;
    void write(const TimeStamp &t) const;

  private:
    byte address;

    byte decToBcd(const byte val) const;
    byte bcdToDec(const byte val) const;
};

#endif
