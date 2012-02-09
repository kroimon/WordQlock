/**
 * Plugins.cpp
 * Plugin manager and plugin base classes.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#include "Plugins.h"
#include "Global.h" // for TextScrollerPlugin

/**
 *
 */
PluginManager::PluginManager()
  : plugins(NULL),
    pluginCount(0),
    activePlugin(0),
    lastUpdate(0)
{}

/**
 *
 */
void PluginManager::registerPlugin(Plugin *plugin) {
  plugins = (Plugin **) realloc(plugins, (pluginCount + 1) * sizeof(Plugin *));
  plugins[pluginCount] = plugin;
  pluginCount++;
}

/**
 *
 */
int PluginManager::getActivePlugin() {
  return activePlugin;
}

/**
 *
 */
void PluginManager::setActivePlugin(int id) {
  if (id >= 0 && id < pluginCount) {
    activePlugin = id;
    callShow();
  }
}

/**
 *
 */
void PluginManager::incActivePlugin() {
  if (activePlugin == pluginCount - 1) {
    activePlugin = 0;
  } else {
    activePlugin++;
  }
  callShow();
}


/**
 *
 */
void PluginManager::callShow() {
  plugins[activePlugin]->needsUpdate = false;
  plugins[activePlugin]->show();
  lastUpdate = millis();
}

/**
 *
 */
void PluginManager::callUpdate(boolean realtimeSync) {
  int now = millis();
  if ((realtimeSync && plugins[activePlugin]->syncToRealtime) || plugins[activePlugin]->needsUpdate || (now - lastUpdate >= plugins[activePlugin]->updateInterval)) {
    plugins[activePlugin]->update(now - lastUpdate, realtimeSync);
    plugins[activePlugin]->needsUpdate = false;
    lastUpdate = now;
  }
}

/**
 *
 */
void PluginManager::callButtonPressed(PressedButton button) {
  plugins[activePlugin]->buttonPressed(button);
}



/*
 * class Plugin
 */

Plugin::Plugin(word updateInterval, boolean syncToRealtime)
  : updateInterval(updateInterval),
    syncToRealtime(syncToRealtime),
    needsUpdate(false)
{}



/*
 * class TextScrollerPlugin
 */
 
TextScrollerPlugin::TextScrollerPlugin(word updateInterval, boolean syncToRealtime, const char *text)
  : Plugin(updateInterval, syncToRealtime), text(text), offset(MATRIX_COLS - 1)
{}

void TextScrollerPlugin::show() {
  offset = MATRIX_COLS - 1;
  onTextWrap();
  needsUpdate = true;
}

void TextScrollerPlugin::update(unsigned long timeDiff, boolean realtimeSync) {
  matrix.clear();
  matrix.print(text, offset);

  int textWidth = matrix.getTextWidth(text);
  if (offset <= -textWidth) {
    offset = MATRIX_COLS - 1;
    onTextWrap(); // update the text
  } else {
    offset--;
  }
}
