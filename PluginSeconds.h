/**
 * PluginSeconds.h
 * Plugin to display the current time seconds.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#ifndef PluginSeconds_h
#define PluginSeconds_h

#include <Wiring.h>
#include "Plugins.h"

class PluginSeconds : public Plugin {
  public:
    PluginSeconds();

    virtual void update(unsigned long timeDiff, boolean realtimeSync);
};

#endif
