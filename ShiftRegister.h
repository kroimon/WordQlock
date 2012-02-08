/**
 * ShiftRegister.h
 * Class to control 74HC595 serial-in parallel-out shift registers.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#ifndef ShiftRegister_h
#define ShiftRegister_h

#define SHIFTREGISTER_FASTWRITE

#include <Wiring.h>

class ShiftRegister {
  public:
    ShiftRegister(byte dataPin, byte clockPin, byte storePin, byte oePin);
    ShiftRegister(const ShiftRegister& rhs);

    void write(word data) const;
    void store() const;

    void setOutputEnabled(boolean oe) const;

  private:
    #ifdef SHIFTREGISTER_FASTWRITE
      volatile uint8_t *dataReg;
      volatile uint8_t *clockReg;
      volatile uint8_t *storeReg;
      volatile uint8_t *oeReg;

      uint8_t dataBit;
      uint8_t clockBit;
      uint8_t storeBit;
      uint8_t oeBit;

      void fastDigitalWriteToData(boolean state) const;
      void fastDigitalWriteToClock(boolean state) const;
      void fastDigitalWriteToStore(boolean state) const;
      void fastDigitalWriteToOE(boolean state) const;
    #else
      byte dataPin;
      byte clockPin;
      byte storePin;
      byte oePin;
    #endif
};

#endif

