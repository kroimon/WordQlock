/**
 * PluginTemperature.h
 * Plugin to display the current temperature.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#ifndef PluginTemperature_h
#define PluginTemperature_h

#include <Wiring.h>
#include "Plugins.h"

/**
 *
 */
class PluginTemperature : public TextScrollerPlugin {
  public:
    PluginTemperature();

    virtual void onTextWrap();

  private:
    char text[20];
};

#endif

