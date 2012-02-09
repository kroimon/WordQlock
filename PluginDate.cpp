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

  byte pos = strlen(strcpy(text, t.getDayOfWeekString()));
  text[pos++] = ',';
  text[pos++] = ' ';
  byte day   = t.getDay();
  text[pos++] = '0' + (day / 10);
  text[pos++] = '0' + (day % 10);
  text[pos++] = '.';
  byte month = t.getMonth();
  text[pos++] = '0' + (month / 10);
  text[pos++] = '0' + (month % 10);
  text[pos++] = '.';
  byte year  = t.getYear();
  text[pos++] = '0' + (year / 10);
  text[pos++] = '0' + (year % 10);
  text[pos] = '\0';
}
