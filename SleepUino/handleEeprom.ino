/*
  handleEeprom

  handleEeprom is used by SleepUino.ino. It is used to store configuration values permalently on ESP.

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

#include "handleEeprom.h"

HandleEeprom::HandleEeprom() { EEPROM.begin(512); }

uint8_t HandleEeprom::getSunBrightnessValue() { return EEPROM.read(EEADR_BRI_SUN); }

uint8_t HandleEeprom::getMoonBrightnessValue() { return EEPROM.read(EEADR_BRI_MOON); }

uint8_t HandleEeprom::getGainValue() { return EEPROM.read(EEADR_GAIN_SOUND); }

uint8_t HandleEeprom::getDisplayMode() { return EEPROM.read(EEADR_DISPLAY_MODE); }

uint8_t HandleEeprom::getSoundReplay() { return EEPROM.read(EEADR_SOUND_REPLAY); }

void HandleEeprom::setSunBrightnessValue(uint8_t sunBrightness) {
  uint8_t currentValue = EEPROM.read(EEADR_BRI_SUN);
  if (currentValue != sunBrightness) {
    _updateEeprom = true;
    EEPROM.write(EEADR_BRI_SUN, sunBrightness);
  }
}

void HandleEeprom::setMoonBrightnessValue(uint8_t moonBrightness) {
  uint8_t currentValue = EEPROM.read(EEADR_BRI_MOON);
  if (currentValue != moonBrightness) {
    _updateEeprom = true;
    EEPROM.write(EEADR_BRI_MOON, moonBrightness);
  }
}

void HandleEeprom::setGainValue(uint8_t gainValue) {
  uint8_t currentValue = EEPROM.read(EEADR_GAIN_SOUND);
  if (currentValue != gainValue) {
    _updateEeprom = true;
    EEPROM.write(EEADR_GAIN_SOUND, gainValue);
  }
}

void HandleEeprom::setDisplayMode(uint8_t displayMode) {
  uint8_t currentValue = EEPROM.read(EEADR_DISPLAY_MODE);
  if (currentValue != displayMode) {
    _updateEeprom = true;
    EEPROM.write(EEADR_DISPLAY_MODE, displayMode);
  }
}

void HandleEeprom::setSoundReplay(uint8_t soundReplay) {
  uint8_t currentValue = EEPROM.read(EEADR_SOUND_REPLAY);
  if (currentValue != soundReplay) {
    _updateEeprom = true;
    EEPROM.write(EEADR_SOUND_REPLAY, soundReplay);
  }
}

bool HandleEeprom::commitValues() {
  bool returnValue = false;

  if (_updateEeprom) {
    if (EEPROM.commit()) {
      Serial.printf("Save eeprom data successfully\n");
      returnValue = true;
    } else {
      Serial.printf("Error: Save eeprom data failed\n");
    }
  } else {
    // data not changed, commit not necessary
    returnValue = true;
  }
  return returnValue;
}

bool HandleEeprom::getUpdateEeprom() { return _updateEeprom; }

bool HandleEeprom::setWakeCountToZero() {
  uint8_t currentValue = EEPROM.read(EEADR_COUNT);

  if (currentValue != 0) {
    EEPROM.write(EEADR_COUNT, 0);
    _updateEeprom = true;
  }

  return commitValues();
}

// store Wakeup Timings on EEPROM
void HandleEeprom::writeWakeUpTimes(uint16_t wakeUpTimes[], uint8_t count) {
  int i;
  EEPROM.write(EEADR_COUNT, count);
  for (i = 0; i < count; i++) {
    uint16_t time = wakeUpTimes[i];
    Serial.printf("WakeUpTime %d -> %d\n", i + 1, time);

    uint8_t byteLow = time & 0x00FF;
    uint8_t byteHigh = (time & 0xFF00) >> 8;

    Serial.printf("ByteLow %d ByteHigh %d\n", byteLow, byteHigh);
    EEPROM.write(EEADR_COUNT + i * 2 + 1, byteHigh);
    EEPROM.write(EEADR_COUNT + i * 2 + 2, byteLow);
  }
  _updateEeprom = true;
}

void HandleEeprom::readWakeTimes(uint8_t numberOfTimes, uint16_t wakeUpTimes[]) {
  for (int i = 0; i < numberOfTimes; i++) {
    uint8_t byteHigh = EEPROM.read((EEADR_COUNT + 1) + (i * 2));
    uint8_t byteLow = EEPROM.read((EEADR_COUNT + 1) + (i * 2 + 1));

    uint16_t time = (uint16_t)byteHigh;
    time = time << 8;
    time = time | byteLow;

    // Serial.printf("hb %d lb %d time %d\n", byteHigh, byteLow, time);

    wakeUpTimes[i] = time;
  }
}

uint8_t HandleEeprom::getNumberOfWakeTimes() { return EEPROM.read(EEADR_COUNT); }
