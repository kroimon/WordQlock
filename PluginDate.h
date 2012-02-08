/**
 * PluginDate.h
 * Plugin to display the current date.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#ifndef PluginDate_h
#define PluginDate_h

#include <Wiring.h>
#include "Plugins.h"

class PluginDate : public TextScrollerPlugin {
  public:
    PluginDate();

    virtual void onTextWrap();

  private:
    char text[20];
};

#endif

