/**
 * Global.h
 * Global variables. For configuration directives, see Config.h
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#ifndef Global_h
#define Global_h

#include "Matrix.h"
#include "RTC.h"
#include "Thermometer.h"

// Global variable declarations - definitions in WordQlock.ino
extern Matrix matrix;
extern RTC rtc;
extern Thermometer thermometer;

#endif

