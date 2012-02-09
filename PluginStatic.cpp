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
  matrix.clear();
  switch (pattern) {
    case 0:
      matrix.setRow(0, 0b10101010101);
      matrix.setRow(1, 0b01010101010);
      matrix.setRow(2, 0b10101010101);
      matrix.setRow(3, 0b01010101010);
      matrix.setRow(4, 0b10101010101);
      matrix.setRow(5, 0b01010101010);
      matrix.setRow(6, 0b10101010101);
      matrix.setRow(7, 0b01010101010);
      matrix.setRow(8, 0b10101010101);
      matrix.setRow(9, 0b01010101010);
      matrix.setCorner(MATRIX_CORNER_UPPER_LEFT);
      matrix.setCorner(MATRIX_CORNER_UPPER_RIGHT);
      break;
    case 1:
      matrix.setRow(0, 0b01010101010);
      matrix.setRow(1, 0b10101010101);
      matrix.setRow(2, 0b01010101010);
      matrix.setRow(3, 0b10101010101);
      matrix.setRow(4, 0b01010101010);
      matrix.setRow(5, 0b10101010101);
      matrix.setRow(6, 0b01010101010);
      matrix.setRow(7, 0b10101010101);
      matrix.setRow(8, 0b01010101010);
      matrix.setRow(9, 0b10101010101);
      matrix.setCorner(MATRIX_CORNER_LOWER_LEFT);
      matrix.setCorner(MATRIX_CORNER_LOWER_RIGHT);
      break;
    case 2:
      matrix.setRow(0, 0b00000000000);
      matrix.setRow(1, 0b00110001100);
      matrix.setRow(2, 0b01001010010);
      matrix.setRow(3, 0b01000100010);
      matrix.setRow(4, 0b01000000010);
      matrix.setRow(5, 0b00100000100);
      matrix.setRow(6, 0b00010001000);
      matrix.setRow(7, 0b00001010000);
      matrix.setRow(8, 0b00000100000);
      matrix.setRow(9, 0b00000000000);
      break;
    case 3:
      matrix.setRow(0, 0b11111111111);
      matrix.setRow(1, 0b11111111111);
      matrix.setRow(2, 0b11111111111);
      matrix.setRow(3, 0b11111111111);
      matrix.setRow(4, 0b11111111111);
      matrix.setRow(5, 0b11111111111);
      matrix.setRow(6, 0b11111111111);
      matrix.setRow(7, 0b11111111111);
      matrix.setRow(8, 0b11111111111);
      matrix.setRow(9, 0b11111111111);
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
