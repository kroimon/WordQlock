/**
 * PluginSeconds.cpp
 * Plugin to display the current time seconds.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#include "PluginSeconds.h"
#include "Global.h"


/**
 *
 */
PluginSeconds::PluginSeconds()
  : Plugin(990, true) // 1 second update interval, realtime sync
{}

/**
 * 
 */
void PluginSeconds::update(unsigned long timeDiff, boolean realtimeSync) {
  byte seconds = rtc.read().getSeconds();
  char str[3];
  str[0] = '0' + (seconds / 10);
  str[1] = '0' + (seconds % 10);
  str[2] = '\0';

  matrix.clear();
  matrix.print(str);
}
