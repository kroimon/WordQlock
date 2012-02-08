/**
 * PluginStatic.h
 * Plugin to display static patterns/images.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#ifndef PluginStatic_h
#define PluginStatic_h

#include <Wiring.h>
#include "Plugins.h"

class PluginStatic : public Plugin {
  public:
    PluginStatic();

    virtual void update(unsigned long timeDiff, boolean realtimeSync);
    virtual void buttonPressed(PressedButton button);
  
  private:
    byte pattern;
};

#endif

