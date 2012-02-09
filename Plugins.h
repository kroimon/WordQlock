/**
 * Plugins.h
 * Plugin manager and plugin base classes.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#ifndef Plugins_h
#define Plugins_h

#include <Wiring.h>

enum PressedButton {
  pbMode,
  pbUp,
  pbDown
};

class Plugin;

class PluginManager {
  public:
    PluginManager();

    void registerPlugin(Plugin *plugin);
    int  getActivePlugin();
    void setActivePlugin(int id);
    void incActivePlugin();

    void callUpdate(boolean force = false);
    void callButtonPressed(PressedButton button);

  private:
    Plugin **plugins;
    int pluginCount;
    int activePlugin;

    unsigned long lastUpdate;

    void callShow();
};

class Plugin {
  friend class PluginManager;

  public:
    Plugin(word updateInterval, boolean syncToRealtime);

    virtual void show() { needsUpdate = true; };
    virtual void update(unsigned long timeDiff, boolean realtimeSync) = 0;
    virtual void buttonPressed(PressedButton button) {};

  protected:
    word updateInterval;
    boolean syncToRealtime;
    boolean needsUpdate;
};


class TextScrollerPlugin : public Plugin {
  public:
    TextScrollerPlugin(word updateInterval, boolean syncToRealtime, const char *text);

    virtual void show();
    virtual void update(unsigned long timeDiff, boolean realtimeSync);

    virtual void onTextWrap() {};

  private:
    const char *text;
    int offset;
};


#endif
