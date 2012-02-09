/**
 * LDR.h
 * Class to read out the LDR.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#ifndef LDR_h
#define LDR_h

#include <Wiring.h>

class LDR {
  public:
    LDR(byte pin);

    int readRaw();
    byte read();

  private:
    byte pin;
};

#endif
