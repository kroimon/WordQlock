/**
 * PluginDate.cpp
 * Plugin to display the current date.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#include "PluginDate.h"
#include "Global.h"


/**
 *
 */
PluginDate::PluginDate()
  : TextScrollerPlugin(100, false, text) // 0.1 second update interval, no realtime sync
{}

/**
 *
 */
void PluginDate::onTextWrap() {
  TimeStamp t = rtc.read();

  //snprintf_P(text, 15, PSTR("%s, %02d.%02d.%02d"), dayOfWeekStr, t.getDay(), t.getMonth(), t.getYear());

  strcpy(text, t.getDayOfWeekString());
  strcat_P(text, PSTR(", 00.00.00"));
  byte day   = t.getDay();
  text[4] += (day / 10);
  text[5] += (day % 10);
  byte month = t.getMonth();
  text[7] += (month / 10);
  text[8] += (month % 10);
  byte year  = t.getYear();
  text[10] += (year / 10);
  text[11] += (year % 10);
}
