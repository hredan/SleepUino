/*
  config.h

  config.h is used by SleepUino. It contains values for configuration.

  Information and contribution at https://github.com/hredan/SleepUino.

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

#ifndef SLEEPUINO_CONFIG_H_
#define SLEEPUINO_CONFIG_H_

// define constant values
const int GAIN_MIN = 2;
const int GAIN_MAX = 100;
const int BRIGHTNESS_MIN = 10;
const int BRIGHTNESS_MAX = 100;
const int LONG_PRESS = 1000;  // long press > 1 sec
const int MAX_SOUND_REPLAYS = 10;
const int MAX_CHARACTOR_TIME_STR = 6;
const int MAX_GET_TIME_BUFFER_SIZE = 100;


//GPIOs
#define LED_SUN_GPIO 2
#define LED_MOON_GPIO 1
#define BUTTON_GPIO 0
// I2C SLC D6, SDA D7 Used for Real Time Clock and Display
#define SLC_PIN 12
#define SDA_PIN 13
// Max number for wake up entries
const int MAX_WAKE_TIMES = 10;

// WiFi SSID (fixed, not configurable)
const char WIFI_SSID[] = "SleepUino";

// time types
enum TimeTypes { TT_UNKNOWN, TT_MOON, TT_SUN, TT_SUN_ALARM };

enum DisplayMode { DM_ON, DM_OFF, DM_ON_AT_BEDTIME, DM_ON_AT_DAYTIME };

const int DISPLAY_TIME_BEFORE = 0;
// define states for state machine, used in SleepUino.ino
enum states_t { OPS, TO_WIFI, WIFI, TO_OPS };

#endif  // SLEEPUINO_CONFIG_H_
