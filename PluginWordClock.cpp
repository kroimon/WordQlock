/**
 * PluginWordClock.cpp
 * Plugin to display the standard word clock time.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#include "PluginWordClock.h"
#include "Global.h"


/**
 *
 */
PluginWordClock::PluginWordClock()
  : Plugin(1000, true) // 1 second update interval, realtime sync
{}

/**
 *
 */
void PluginWordClock::update(unsigned long timeDiff, boolean realtimeSync) {
  TimeStamp timeStamp = rtc.read();
  byte hours   = timeStamp.getHours() % 12;
  byte minutes = timeStamp.getMinutes();

  matrix.clear();

  // ES IST
  matrix.setRowOr(0, 0b11011100000);

  // Minutes
  switch (minutes / 5) {
    case 0: // :00
      matrix.setRowOr(9, 0b00000000111); // UHR
      break;
    case 1: // :05
      matrix.setRowOr(0, 0b00000001111); // FÜNF
      matrix.setRowOr(3, 0b00000001111); // NACH
      break;
    case 2: // :10
      matrix.setRowOr(1, 0b11110000000); // ZEHN
      matrix.setRowOr(3, 0b00000001111); // NACH
      break;
    case 3: // :15
      matrix.setRowOr(2, 0b00001111111); // VIERTEL
      matrix.setRowOr(3, 0b00000001111); // NACH
      break;
    case 4: // :20
      matrix.setRowOr(1, 0b00001111111); // ZWANZIG
      matrix.setRowOr(3, 0b00000001111); // NACH
      break;
    case 5: // :25
      matrix.setRowOr(0, 0b00000001111); // FÜNF
      matrix.setRowOr(3, 0b11100000000); // VOR
      matrix.setRowOr(4, 0b11110000000); // HALB
      hours++;
      break;
    case 6: // :30
      matrix.setRowOr(4, 0b11110000000); // HALB
      hours++;
      break;
    case 7: // :35
      matrix.setRowOr(0, 0b00000001111); // FÜNF
      matrix.setRowOr(3, 0b00000001111); // NACH
      matrix.setRowOr(4, 0b11110000000); // HALB
      hours++;
      break;
    case 8: // :40
      matrix.setRowOr(1, 0b00001111111); // ZWANZIG
      matrix.setRowOr(3, 0b11100000000); // VOR
      hours++;
      break;
    case 9: // :45
      matrix.setRowOr(2, 0b00001111111); // VIERTEL
      matrix.setRowOr(3, 0b11100000000); // VOR
      hours++;
      break;
    case 10: // :50
      matrix.setRowOr(1, 0b11110000000); // ZEHN
      matrix.setRowOr(3, 0b11100000000); // VOR
      hours++;
      break;
    case 11: // :55
      matrix.setRowOr(0, 0b00000001111); // FÜNF
      matrix.setRowOr(3, 0b11100000000); // VOR
      hours++;
      break;
  }
  matrix.setCorners(minutes % 5);

  // Hours
  switch (hours) {
    case  1:
      if (minutes / 5 == 0)
        matrix.setRowOr(5, 0b11100000000);              // EIN
      else
        matrix.setRowOr(5, 0b11110000000);              // EINS
      break;
    case  2: matrix.setRowOr(5, 0b00000001111); break;  // ZWEI
    case  3: matrix.setRowOr(6, 0b11110000000); break;  // DREI
    case  4: matrix.setRowOr(6, 0b00000001111); break;  // VIER
    case  5: matrix.setRowOr(4, 0b00000001111); break;  // FÜNF
    case  6: matrix.setRowOr(7, 0b11111000000); break;  // SECHS
    case  7: matrix.setRowOr(8, 0b11111100000); break;  // SIEBEN
    case  8: matrix.setRowOr(7, 0b00000001111); break;  // ACHT
    case  9: matrix.setRowOr(9, 0b00011110000); break;  // NEUN
    case 10: matrix.setRowOr(9, 0b11110000000); break;  // ZEHN
    case 11: matrix.setRowOr(4, 0b00000111000); break;  // ELF
    case 12:
    case  0: matrix.setRowOr(8, 0b00000011111); break;  // ZWÖLF
  }
}
