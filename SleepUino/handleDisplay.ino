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

#include "handleDisplay.h"

HandleDisplay::HandleDisplay()
{
    _u8g2 = new U8G2_SH1106_128X64_NONAME_F_HW_I2C(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
    _wlan = false;
    _alarm = false;
    _enableDisplay = false;
    strcpy(_timeStr, "00:00");
};

void HandleDisplay::setWlanStatus(bool wlan)
{
    if (_wlan != wlan)
    {
        _wlan = wlan;
        updateDisplay();
    }
}

void HandleDisplay::setEnableDisplay(bool enableDisplay, bool force)
{
    Serial.printf("Enter HandleDisplay::setEnableDisplay: enableDisplay = %s, force = %s\n", enableDisplay ? "true": "false", force ? "true": "false");
    if (_enableDisplay != enableDisplay)
    {
        _enableDisplay = enableDisplay;
        if (force)
        {
            updateDisplay();
        }
    }
}

bool HandleDisplay::getEnableDisplay()
{
    return _enableDisplay;
}

void HandleDisplay::setAlarmStatus(bool alarm, bool force)
{
    Serial.printf("HandleDisplay: Enter setAlarmStatus alarm: %s, force %s\n", alarm ? "true" : "false", force ? "true" : "false");
    if (_alarm != alarm)
    {
        _alarm = alarm;
        
        if (force)
        {
            updateDisplay();
        }
    }
}

void HandleDisplay::setTime(char *timeStr)
{
    if (strncmp(timeStr, _timeStr, 5) != 0)
    {
        strcpy(_timeStr, timeStr);
        Serial.printf("update _timeStr: %s\n", _timeStr);
        updateDisplay();
    }
}

void HandleDisplay::begin(){
    _u8g2->begin();
}

void HandleDisplay::updateDisplay(){
    
    //Enable Display allways in Config Mode
    if (_enableDisplay || _wlan)
    {
        _u8g2->setPowerSave(0);
        _u8g2->clearBuffer();					// clear the internal memory
        _u8g2->setFont(u8g2_font_logisoso42_tn);
        _u8g2->setFontDirection(0);
        _u8g2->drawStr(0,63, _timeStr);

        if (_wlan)
        {
            _u8g2->setFont(u8g2_font_open_iconic_embedded_2x_t);
            _u8g2->setFontDirection(1);
            _u8g2->drawUTF8(2,2, "P");
        }

        if (_alarm)
        {
            _u8g2->setFont(u8g2_font_unifont_t_symbols);
            _u8g2->setFontDirection(0);
            _u8g2->drawUTF8(100,16, "⏰");
        }
        //u8g2.drawFrame(0,0,128,64);
        _u8g2->sendBuffer();					// transfer internal memory to the display
        Serial.printf("Display: update display time: %s wlan:%s alarm:%s\n", _timeStr, _wlan ? "true": "false", _alarm ? "true": "false");
    }
    else
    {
        _u8g2->setPowerSave(1);
        Serial.println("Display: PowerSave Enabled");
    }
    
}
