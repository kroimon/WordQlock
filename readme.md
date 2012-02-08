# WordQlock #
Firmware for a QlockTwo clone by Stefan Rado


## Hardware ##
The WordQlock is based on an ATmega328p in a Wiring/Arduino compatible environment.

It closely follows the instructions on http://code.google.com/p/grosses-bastelwochenende/
as developed by [Christian Aschoff](http://christians-bastel-leben.blogspot.com/) with small changes.

It's using some different components and the pin configuration of the [RBBB](http://jeelabs.com/products/rbbb) differs a lot.
For temperature display there is a DS1621 Digital Thermometer connected via I2C and the RTC is integrated onto a JeeLabs [RTC Plug](http://jeelabs.com/products/rtc-plug).

The pin configuration can be found in `Config.h`.


## Software ##
The firmware for the clock was written from scratch with influences from the original firmware by Christian and the [multiqlock firmware](http://code.google.com/p/multiqlock/) by Andreas.

**Warning:** In contrast to the other firmwares around, this one is written to be compiled with [Wiring](http://wiring.co/), not Arduino.
Most of the code is compatible as Arduino is based on Wiring, but some timer specific lines would need changes.

All configration directives may be found in `Config.h`.


### Required libraries ###
The following libraries are currently required to compile the firmware:

* *RealTimerOne*  
  Library to easily set up a repeated task using Timer1 (in microsecond resolution)  
  https://github.com/kroimon/Wiring-RealTimerOne

* *SerialCommand*  
  Library to tokenize and parse commands received over a serial port.  
  https://github.com/kroimon/Wiring-SerialCommand

* *MemoryFree*  
  Library to get the free RAM on an ATmega microcontroller.  
  http://arduino.cc/playground/Code/AvailableMemory


### License ###
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
