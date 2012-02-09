/**
 * Matrix.h
 * Class to control the 11x10 LED matrix.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#ifndef Matrix_h
#define Matrix_h

#include <Wiring.h>
#include "ShiftRegister.h"

#define MATRIX_ROWS 10
#define MATRIX_COLS 11

#define MATRIX_CORNER_UPPER_LEFT  0
#define MATRIX_CORNER_UPPER_RIGHT 1
#define MATRIX_CORNER_LOWER_RIGHT 2
#define MATRIX_CORNER_LOWER_LEFT  3

class Matrix {
  public:
    Matrix(const ShiftRegister &shiftReg, byte pwmPin);

    void clear();
    void setRow  (byte row, word data);
    void setRowOr(byte row, word data);
    void setPixel(byte x, byte y);
    void setCorner(byte corner);
    void setCorners(byte count);

    void drawLine(byte x0, byte y0, byte x1, byte y1);

    void print(const char c, const int x = 0);
    void print(const char *str, const int x = 0);
    word getTextWidth(const char *str);

    byte getBrightness() const;
    void setBrightness(const byte brightness);

    void write();
    void writeRow();
    void writeRow(const byte row) const;

    String toString();

  private:
    ShiftRegister shiftReg;
    byte pwmPin;
    
    volatile byte nextRow;
    byte brightness;

    word matrix[MATRIX_ROWS];
};


/**
 * Returns the current matrix brightness between 1 and 255.
 */
inline byte Matrix::getBrightness() const {
  return brightness;
}

#endif
