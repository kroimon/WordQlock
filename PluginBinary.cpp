/**
 * PluginBinary.cpp
 * Plugin to display a binary clock.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#include "PluginBinary.h"
#include "Global.h"

#define MODE_COUNT 8


/**
 *
 */
PluginBinary::PluginBinary()
  : Plugin(1000, true), // 1 second update interval, realtime sync
    mode(0)
{}

/**
 * Reverse the order of bits in a byte.
 * Found at http://stackoverflow.com/a/2602885/291823
 */
byte PluginBinary::reverse(byte b) const {
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return b;
}

/**
 * 
 */
void PluginBinary::update(unsigned long timeDiff, boolean realtimeSync) {
  TimeStamp t = rtc.read();

  byte hours     = t.getHours();
  byte hours10   = 0;
  byte minutes   = t.getMinutes();
  byte minutes10 = 0;
  byte seconds   = t.getSeconds();
  byte seconds10 = 0;

  if (mode % 2 == 1) {
    hours10   = hours   / 10;
    hours     = hours   % 10;
    minutes10 = minutes / 10;
    minutes   = minutes % 10;
    seconds10 = seconds / 10;
    seconds   = seconds % 10;
  }

  matrix.clear();
  switch (mode) {
    case 0: // Horizontal, right-aligned
      matrix.setRow(2, hours  );
      matrix.setRow(4, minutes);
      matrix.setRow(6, seconds);
      break;
    case 1: // Horizontal, right-aligned, digit-wise
      matrix.setRow(1, hours10  );
      matrix.setRow(2, hours    );
      matrix.setRow(4, minutes10);
      matrix.setRow(5, minutes  );
      matrix.setRow(7, seconds10);
      matrix.setRow(8, seconds  );
      break;
    case 2: // Vertical, bottom-aligned
      matrix.setCol(2, reverse(hours  ) << (MATRIX_ROWS - 8));
      matrix.setCol(5, reverse(minutes) << (MATRIX_ROWS - 8));
      matrix.setCol(8, reverse(seconds) << (MATRIX_ROWS - 8));
      break;
    case 3: // Vertical, bottom-aligned, digit-wise
      matrix.setCol(1, reverse(hours10  ) << (MATRIX_ROWS - 8));
      matrix.setCol(2, reverse(hours    ) << (MATRIX_ROWS - 8));
      matrix.setCol(4, reverse(minutes10) << (MATRIX_ROWS - 8));
      matrix.setCol(5, reverse(minutes  ) << (MATRIX_ROWS - 8));
      matrix.setCol(7, reverse(seconds10) << (MATRIX_ROWS - 8));
      matrix.setCol(8, reverse(seconds  ) << (MATRIX_ROWS - 8));
      break;
    case 4: // Horizontal, left-aligned
      matrix.setRow(2, reverse(hours  ) << (MATRIX_COLS - 8));
      matrix.setRow(4, reverse(minutes) << (MATRIX_COLS - 8));
      matrix.setRow(6, reverse(seconds) << (MATRIX_COLS - 8));
      break;
    case 5: // Horizontal, left-aligned, digit-wise
      matrix.setRow(1, reverse(hours10  ) << (MATRIX_COLS - 8));
      matrix.setRow(2, reverse(hours    ) << (MATRIX_COLS - 8));
      matrix.setRow(4, reverse(minutes10) << (MATRIX_COLS - 8));
      matrix.setRow(5, reverse(minutes  ) << (MATRIX_COLS - 8));
      matrix.setRow(7, reverse(seconds10) << (MATRIX_COLS - 8));
      matrix.setRow(8, reverse(seconds  ) << (MATRIX_COLS - 8));
      break;
    case 6: // Vertical, top-aligned
      matrix.setCol(2, hours  );
      matrix.setCol(5, minutes);
      matrix.setCol(8, seconds);
      break;
    case 7: // Vertical, top-aligned, digit-wise
      matrix.setCol(1, hours10  );
      matrix.setCol(2, hours    );
      matrix.setCol(4, minutes10);
      matrix.setCol(5, minutes  );
      matrix.setCol(7, seconds10);
      matrix.setCol(8, seconds  );
      break;
  }
}

/**
 * The up and down buttons can be used to cycle between the different modes.
 */
void PluginBinary::buttonPressed(PressedButton button) {
  if (button == pbUp) {
    if (mode == MODE_COUNT - 1) {
      mode = 0;
    } else {
      mode++;
    }
  } else if (button == pbDown) {
    if (mode == 0) {
      mode = MODE_COUNT - 1;
    } else {
      mode--;
    }
  }
  needsUpdate = true;
}
