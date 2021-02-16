/*
  Config.h
  
  Config.h is used by SleepUino. It contains values for configuration.
  
  Information and contribution at https://www.sleepuino.sourcecode3d.de/.
  
  Copyright (C) 2021  André Herrmann
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef HANDLE_CONFIG_H_INCLUDED
#define HANDLE_CONFIG_H_INCLUDED

#include "LittleFS.h"

//define constant values
const int GAIN_MIN = 2;
const int GAIN_MAX = 100;
const int BRIGHTNESS_MIN = 10;
const int BRIGHTNESS_MAX = 100;
const int LONG_PRESS = 1000; //long press > 1 sec
const int MAX_SOUND_REPLAYS = 10;

const byte
    LED_SUN_PIN(5),            // PIN to connect the SUN LED
    LED_MOON_PIN(4),           // PIN to connect the MOON LED
    BUTTON_PIN(0);             // PIN to connect button

//I2C SLC D6, SDA D7 Used for Real Time Clock and Display
const int SLC_PIN = 12;
const int SDA_PIN = 13;

//Max number for wake up entries
const int MAX_WAKE_TIMES = 10;

//time types
enum TimeTypes {TT_UNKNOWN, TT_MOON, TT_SUN, TT_SUN_ALARM};

enum DisplayMode {DM_ON, DM_OFF, DM_ON_AT_BEDTIME, DM_ON_AT_DAYTIME};

const int DISPLAY_TIME_BEFORE = 30;
//define states for state machine, used in SleepUino.ino
enum states_t {OPS, TO_WIFI, WIFI, TO_OPS};

#endif //HANDLE_CONFIG_H_INCLUDED