/**
 * TimeStamp.h
 * Encapsulates a date/time stamp.
 *
 * This file is part of WordQlock.
 * Copyright (C) 2012 Stefan Rado
 * Please see WordQlock.pde for license details.
 */
#ifndef TimeStamp_h
#define TimeStamp_h

#include <Wiring.h>

class TimeStamp {
  public:
    TimeStamp();
    TimeStamp(byte seconds, byte minutes, byte hours, byte dayOfWeek, byte day, byte month, byte year);

    void setSeconds(byte seconds);
    void setMinutes(byte minutes);
    void setHours(byte hours);
    void setDayOfWeek(byte dayOfWeek);
    void setDay(byte day);
    void setMonth(byte month);
    void setYear(byte year);

    void incMinutes(boolean independent = true);
    void incHours(boolean independent = true);
    void incDay(boolean independent = true);
    void incMonth(boolean independent = true);
    void incYear();

    byte getSeconds() const;
    byte getMinutes() const;
    byte getHours() const;
    byte getDayOfWeek() const;
    byte getDay() const;
    byte getMonth() const;
    byte getYear() const;

    boolean equals(const TimeStamp& rhs) const;

    const char *getDayOfWeekString() const;
    String toString() const;

  private:
    byte seconds;
    byte minutes;
    byte hours;

    byte dayOfWeek;
    byte day;
    byte month;
    byte year;
};

/*
 * Inline getters
 */

inline byte TimeStamp::getSeconds() const {
  return seconds;
}

inline byte TimeStamp::getMinutes() const {
  return minutes;
}

inline byte TimeStamp::getHours() const {
  return hours;
}

inline byte TimeStamp::getDayOfWeek() const {
  return dayOfWeek;
}

inline byte TimeStamp::getDay() const {
  return day;
}

inline byte TimeStamp::getMonth() const {
  return month;
}

inline byte TimeStamp::getYear() const {
  return year;
}

#endif

