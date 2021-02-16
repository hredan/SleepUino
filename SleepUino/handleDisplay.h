/*
  handleDisplay
  
  handleDisplay is used by SleepUino to handle the display.
  It is using the U8g2 library from olikraus https://github.com/olikraus/u8g2.

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

#ifndef HANDLEDISPLAY_H_INCLUDED
#define HANDLEDISPLAY_H_INCLUDED

#include <U8g2lib.h>
class HandleDisplay
{
    public:
        HandleDisplay();
        void begin();
        void setTime(char *timeStr);
        void setWlanStatus(bool wlan);
        void setAlarmStatus(bool alarm, bool force=true);
        void setEnableDisplay(bool enableDisplay, bool force=false);

        bool getEnableDisplay();
    private:
        //define Display type
        U8G2_SH1106_128X64_NONAME_F_HW_I2C *_u8g2;
        bool _wlan;
        bool _alarm;
        bool _enableDisplay;
        char _timeStr[6];
        void updateDisplay();
};
#endif // HANDLEISPLAY_H_INCLUDED