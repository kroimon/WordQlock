/**
 * PluginBinary.h
 * Plugin to display a binary clock.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#ifndef PluginBinary_h
#define PluginBinary_h

#include <Wiring.h>
#include "Plugins.h"

class PluginBinary : public Plugin {
  public:
    PluginBinary();

    virtual void update(unsigned long timeDiff, boolean realtimeSync);
    virtual void buttonPressed(PressedButton button);

  private:
    byte mode;

    byte reverse(byte b) const;
};

#endif
