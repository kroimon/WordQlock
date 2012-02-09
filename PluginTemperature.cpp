/**
 * PluginTemperature.cpp
 * Plugin to display the current temperature.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#include "PluginTemperature.h"
#include "Global.h"


/**
 *
 */
PluginTemperature::PluginTemperature()
  : TextScrollerPlugin(100, false, text) // 0.1 second update interval, no realtime sync
{}

/**
 *
 */
void PluginTemperature::onTextWrap() {
  byte pos = thermometer.read().toString(text) - 1;
  text[pos++] = '\xF8'; // degree symbol
  text[pos++] = 'C';
  text[pos++] = '\0';
}
