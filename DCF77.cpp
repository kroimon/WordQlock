/**
 * DCF77.cpp
 * Class to parse and validate DCF77 signals.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#include "DCF77.h"

//#define DCF77_DEBUG
#define DCF77_THRESHOLD_SIGNAL   700
#define DCF77_THRESHOLD_100MS     70
#define DCF77_THRESHOLD_200MS    170
#define DCF77_THRESHOLD_GAP     1700


/**
 * Initialize the DCF77 decoder with the signal and power-on (active low) pin.
 */
DCF77::DCF77(byte signalPin, byte powerOnPin)
  : signalPin(signalPin),
    powerOnPin(powerOnPin),
    dataPointer(0),
    lastSignal(LOW),
    signalStartMillis(0)
{
  for (byte i = 0; i < DCF77_DATALENGTH; i++) {
    data[i] = false;
  }

  pinMode(signalPin, INPUT);
  pinMode(powerOnPin, OUTPUT);
  digitalWrite(powerOnPin, LOW);
}

/**
 * Polls the DCF77 signal.
 * Returns true if a datagram has been received completely and a new timestamp can be fetched via getTime().
 */
boolean DCF77::poll() {
  boolean result = false;
  boolean signal = (digitalRead(signalPin) == HIGH);

  if (signal) {
    if (!lastSignal) {
      // rising edge - signal start

      unsigned long now = millis();
      unsigned long lastSignalStart = now - signalStartMillis;

      if (lastSignalStart > DCF77_THRESHOLD_SIGNAL) {
        signalStartMillis = now;
        #ifdef DCF77_DEBUG
          if (dataPointer < DCF77_DATALENGTH) {
            Serial.print(data[dataPointer]);
          }
        #endif
        dataPointer++;
      }

      if (lastSignalStart > DCF77_THRESHOLD_GAP) {
        // gap detected
        #ifdef DCF77_DEBUG
          Serial.println("!");
        #endif

        if ((dataPointer == DCF77_DATALENGTH) && isDataValid()) {
          TimeStamp timeStamp = decodeData();
          if (isConsecutiveTimeStamp(lastTimeStamp, timeStamp)) {
            validTimeStamp = timeStamp;
            result = true;
          }
          lastTimeStamp = timeStamp;
        }
        dataPointer = 0;
      }

    }
  } else {
    if (lastSignal) {
      // falling edge - signal end

      unsigned long signalDuration = millis() - signalStartMillis;

      if (signalDuration > DCF77_THRESHOLD_200MS) {
        // received bit: 1
        if (dataPointer < DCF77_DATALENGTH) {
          data[dataPointer] = true;
        }
      }
      else if (signalDuration > DCF77_THRESHOLD_100MS) {
        // received bit: 0
        if (dataPointer < DCF77_DATALENGTH) {
          data[dataPointer] = false;
        }
      }
    }
  }

  lastSignal = signal;
  return result;
}

/**
 * Checks if the data array contains a valid datagram by
 * asserting fixed bits and validating even parity bits.
 */
boolean DCF77::isDataValid() {
  // Bit 0 is always 0; Bit 20 is always 1
  if (data[0] || !data[20]) {
    #ifdef DCF77_DEBUG
      Serial.println(Constant("DCF77::isDataValid(): Failed. Fixed bits wrong!"));
    #endif
    return false;
  }

  // P1: minutes
  if ((data[21] ^ data[22] ^ data[23] ^ data[24] ^ data[25] ^ data[26] ^ data[27]) != data[28]) {
    #ifdef DCF77_DEBUG
      Serial.println(Constant("DCF77::isDataValid(): Failed. Invalid minute parity (P1)."));
    #endif
    return false;
  }

  // P2: hours
  if ((data[29] ^ data[30] ^ data[31] ^ data[32] ^ data[33] ^ data[34]) != data[35]) {
    #ifdef DCF77_DEBUG
      Serial.println(Constant("DCF77::isDataValid(): Failed. Invalid hour parity (P2)."));
    #endif
    return false;
  }

  // P3: date
  if ((data[36] ^ data[37] ^ data[38] ^ data[39] ^ data[40] ^ data[41] ^ data[42] ^ data[43] ^ data[44] ^ data[45]
     ^ data[46] ^ data[47] ^ data[48] ^ data[49] ^ data[50] ^ data[51] ^ data[52] ^ data[53] ^ data[54] ^ data[55]
     ^ data[56] ^ data[57]) != data[58]) {
    #ifdef DCF77_DEBUG
      Serial.println(Constant("DCF77::isDataValid(): Failed. Invalid date parity (P3) wrong!"));
    #endif
    return false;
  }

  #ifdef DCF77_DEBUG
    Serial.println(Constant("DCF77::isDataValid(): OK."));
  #endif
  return true;
}

/**
 * Decodes the current datagram into a TimeStamp object.
 */
TimeStamp DCF77::decodeData() {
  TimeStamp t;

  t.setSeconds(0);
  t.setMinutes(
      data[21] * 1
    + data[22] * 2
    + data[23] * 4
    + data[24] * 8
    + data[25] * 10
    + data[26] * 20
    + data[27] * 40);
  t.setHours(
      data[29] * 1
    + data[30] * 2
    + data[31] * 4
    + data[32] * 8
    + data[33] * 10
    + data[34] * 20);

  t.setDay(
      data[36] * 1
    + data[37] * 2
    + data[38] * 4
    + data[39] * 8
    + data[40] * 10
    + data[41] * 20);
  t.setDayOfWeek(
      data[42] * 1
    + data[43] * 2
    + data[44] * 4);
  t.setMonth(
      data[45] * 1
    + data[46] * 2
    + data[47] * 4
    + data[48] * 8
    + data[49] * 10);
  t.setYear(
      data[50] * 1
    + data[51] * 2
    + data[52] * 4
    + data[53] * 8
    + data[54] * 10
    + data[55] * 20
    + data[56] * 40
    + data[57] * 80);

  return t;
}

/**
 * Checks if newTimeStamp is a maximum of 5 minutes after oldTimeStamp.
 */
boolean DCF77::isConsecutiveTimeStamp(TimeStamp &oldTimeStamp, TimeStamp &newTimeStamp) {
  TimeStamp movingTimeStamp = oldTimeStamp;
  for (byte i = 0; i < 5; i++) {
    movingTimeStamp.incMinutes(false);
    if (movingTimeStamp.equals(newTimeStamp)) {
      #ifdef DCF77_DEBUG
        Serial.print(Constant("DCF77::isConsecutiveTimeStamp(): OK. Difference is "));
        Serial.print(i + 1);
        Serial.println(Constant(" minute(s)."));
      #endif
      return true;
    }
  }
  #ifdef DCF77_DEBUG
    Serial.print(Constant("DCF77::isConsecutiveTimeStamp(): Failed. [oldTimeStamp="));
    Serial.print(oldTimeStamp.toString());
    Serial.print(Constant(", newTimeStamp="));
    Serial.print(newTimeStamp.toString());
    Serial.println(']');
  #endif
  return false;
}

/**
 * Returns the last decoded timestamp that passed the validity checks.
 */
TimeStamp DCF77::getTime() {
  return validTimeStamp;
}

