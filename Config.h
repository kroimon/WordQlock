/**
 * Config.h
 * Hardware-specific compile-time settings.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#ifndef Config_h
#define Config_h

#define QLOCK_VERSION "0.1"

// ****************************************************************************************************
// Pins & I2C addresses
// ****************************************************************************************************

// DCF77 module
#define PIN_DCF77_PON     A1  // Power on (active low)
#define PIN_DCF77_SIGNAL  A2  // DCF77 signal

// Matrix (shift registers)
#define PIN_MATRIX_CLOCK  5  // Shift clock
#define PIN_MATRIX_STORE  6  // Store clock (latch)
#define PIN_MATRIX_OE     7  // Output enable (active low)
#define PIN_MATRIX_DATA   8  // Serial shift data
#define PIN_MATRIX_PWM    3  // PWM brightness pin (timer2, pin 3 or 11)

// Buttons (active high, external pull-downs)
#define PIN_BUTTON_UP     9  // Up (or Hour+)
#define PIN_BUTTON_DOWN  10  // Down (or Minute+)
#define PIN_BUTTON_MODE  11  // Mode

// LEDs
#define PIN_LED1         12  // Green LED
#define PIN_LED2         13  // Yellow LED (also used by bootloader)

// LDR
#define PIN_LDR          A3  // Light sensor

// I2C bus addresses
#define I2C_ADDR_RTC   0x68  // DS1340 on JeeLabs RTC plug
#define I2C_ADDR_TEMP  0x48  // DS1621 Digital Thermometer


// ****************************************************************************************************
// Settings
//   to disable a feature, comment out the corresponding define
// ****************************************************************************************************

// Debug flag
#define DEBUG

// Enables the light sensor
#define LDR_ENABLED

// LDR sample values to calibrate the light sensor
#define LDR_SAMPLE_DARK    1000
#define LDR_SAMPLE_BRIGHT   200

// The "speed" of matrix brightness adaption to ambient brightness changes
#define LDR_BRIGHTNESS_GAIN .2f

#endif
