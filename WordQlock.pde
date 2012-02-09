/**
 * WordQlock - Firmware for a QlockTwo clone
 * Copyright (C) 2012 Stefan Rado
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <Wire.h>
#include <RealTimerOne.h>
#include <SerialCommand.h>
#include <MemoryFree.h>

#include "Config.h"

#include "Global.h"
#include "DCF77.h"
#include "LDR.h"
#include "Button.h"

#include "Plugins.h"
#include "PluginWordClock.h"
#include "PluginSeconds.h"
#include "PluginDate.h"
#include "PluginTemperature.h"
#include "PluginBinary.h"
#include "PluginPong.h"
#include "PluginStatic.h"

// Global variables from Global.h
Matrix matrix(ShiftRegister(PIN_MATRIX_DATA, PIN_MATRIX_CLOCK, PIN_MATRIX_STORE, PIN_MATRIX_OE), PIN_MATRIX_PWM);
RTC rtc(I2C_ADDR_RTC);
Thermometer thermometer(I2C_ADDR_TEMP);

// "Internal" variables
static DCF77 dcf77(PIN_DCF77_SIGNAL, PIN_DCF77_PON);
static LDR ldr(PIN_LDR);
static Button buttonMode(PIN_BUTTON_MODE);
static Button buttonUp(PIN_BUTTON_UP);
static Button buttonDown(PIN_BUTTON_DOWN);

static SerialCommand serialCommands;
static PluginManager pluginManager;


/**
 * Initial setup routine.
 */
void setup() {
  // Setup LED pins
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);

  // Set Timer2 to FastPWM (for Matrix brightness)
  Timer2.setMode(0b011); // 011=FastPWM

  // Setup serial connection to PC
  Serial.begin(115200);
  serialCommands.addCommand("show",   commandShow);
  serialCommands.addCommand("button", commandButton);
  serialCommands.addCommand("set",    commandSet);
  serialCommands.setDefaultHandler(commandUnknown);
  Serial.println(Constant("sradonia WordQlock v" QLOCK_VERSION));
  Serial.println(Constant("Happy to be of service, sir!"));
  Serial.println();

  // Setup Wire library as I2C bus master
  Wire.begin();
  // Start temperature chip
  thermometer.startConversion();

  // Setup button callbacks
  buttonMode.onClick = onButtonClick;
  buttonUp.onClick   = onButtonClick;
  buttonDown.onClick = onButtonClick;
  buttonMode.onLongPress = onButtonLongPress;
  buttonUp.onLongPress   = onButtonLongPress;
  buttonDown.onLongPress = onButtonLongPress;

  // Load plugins
  pluginManager.registerPlugin(new PluginWordClock());  
  pluginManager.registerPlugin(new PluginSeconds());
  pluginManager.registerPlugin(new PluginDate());
  pluginManager.registerPlugin(new PluginTemperature());
  pluginManager.registerPlugin(new PluginBinary());
  pluginManager.registerPlugin(new PluginPong());
  #ifdef DEBUG
    pluginManager.registerPlugin(new PluginStatic());
  #endif
  pluginManager.setActivePlugin(0);

  // Start Timer1 to fill the matrix, one row every 1.1ms
  RealTimer1.attachInterrupt(onTimer1, 1100);
}


/**
 * Called every 1.1 milliseconds by Timer1 to shift out the next matrix row.
 */
void onTimer1() {
  // Enable interrupts (for serial communication etc.)
  sei();
  // Write out one matrix row
  matrix.writeRow();
}


/**
 * Loop.
 */
void loop() {
  unsigned long now = millis();

  // Buttons
  buttonMode.update();
  buttonUp.update();
  buttonDown.update();

  // DCF77
  boolean realtimeSync = false;
  if (dcf77.poll()) {
    TimeStamp dcf77time = dcf77.getTime();
    rtc.write(dcf77time);

    #ifdef DEBUG
      Serial.print(Constant("DCF77: "));
      Serial.println(dcf77time.toString());
    #endif

    // set flag to sync display to realtime
    realtimeSync = true;
  }
  digitalWrite(PIN_LED2, digitalRead(PIN_DCF77_SIGNAL));

  // update plugin display
  pluginManager.callUpdate(realtimeSync);

  // 1 second timer
  static unsigned long nextSecondTimer = now;
  if ((nextSecondTimer <= now) || realtimeSync) {
    nextSecondTimer = now + 1000;

    static boolean ledState = true;
    digitalWrite(PIN_LED1, ledState);
    ledState = !ledState;
  }

  // Adjust brightness
  #ifdef LDR_ENABLED
    adjustBrightness();
  #endif

  // Read and execute serial commands
  serialCommands.readSerial();
}

/**
 * Adjusts the matrix's brightness to the ambient brightness obtained via LDR.
 */
void adjustBrightness() {
  static unsigned long nextAdjustment = 0;
  static byte noChangeCounter = 0;

  if (nextAdjustment <= millis()) {
    byte newBrightness = max(50, ldr.readSmoothed() / 4);

    if (matrix.getBrightness() != newBrightness) {
      matrix.setBrightness(newBrightness);
      noChangeCounter = 0;
    } else {
      if (noChangeCounter < 10)
        noChangeCounter++;
    }

    nextAdjustment = millis() + (noChangeCounter < 10 ? 50 : 500);
  }
}


/**
 * Callback for the buttons.
 */
void onButtonClick(Button &button) {
  if (&button == &buttonMode) {
    #ifdef DEBUG
      Serial.println(Constant("Button clicked: Mode"));
    #endif
    pluginManager.callButtonPressed(pbMode);
    pluginManager.incActivePlugin();
  } else if (&button == &buttonUp) {
    #ifdef DEBUG
      Serial.println(Constant("Button clicked: Up"));
    #endif
    pluginManager.callButtonPressed(pbUp);
  } else if (&button == &buttonDown) {
    #ifdef DEBUG
      Serial.println(Constant("Button clicked: Down"));
    #endif
    pluginManager.callButtonPressed(pbDown);
  }
}

/**
 * Callback for the buttons.
 */
void onButtonLongPress(Button &button) {
  #ifdef DEBUG
    Serial.println(Constant("Button clicked: Long click"));
  #endif
  onButtonClick(button);
}


/*
 * Serial commands
 */

/** show (matrix|time|temp|memory) */
void commandShow() {
  char *arg = serialCommands.next();
  if (arg == NULL) {
    Serial.println(Constant("Error: Missing argument!"));
  }
  else if (strcmp_P(arg, PSTR("matrix")) == 0) {
    Serial.println(Constant("Matrix: "));
    Serial.println(matrix.toString());
  }
  else if (strcmp_P(arg, PSTR("time")) == 0) {
    Serial.print(Constant("Time: "));
    Serial.println(rtc.read().toString());
  }
  else if (strcmp_P(arg, PSTR("temp")) == 0) {
    Serial.print(Constant("Temperature: "));
    Serial.println(thermometer.read().toFloat(), 1);
  }
  else if (strcmp_P(arg, PSTR("ldr")) == 0) {
    Serial.print(Constant("Light sensor: "));
    Serial.print(ldr.read());
    Serial.print(Constant(" ("));
    Serial.print(ldr.readSmoothed());
    Serial.println(')');
  }
  else if (strcmp_P(arg, PSTR("memory")) == 0) {
    Serial.print(Constant("Free memory: "));
    Serial.print(freeMemory());
    Serial.println(Constant(" bytes"));
  }
  else {
    Serial.println(Constant("Error: Unknown argument!"));
  }
}

/** button (mode|up|down) */
void commandButton() {
  char *arg = serialCommands.next();
  if (arg == NULL) {
    Serial.println(Constant("Error: Missing argument!"));
  }
  else if (strcmp_P(arg, PSTR("mode")) == 0) {
    onButtonClick(buttonMode);
  }
  else if (strcmp_P(arg, PSTR("up")) == 0) {
    onButtonClick(buttonUp);
  }
  else if (strcmp_P(arg, PSTR("down")) == 0) {
    onButtonClick(buttonDown);
  }
  else {
    Serial.println(Constant("Error: Unknown button! Valid buttons are: mode, up, down"));
  }
}

/** set brightness x */
void commandSet() {
  char *varName = serialCommands.next();
  char *value   = serialCommands.next();
  if (varName == NULL || value == NULL) {
    Serial.println(Constant("Error: Missing argument!"));
  }
  else if (strcmp_P(varName, PSTR("brightness")) == 0) {
    // Attention: does not work if LDR_ENABLED is defined
    matrix.setBrightness(atoi(value));
    Serial.print(Constant("Matrix brightness set to "));
    Serial.println(matrix.getBrightness());
    #ifdef LDR_ENABLED
      Serial.println(Constant("As LDR is enabled, this might not last for long!"));
    #endif
  }
  #ifdef DEBUG
    else if (strcmp_P(varName, PSTR("timing")) == 0) {
      RealTimer1.setPeriod(atoi(value));
      Serial.println(Constant("Timing changed."));
    }
  #endif
  else {
    Serial.println(Constant("Error: Unknown argument!"));
  }
}

/** Default handler for unknown commands. */
void commandUnknown(const char *command) {
  if (strcmp_P(command, PSTR("help")) != 0) {
    Serial.print(Constant("Error: Unknown command "));
    Serial.println(command);
  }
  Serial.println(Constant("Known commands:"));
  Serial.println(Constant("  help           : This help"));
  Serial.println(Constant("  show matrix    : Show current matrix content"));
  Serial.println(Constant("  show time      : Show time"));
  Serial.println(Constant("  show temp      : Show temperature"));
  Serial.println(Constant("  show ldr       : Show light sensor value"));
  Serial.println(Constant("  show memory    : Show free RAM space"));
  Serial.println(Constant("  button mode    : Simulate button Mode press"));
  Serial.println(Constant("  button up      : Simulate button Up press"));
  Serial.println(Constant("  button down    : Simulate button Down press"));
  Serial.println(Constant("  set brightness : Change matrix brightness (1 - 255)"));
}
