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


#ifndef HANDLEEEPROM_H_INCLUDED
#define HANDLEEEPROM_H_INCLUDED

#include <EEPROM.h>

class HandleEeprom
{
    public:
        HandleEeprom();
        
        bool getUpdateEeprom();

        void writeWakeUpTimes(uint16_t wakeUpTimes[], uint8_t count);
        bool setWakeCountToZero();
        uint8_t getNumberOfWakeTimes();
        void readWakeTimes(uint8_t numberOfTimes, uint16_t wakeUpTimes[]);

        uint8_t getSunBrightnessValue();
        uint8_t getMoonBrightnessValue();
        uint8_t getGainValue();

        uint8_t getDisplayMode();
        uint8_t getSoundReplay();

        void setSunBrightnessValue(uint8_t sunBrightness);
        void setMoonBrightnessValue(uint8_t moonBrightness);
        void setGainValue(uint8_t gainValue);

        void setDisplayMode(uint8_t displayMode);
        void setSoundReplay(uint8_t soundReplay);

        bool commitValues();

    private:
        const int EEADR_BRI_SUN = 0;
        const int EEADR_BRI_MOON = 1;
        const int EEADR_GAIN_SOUND = 2;
        const int EEADR_DISPLAY_MODE = 3;
        const int EEADR_SOUND_REPLAY = 4;
        
        //Number of entries for wake up times
        //the next bytes will be the entries, 2 bytes per entry
        const int EEADR_COUNT = 10;
        bool _updateEeprom = false;
        enum timeState {SUN, MOON};
};

#endif // HANDLEEEPROM_H_INCLUDED