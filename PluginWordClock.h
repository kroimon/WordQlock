/**
 * PluginWordClock.h
 * Plugin to display the standard word clock time.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#ifndef PluginWordClock_h
#define PluginWordClock_h

#include <Wiring.h>
#include "Plugins.h"

class PluginWordClock : public Plugin {
  public:
    PluginWordClock();

    virtual void update(unsigned long timeDiff, boolean realtimeSync);
    virtual void buttonPressed(PressedButton button);
};

#endif

