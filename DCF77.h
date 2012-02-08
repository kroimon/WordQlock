/**
 * DCF77.h
 * Class to parse and validate DCF77 signals.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#ifndef DCF77_h
#define DCF77_h

#include <Wiring.h>
#include "TimeStamp.h"

#define DCF77_DATALENGTH 59

class DCF77 {
  public:
    DCF77(byte signalPin, byte powerOnPin);

    boolean poll();
    TimeStamp getTime();

  private:
    byte signalPin;
    byte powerOnPin;

    boolean data[DCF77_DATALENGTH];
    byte    dataPointer;
    boolean lastSignal;
    unsigned long signalStartMillis;

    TimeStamp lastTimeStamp;
    TimeStamp validTimeStamp;

    boolean isDataValid();
    TimeStamp decodeData();
    boolean isConsecutiveTimeStamp(TimeStamp &oldTimeStamp, TimeStamp &newTimeStamp);
};

#endif

