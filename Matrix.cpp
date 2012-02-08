/**
 * Matrix.cpp
 * Class to control the 11x10 LED matrix.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#include "Matrix.h"
#include "Font.h"

#define TEXT_SPACING 0

/**
 * Initializes the matrix using a shift register for output.
 */
Matrix::Matrix(const ShiftRegister &shiftReg, byte pwmPin)
  : shiftReg(shiftReg),
    pwmPin(pwmPin),
    nextRow(0)
{
  clear();
  this->shiftReg.setOutputEnabled(false);
  pinMode(pwmPin, OUTPUT);
  setBrightness(255);
}


/**
 * Clears the matrix turning every single LED off.
 */
void Matrix::clear() {
  for (byte i = 0; i < MATRIX_ROWS; i++) {
    matrix[i] = 0;
  }
}

/**
 * Sets all bits of the specified row.
 */
void Matrix::setRow(byte row, word data) {
  if (row >= 0 && row < MATRIX_ROWS) {
    matrix[row] = data << 5;
  }
}

/**
 * Performs a bitwise OR with the given data on the specified row.
 */
void Matrix::setRowOr(byte row, word data) {
  if (row >= 0 && row < MATRIX_ROWS) {
    matrix[row] |= data << 5;
  }
}

/**
 * Turns on the specified pixel.
 * Origin is the upper left corner of the matrix.
 */
void Matrix::setPixel(byte x, byte y) {
  if (x >= 0 && x < MATRIX_COLS && y >= 0 && y < MATRIX_ROWS) {
    matrix[y] |= (0b1000000000000000 >> x);
  }
}

/**
 * Turns on the specified corner LEDs.
 */
void Matrix::setCorner(byte corner) {
  switch (corner) {
    case MATRIX_CORNER_UPPER_LEFT:  matrix[1] |= (1 << 3); break;
    case MATRIX_CORNER_UPPER_RIGHT: matrix[0] |= (1 << 2); break;
    case MATRIX_CORNER_LOWER_RIGHT: matrix[9] |= (1 << 1); break;
    case MATRIX_CORNER_LOWER_LEFT:  matrix[8] |= (1 << 0); break;
  }
}

/**
 * Sets the given number of corner LEDs.
 */
void Matrix::setCorners(byte count) {
  switch (count) {
    case 4: setCorner(MATRIX_CORNER_LOWER_LEFT);  /* no break! */
    case 3: setCorner(MATRIX_CORNER_LOWER_RIGHT); /* no break! */
    case 2: setCorner(MATRIX_CORNER_UPPER_RIGHT); /* no break! */
    case 1: setCorner(MATRIX_CORNER_UPPER_LEFT);  /* no break! */
  }
}


/**
 * Draws a line from (x0, y0) to (x1, y1) using Bresenham's line algorithm.
 */
void Matrix::drawLine(byte x0, byte y0, byte x1, byte y1) {
  char deltaX = abs(x1 - x0);
  char deltaY = abs(y1 - y0);
  char steepX = (x0 < x1) ? 1 : -1;
  char steepY = (y0 < y1) ? 1 : -1;
  char error = deltaX - deltaY;

  while (true) {
    setPixel(x0, y0);
    if (x0 == x1 && y0 == y1) break;
    char error2 = 2 * error;
    if (error2 > -deltaY) {
      error -= deltaY;
      x0 += steepX;
    }
    if (error2 < deltaX) {
      error += deltaX;
      y0 += steepY;
    }
  }
}


/**
 * Prints a single character.
 */
void Matrix::print(const char c, int x) {
  // Check if the character is visible
  if (x <= -FONT_WIDTH || x >= MATRIX_COLS)
    return;

  // Align on left matrix border
  x -= (MATRIX_COLS - FONT_WIDTH);

  // Draw it to the matrix
  for (byte row = 0; row < FONT_HEIGHT; row++) {
    word data = pgm_read_byte(&font[(byte)c][row]);
    if (x < 0)
      data <<= -x;
    else
      data >>= x;
    setRowOr(row + 2, data);
  }
}

/**
 * Prints the given string to the matrix buffer.
 */
void Matrix::print(const char *str, const int x) {
  size_t len = strlen(str);
  for (size_t i = 0; i < len; i++) {
    print(str[i], x + i * (FONT_WIDTH + TEXT_SPACING));
  }
}

/**
 * 
 */
word Matrix::getTextWidth(const char *str) {
  size_t len = strlen(str);
  return (len * FONT_WIDTH) + ((len - 1) * TEXT_SPACING);
}

/**
 * Sets the matrix brightness between 1 and 255.
 */
void Matrix::setBrightness(const byte brightness) {
  this->brightness = brightness;
  setPWMPrescale(pwmPin, CLOCK_NO_PRESCALE);
  analogWrite(pwmPin, 255 - brightness);
}


/**
 * Writes out the whole matrix to the shift registers.
 */
void Matrix::write() {
  for (byte row = 0; row < MATRIX_ROWS; row++) {
    writeRow(row);
  }
  nextRow = 0;
}

/**
 * Writes out one single row to the shift registers.
 * The next call to this function will write the next row and so on.
 */
void Matrix::writeRow() {
  writeRow(nextRow);
  if (nextRow == MATRIX_ROWS - 1) {
    nextRow = 0;
  } else {
    nextRow++;
  }
}

/**
 * Writes out the given row to the shift registers.
 */
void Matrix::writeRow(const byte row) const {
  if (row < 0 || row >= MATRIX_ROWS)
    return;

  shiftReg.setOutputEnabled(false);

  shiftReg.write(1 << (MATRIX_ROWS - 1 - row));
  shiftReg.write(~matrix[row]);
  shiftReg.store();

  shiftReg.setOutputEnabled(true);
}


/**
 * Returns a String representation of the current matrix contents.
 * Yepp I know, String class and these loops and concatenations in here are evil, but so I am.
 */
String Matrix::toString() {
  String s;
  s.reserve((2 + MATRIX_ROWS) * (MATRIX_COLS + 3));

  // First line with upper corners
  s += (matrix[1] & (1 << 3)) ? 'X' : ' ';
  for (byte col = 0; col < MATRIX_COLS; col++)
    s += '-';
  s += (matrix[0] & (1 << 2)) ? 'X' : ' ';
  s += '\n';

  // Main matrix
  for (byte row = 0; row < MATRIX_ROWS; row++) {
    s += '|';
    for (byte col = 0; col < MATRIX_COLS; col++) {
      s += (matrix[row] & (0b1000000000000000 >> col)) ? 'X' : ' ';
    }
    s += '|';
    s += '\n';
  }

  // Last line with lower corners
  s += (matrix[8] & (1 << 0)) ? 'X' : ' ';
  for (byte col = 0; col < MATRIX_COLS; col++)
    s += '-';
  s += (matrix[9] & (1 << 1)) ? 'X' : ' ';

  return s;
}

