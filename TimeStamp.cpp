/**
 * TimeStamp.cpp
 * Encapsulates a date/time stamp.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#include "TimeStamp.h"


TimeStamp::TimeStamp()
  : seconds  (0),
    minutes  (0),
    hours    (0),
    dayOfWeek(1),
    day      (1),
    month    (1),
    year     (1)
{}

TimeStamp::TimeStamp(byte seconds, byte minutes, byte hours, byte dayOfWeek, byte day, byte month, byte year) {
  setSeconds(seconds);
  setMinutes(minutes);
  setHours(hours);
  setDayOfWeek(dayOfWeek);
  setDay(day);
  setMonth(month);
  setYear(year);
}


/*
 * Setter
 */

void TimeStamp::setSeconds(byte seconds) {
  this->seconds = constrain(seconds, 0, 59);
}

void TimeStamp::setMinutes(byte minutes) {
  this->minutes = constrain(minutes, 0, 59);
}

void TimeStamp::setHours(byte hours) {
  this->hours = constrain(hours, 0, 23);
}

void TimeStamp::setDayOfWeek(byte dayOfWeek) {
  this->dayOfWeek = constrain(dayOfWeek, 1, 7);
}

void TimeStamp::setDay(byte day) {
  this->day = constrain(day, 1, 31);
}

void TimeStamp::setMonth(byte month) {
  this->month = constrain(month, 1, 12);
}

void TimeStamp::setYear(byte year) {
  this->year = constrain(year, 0, 99);
}


void TimeStamp::incMinutes(boolean independent) {
  if (minutes == 59) {
    minutes = 0;
    if (!independent) {
      incHours();
    }
  } else {
    minutes++;
  }
}

void TimeStamp::incHours(boolean independent) {
  if (hours == 23) {
    hours = 0;
    if (!independent) {
      incDay(false);
    }
  } else {
    hours++;
  }
}

void TimeStamp::incDay(boolean independent) {
  static const byte monthDays[] PROGMEM = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
  if (day == pgm_read_byte(&monthDays[month - 1])) {
    day = 1;
    if (!independent) {
      incMonth(false);
    }
  } else {
    day++;
  }
  if (dayOfWeek == 7) {
    dayOfWeek = 1;
  } else {
    dayOfWeek++;
  }
}

void TimeStamp::incMonth(boolean independent) {
  if (month == 12) {
    month = 1;
    if (!independent) {
      incYear();
    }
  } else {
    month++;
  }
}

void TimeStamp::incYear() {
  if (year == 99) {
    year = 0;
  } else {
    year++;
  }
}


boolean TimeStamp::equals(const TimeStamp& rhs) const {
  return (seconds   == rhs.seconds)
      && (minutes   == rhs.minutes)
      && (hours     == rhs.hours)
      && (dayOfWeek == rhs.dayOfWeek)
      && (day       == rhs.day)
      && (month     == rhs.month)
      && (year      == rhs.year);
}


const char *TimeStamp::getDayOfWeekString() const {
  static const char *dayOfWeekStr[] PROGMEM = {"Mo", "Di", "Mi", "Do", "Fr", "Sa", "So"}; // array in PROGMEM, strings in RAM
  return (const char*) pgm_read_word(&dayOfWeekStr[dayOfWeek - 1]);
}


String TimeStamp::toString() const {
  char str[25];

  // snprintf_P(str, 25, PSTR("%s, %02d.%02d.%02d %02d:%02d:%02d"), getDayOfWeekString(), day, month, year, hours, minutes, seconds);

  strcpy(str, getDayOfWeekString());
  strcat_P(str, PSTR(", 00.00.00 00:00:00"));
  str[4]  += (day / 10);
  str[5]  += (day % 10);
  str[7]  += (month / 10);
  str[8]  += (month % 10);
  str[10] += (year / 10);
  str[11] += (year % 10);
  str[13] += (hours / 10);
  str[14] += (hours % 10);
  str[16] += (minutes / 10);
  str[17] += (minutes % 10);
  str[19] += (seconds / 10);
  str[20] += (seconds % 10);

  return String(str);
}
