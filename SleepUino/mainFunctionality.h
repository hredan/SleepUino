/*
  mainFunctionality
  
  mainFunctionality is used by SleepUino to support the base functionlality.
  
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

#ifndef HANDLE_MAIN_H_INCLUDED
#define HANDLE_MAIN_H_INCLUDED

#include "handleAudio.h"
#include "handleWebpage.h"
#include "handleEeprom.h"
#include "handleDisplay.h"
#include "RTClib.h"

#include "ArduinoJson.h"

class MainFunc
{
    public:
        MainFunc();
        void updateDisplay();
        void buttonReleasedInOpsState();

        void updateDisplay(bool force);
        void storeValuesInEeprom();
        TimeTypes checkState(uint16_t currentTimeInMinutes);
        static void startLittleFileSystem();
        
        //static methodes used for callbacks
        static void setSunBrightness(uint8_t sunBrightness);
        static void setMoonBrightness(uint8_t moonBrightness);
        static void setGainSound(uint8_t gainSound);
        static void setSoundReplay(uint8_t soundReplay);
        static void setDisplayMode(uint8_t displayMode);

        static uint8_t getSunBrightness();
        static uint8_t getMoonBrightness();
        static uint8_t getGainSound();
        static uint8_t getDisplayMode();
        static uint8_t getSoundReplayes();
        
        static void playSound();

        static void setWakeTimeData(uint8_t size, uint16_t dataArr[]);
        static String getWakeTimeDataJsonStr();

        void setupEeprom();
        bool isSoundRunning();
        void webpageCheckClient();
        void setWlanStatusOnDisplay(bool wlan);
        void startRealTimeClock();

        void setInConfigMode(bool value);

        
    private:
        void checkDisplayOnBeforeWakeTime(DateTime currentTime);

        void enableDisplayShort();
        void enableDisplayByButton();
        bool _alarmUntilPressButton = false;
        bool _alarm = false;
        int _numOfAlarmReplays = 0;

        unsigned long _lastMicroTime = 0;
        unsigned long _milliTime;

        int _lastHour = 0;
        int _lastMinute = 0;

        int _indexOfNextWakeTime = 0;

        TimeTypes _currentTimeType = TT_UNKNOWN;
        TimeTypes _lastTimeType = TT_UNKNOWN;

        bool _displayOn = false;
        DateTime _displayOnTime;
        DateTime _now;

        bool _enableDisplayTimer = false;
        unsigned long _enableDisplayTime = 0;

        bool _inConfigMode = false;
        
        //define variable
        static uint8_t _sunBrightness;
        static uint8_t _moonBrightness;

        RTC_DS3231 *_rtc;
        //HandleWebpage *_handleWebpage;
        static HandleDisplay *_handleDisplay;
        static HandleEeprom *_handleEeprom;
        static HandleAudio *_handleAudio;
        HandleWebpage *_handleWebpage = nullptr;

        static int procent2brightness(int prozent);

        static uint16_t getTimeInMintuesPerDay(int hour, int minutes);
        static uint16_t getTimeInMinutesPerDay(uint16_t eeWakeUpTimeFormat);

        uint16_t createWakeUpTime(int hour, int minutes, TimeTypes timeType);
        static TimeTypes getWakeUpType(uint16_t eeWakeUpTimeFormat);
        String getWakeUpTypeAsStr(uint16_t eeWakeUpTimeFormat);
        
        static String getTimeStr(uint16_t eeWakeUpTimeFormat);
        static int getHour(uint16_t eeWakeUpTimeFormat);
        static int getMinutes(uint16_t eeWakeUpTimeFormat);
        static uint16_t _wakeArr[];
        static uint8_t _countOfWakeTimes;
        void sanityCheckOfWakeUpTimes();
        bool checkEETimeFormat(uint16_t eeWakeUpTimeFormat);  
        void setEnableDisplay();
};

#endif //HANDLE_MAIN_H_INCLUDE