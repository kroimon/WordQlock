/**
 * PluginQlock.cpp
 * Plugin to display static patterns/images.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#include "PluginStatic.h"
#include "Global.h"

#define PATTERN_COUNT 5

/**
 *
 */
PluginStatic::PluginStatic()
  : Plugin(5000, false), // 5 second update interval, no realtime sync
    pattern(0)
{}

/**
 * 
 */
void PluginStatic::update(unsigned long timeDiff, boolean realtimeSync) {
  static const word patterns[][MATRIX_ROWS] PROGMEM = {
    { // Test pattern 1
      0b10101010101,
      0b01010101010,
      0b10101010101,
      0b01010101010,
      0b10101010101,
      0b01010101010,
      0b10101010101,
      0b01010101010,
      0b10101010101,
      0b01010101010
    }, { // Test pattern 2
      0b01010101010,
      0b10101010101,
      0b01010101010,
      0b10101010101,
      0b01010101010,
      0b10101010101,
      0b01010101010,
      0b10101010101,
      0b01010101010,
      0b10101010101
    }, { // Heart shape
      0b00000000000,
      0b00110001100,
      0b01001010010,
      0b01000100010,
      0b01000000010,
      0b00100000100,
      0b00010001000,
      0b00001010000,
      0b00000100000,
      0b00000000000
    }
  };

  matrix.clear();
  switch (pattern) {
    case 0:
      for (byte row = 0; row < MATRIX_ROWS; row++) {
        matrix.setRow(row, pgm_read_word(&patterns[0][row]));
      }
      matrix.setCorner(MATRIX_CORNER_UPPER_LEFT);
      matrix.setCorner(MATRIX_CORNER_UPPER_RIGHT);
      break;
    case 1:
      for (byte row = 0; row < MATRIX_ROWS; row++) {
        matrix.setRow(row, pgm_read_word(&patterns[1][row]));
      }
      matrix.setCorner(MATRIX_CORNER_LOWER_LEFT);
      matrix.setCorner(MATRIX_CORNER_LOWER_RIGHT);
      break;
    case 2:
      for (byte row = 0; row < MATRIX_ROWS; row++) {
        matrix.setRow(row, pgm_read_word(&patterns[2][row]));
      }
      break;
    case 3:
      for (byte row = 0; row < MATRIX_ROWS; row++) {
        matrix.setRow(row, 0b11111111111);
      }
      matrix.setCorners(4);
      break;
    case 4:
      break;
  }
}

/**
 * The up and down buttons can be used to cycle between the different patterns.
 */
void PluginStatic::buttonPressed(PressedButton button) {
  if (button == pbUp) {
    if (pattern == PATTERN_COUNT - 1) {
      pattern = 0;
    } else {
      pattern++;
    }
  } else if (button == pbDown) {
    if (pattern == 0) {
      pattern = PATTERN_COUNT - 1;
    } else {
      pattern--;
    }
  }
  needsUpdate = true;
}
