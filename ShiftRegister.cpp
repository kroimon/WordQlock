/**
 * ShiftRegister.cpp
 * Class to control 74HC595 serial-in parallel-out shift registers.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#include "ShiftRegister.h"


/**
 * Initialization with the Pins for SER (Serial-Data), SCK (Serial-Clock), RCK (Storage-Clock) and OE (Output Enable)
 */
ShiftRegister::ShiftRegister(byte dataPin, byte clockPin, byte storePin, byte oePin)
#ifndef SHIFTREGISTER_FASTWRITE
  : dataPin (dataPin),
    clockPin(clockPin),
    storePin(storePin),
    oePin   (oePin)
#endif
{
  pinMode(dataPin,  OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(storePin, OUTPUT);
  pinMode(oePin,    OUTPUT);

  #ifdef SHIFTREGISTER_FASTWRITE
    dataReg  = digitalPinToPortReg(dataPin);
    clockReg = digitalPinToPortReg(clockPin);
    storeReg = digitalPinToPortReg(storePin);
    oeReg    = digitalPinToPortReg(oePin);

    dataBit  = digitalPinToBitMask(dataPin);
    clockBit = digitalPinToBitMask(clockPin);
    storeBit = digitalPinToBitMask(storePin);
    oeBit    = digitalPinToBitMask(oePin);
  #endif
}

ShiftRegister::ShiftRegister(const ShiftRegister& rhs)
  #ifdef SHIFTREGISTER_FASTWRITE
  : dataReg (rhs.dataReg),
    clockReg(rhs.clockReg),
    storeReg(rhs.storeReg),
    oeReg   (rhs.oeReg),
    dataBit (rhs.dataBit),
    clockBit(rhs.clockBit),
    storeBit(rhs.storeBit),
    oeBit   (rhs.oeBit)
  #else
  : dataPin (rhs.dataPin),
    clockPin(rhs.clockPin),
    storePin(rhs.storePin),
    oePin   (rhs.oePin)
  #endif
{}


/**
 * Output a WORD (16 bit) LSB first.
 */
void ShiftRegister::write(word data) const {
  #ifdef SHIFTREGISTER_FASTWRITE
    for (word b = 0; b < 16; b++) {
      fastDigitalWriteToClock(LOW);
      if (data & (1 << b)) {
        fastDigitalWriteToData(HIGH);
      } else {
        fastDigitalWriteToData(LOW);
      }
      fastDigitalWriteToClock(HIGH);
    }
  #else
    for (word b = 0; b < 16; b++) {
      digitalWrite(clockPin, LOW);
      if (data & (1 << b)) {
        digitalWrite(dataPin, HIGH);
      } else {
        digitalWrite(dataPin, LOW);
      }
      digitalWrite(clockPin, HIGH);
    }
  #endif
}

/**
 * Finish output by pulsing the storage clock.
 */
void ShiftRegister::store() const {
  #ifdef SHIFTREGISTER_FASTWRITE
    fastDigitalWriteToStore(LOW);
    fastDigitalWriteToStore(HIGH);
  #else
    digitalWrite(storePin, LOW);
    digitalWrite(storePin, HIGH);
  #endif
}

/**
 * Sets the OutputEnable line.
 */
void ShiftRegister::setOutputEnabled(boolean oe) const {
  #ifdef SHIFTREGISTER_FASTWRITE
    fastDigitalWriteToOE(oe ? LOW : HIGH);
  #else
    digitalWrite(oePin, oe ? LOW : HIGH);
  #endif
}


#ifdef SHIFTREGISTER_FASTWRITE

/**
 * Fast digitalWrite() alternatives
 */
void ShiftRegister::fastDigitalWriteToData(boolean state) const {
  uint8_t oldSREG = SREG;
  cli();
  if (state) {
    *dataReg |= dataBit;
  } else {
    *dataReg &= ~dataBit;
  }
  SREG = oldSREG;
}

void ShiftRegister::fastDigitalWriteToClock(boolean state) const {
  uint8_t oldSREG = SREG;
  cli();
  if (state) {
    *clockReg |= clockBit;
  } else {
    *clockReg &= ~clockBit;
  }
  SREG = oldSREG;
}

void ShiftRegister::fastDigitalWriteToStore(boolean state) const {
  uint8_t oldSREG = SREG;
  cli();
  if (state) {
    *storeReg |= storeBit;
  } else {
    *storeReg &= ~storeBit;
  }
  SREG = oldSREG;
}

void ShiftRegister::fastDigitalWriteToOE(boolean state) const {
  uint8_t oldSREG = SREG;
  cli();
  if (state) {
    *oeReg |= oeBit;
  } else {
    *oeReg &= ~oeBit;
  }
  SREG = oldSREG;
}

#endif // SHIFTREGISTER_FASTWRITE

