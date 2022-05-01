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

#include "mainFunctionality.h"

MainFunc::MainFunc()
{
    _rtc = new RTC_DS3231();
    MainFunc::_handleDisplay = new HandleDisplay();
    MainFunc::_handleDisplay->begin();

    MainFunc::_handleEeprom = new HandleEeprom();
    HandleWebpage *_handleWebpage = new HandleWebpage(_rtc);;
    
    MainFunc::_handleAudio = new HandleAudio();
    
    //set Callbacks
    MainFunc::_handleAudio->setCallBackSoundIsDone(_handleWebpage->sendSuccess);
    //set Callbacks
    
    _handleWebpage->setCallBackGetTime(std::bind(&RTC_DS3231::now, _rtc));
    _handleWebpage->setCallBackAdjustTime(std::bind(&RTC_DS3231::adjust, _rtc, std::placeholders::_1));
    
    _handleWebpage->setCallBackSetGain(this->setGainSound);
    _handleWebpage->setCallBackSetSunBrightness(this->setSunBrightness);
    _handleWebpage->setCallBackSetMoonBrightness(this->setMoonBrightness);
    _handleWebpage->setCallBackSetSoundReplay(this->setSoundReplay);
    _handleWebpage->setCallBackSetDisplayMode(this->setDisplayMode);

    _handleWebpage->setCallBackGetGain(this->getGainSound);
    _handleWebpage->setCallBackGetMoonBrightness(this->getMoonBrightness);
    _handleWebpage->setCallBackGetSunBrightness(this->getSunBrightness);
    _handleWebpage->setCallBackGetDisplayMode(this->getDisplayMode);
    _handleWebpage->setCallBackGetSoundReplays(this->getSoundReplayes);
    
    _handleWebpage->setCallBackPlaySound(this->playSound);
    _handleWebpage->setCallBackWakeTimeData(this->setWakeTimeData);
    _handleWebpage->setCallBackGetWakeTimeJson(this->getWakeTimeDataJsonStr);

    _handleWebpage->setupHandleWebpage();

    //check and read Wake Times in the EEPROM
    sanityCheckOfWakeUpTimes();
    
    Serial.printf("MainFunc:Number of stored wake times: %d\n", MainFunc::_countOfWakeTimes);
    bool setAlarm = false;
    for(int i=0; i < MainFunc::_countOfWakeTimes; i++)
    {
        uint16_t eeTimeFormat = _wakeArr[i];
        int hour = getHour(eeTimeFormat);
        int minute = getMinutes(eeTimeFormat);
        String wakeTypeStr = getWakeUpTypeAsStr(eeTimeFormat); 
        Serial.printf("%d\ttime %02d:%02d %s\n", i + 1, hour, minute, wakeTypeStr.c_str());
        
        //Show clock on display, if alarm is on
        TimeTypes ttype = MainFunc::getWakeUpType(eeTimeFormat);
        if (ttype == TT_SUN_ALARM)
        {
            setAlarm = true;
        }
    }
    
    //do not update the display at this time, set only the flag
    _handleDisplay->setAlarmStatus(setAlarm, false);
};

void MainFunc::setInConfigMode(bool value)
{
    _inConfigMode = value;
}

void MainFunc::buttonReleasedInOpsState()
{
    this->enableDisplayShort();
    if (_alarm)
    {
        _alarm = false;
        _handleAudio->stopSound();
    }
}

void MainFunc::enableDisplayShort()
{
    if (_handleDisplay->getEnableDisplay() == false)
    {
        _enableDisplayTimer = true;
        _enableDisplayTime = millis()+3000;
        updateDisplay(true);
    }
}

void MainFunc::setEnableDisplay()
{
    Serial.printf("Enter: setEnableDisplay TimeType: %d\n", _currentTimeType);
    bool enableDisplay = false;
    if (_displayOn && (_displayOnTime < _now))
    {
        enableDisplay = true;
    }
    else
    {
        switch (_handleEeprom->getDisplayMode())
        {
        case DM_OFF:
            enableDisplay = false;
            break;
        case DM_ON:
            enableDisplay = true;
            break;
        case DM_ON_AT_BEDTIME:
            if (_currentTimeType == TT_MOON)
            {
                enableDisplay = true;
            }
            else
            {
                enableDisplay = false;
            }
            break;
        case DM_ON_AT_DAYTIME:
            if (_currentTimeType == TT_SUN || _currentTimeType == TT_SUN_ALARM)
            {
                enableDisplay = true;
            }
            else
            {
                enableDisplay = false;
            }
            break;
        default:
            enableDisplay = true;
            break;
        }
    }
    
    _handleDisplay->setEnableDisplay(enableDisplay, true);
    Serial.printf("Leave setEnableDisplay: enableDisplay = %s\n", enableDisplay ? "true" : "false");
}

HandleEeprom* MainFunc::_handleEeprom = nullptr;
HandleAudio* MainFunc::_handleAudio = nullptr;
HandleDisplay* MainFunc::_handleDisplay = nullptr;

uint8_t MainFunc::_countOfWakeTimes = 0;
uint16_t MainFunc::_wakeArr[MAX_WAKE_TIMES];

void MainFunc::setWlanStatusOnDisplay(bool wlan)
{
    _handleDisplay->setWlanStatus(wlan);
}

void MainFunc::startRealTimeClock()
{
    while (!_rtc->begin()) {
        Serial.println("Error: RTC could not be started!");
        delay(300);
    }

    DateTime now = _rtc->now();
    char time_format[25] = "DD.MM.YYYY, hh:mm:ss";
    Serial.printf("RTC: %s, temp: %f °C\n", now.toString(time_format), _rtc->getTemperature());
}

void MainFunc::startLittleFileSystem()
{
    if (LittleFS.begin())
  {
    Serial.print("LittleFS started successfully\n");
    FSInfo fs_info;
    LittleFS.info(fs_info);
    Serial.printf("FS total Bytes %d\n", fs_info.totalBytes);
    Serial.printf("FS used Bytes %d\n", fs_info.usedBytes);
  }
  else
  {
    Serial.print("Error: Could not start LittleFS!\n");
  }
}

String MainFunc::getWakeTimeDataJsonStr()
{
    String jsonStr = ""; //empty json object
    int start = 0;
    if (MainFunc::_countOfWakeTimes > 0)
    {
        //check type of first entry
        uint16_t firstEntry = _wakeArr[0];
        TimeTypes firstEntry_ttype = MainFunc::getWakeUpType(firstEntry);
        if (firstEntry_ttype == TT_MOON)
        {
            start = 1;
        }

        int jsonArrIndex = 0;
        DynamicJsonDocument doc(1024);

        for (int i=start; i < MainFunc::_countOfWakeTimes; i = i + 2)
        {
            bool alarm = false;

            uint16_t entry1 = _wakeArr[i];
            String wakeUpTime = MainFunc::getTimeStr(entry1);
            
            if (MainFunc::getWakeUpType(entry1) == TT_SUN_ALARM)
            {
                alarm = true;
            }

           
            doc["wakeTimes"][jsonArrIndex]["getUp"] = wakeUpTime;
            
            //special case
            //if last entry is reached and first entry was moon, put first entry to the end as goBedTime
            if (i== (MainFunc::_countOfWakeTimes - 1) && start == 1)
            {
                String goBedTime = MainFunc::getTimeStr(firstEntry);
                doc["wakeTimes"][jsonArrIndex]["goToBed"] = goBedTime;
            }
            else
            {
                uint16_t entry2 = _wakeArr[i+1];
                String goBedTime = MainFunc::getTimeStr(entry2);
                doc["wakeTimes"][jsonArrIndex]["goToBed"] = goBedTime;
            }
            
            doc["wakeTimes"][jsonArrIndex]["alarm"] = alarm;
            
            jsonArrIndex++;
        }

        serializeJson(doc, jsonStr);
    }
    return jsonStr;
}


void MainFunc::setWakeTimeData(uint8_t size, uint16_t dataArr[])
{
    MainFunc::_countOfWakeTimes = size;
    Serial.printf("setWakeTimeData: size = %d\n", size);
    bool setAlarm = false;
    for (int i=0; i<size; i++)
    {
        MainFunc::_wakeArr[i] = dataArr[i];
        Serial.printf("setWakeTimeData: fill _wakeArr[%d]=%d\n", i, dataArr[i]);

        //check if alarm is on, if on show it on display
        TimeTypes ttype = MainFunc::getWakeUpType(dataArr[i]);
        Serial.printf("setWakeTimeData: ttype=%d", ttype);
        if (ttype == TT_SUN_ALARM)
        {
            setAlarm = true;
        }
    }

    _handleDisplay->setAlarmStatus(setAlarm);
    _handleEeprom->writeWakeUpTimes(dataArr, size);
}

void MainFunc::storeValuesInEeprom()
{
    _handleEeprom->commitValues();
}

void MainFunc::webpageCheckClient()
{
    _handleWebpage->handleClient();
}

bool MainFunc::isSoundRunning()
{
    return _handleAudio->isSoundPlaying();
}

uint8_t MainFunc::_sunBrightness = 0;
uint8_t MainFunc::_moonBrightness = 0;

void MainFunc::playSound()
{
    _handleAudio->playSound();
}

void MainFunc::setupEeprom()
{
  MainFunc::_sunBrightness = _handleEeprom->getSunBrightnessValue();
  MainFunc::_moonBrightness = _handleEeprom->getMoonBrightnessValue();
  uint8_t gainValue = _handleEeprom->getGainValue();
  DisplayMode displayMode = (DisplayMode) _handleEeprom->getDisplayMode();
  uint8_t soundReplays = _handleEeprom->getSoundReplay();
  
  if (soundReplays > MAX_SOUND_REPLAYS)
  {
      soundReplays = 3;
      _handleEeprom->setSoundReplay(soundReplays);
      Serial.printf("Sound Replays out of rage set to %d\n", soundReplays);
  }
  else
  {
      Serial.printf("Sound Replays: %d\n", soundReplays);
  }

  if((displayMode != DM_ON) && (displayMode != DM_OFF) && (displayMode != DM_ON_AT_BEDTIME) && (displayMode != DM_ON_AT_DAYTIME))
  {
      _handleEeprom->setDisplayMode((uint8_t) DM_ON);
      Serial.printf("Display Mode out of rage (%d) set to %d\n", displayMode, DM_ON);
  }
  else
  {
      Serial.printf("Display Mode: %d\n", displayMode);
  }

  if(gainValue < GAIN_MIN || gainValue > GAIN_MAX)
  {
    gainValue = 10;
    _handleEeprom->setGainValue(gainValue);
    Serial.printf("Gain sound out of rage set to %d\n", gainValue);
  }
  else
  {
    Serial.printf("Gain sound: %d\n", gainValue);

  }
  _handleAudio->setGain(gainValue);

  if(MainFunc::_sunBrightness < BRIGHTNESS_MIN || MainFunc::_sunBrightness > BRIGHTNESS_MAX)
  {
    MainFunc::_sunBrightness = BRIGHTNESS_MAX;
    _handleEeprom->setSunBrightnessValue(MainFunc::_sunBrightness);
    Serial.printf("Sun brightness out of rage set to %d\n", MainFunc::_sunBrightness);
  }
  else
  {
    Serial.printf("Sun brightness value: %d\n", MainFunc::_sunBrightness);
  }
  
  if(MainFunc::_moonBrightness < BRIGHTNESS_MIN || MainFunc::_moonBrightness > BRIGHTNESS_MAX)
  {
    MainFunc::_moonBrightness = BRIGHTNESS_MAX;
    _handleEeprom->setMoonBrightnessValue(MainFunc::_moonBrightness);
    Serial.printf("Moon brightness out of rage set to %d\n", MainFunc::_moonBrightness);
  }
  else
  {
    Serial.printf("Moon brightness value: %d\n", MainFunc::_moonBrightness);
  }

  _handleEeprom->commitValues();
}

void MainFunc::setSunBrightness(uint8_t sunBrightness)
{
    MainFunc::_sunBrightness = sunBrightness;
    analogWrite(LED_SUN_PIN, procent2brightness(MainFunc::_sunBrightness));
    _handleEeprom->setSunBrightnessValue(MainFunc::_sunBrightness);
}
void MainFunc::setMoonBrightness(uint8_t moonBrightness)
{
    MainFunc::_moonBrightness = moonBrightness;
    analogWrite(LED_MOON_PIN, procent2brightness(MainFunc::_moonBrightness));
    _handleEeprom->setMoonBrightnessValue(MainFunc::_moonBrightness);
}

void MainFunc::setDisplayMode(uint8_t displayMode)
{
    _handleEeprom->setDisplayMode(displayMode);
}

void MainFunc::setSoundReplay(uint8_t soundReplay)
{
    _handleEeprom->setSoundReplay(soundReplay);
}

void MainFunc::setGainSound(uint8_t gainSound){
    _handleAudio->setGain(gainSound);
    _handleEeprom->setGainValue(gainSound);
}

uint8_t MainFunc::getSunBrightness()
{
    return MainFunc::_sunBrightness;
}

uint8_t MainFunc::getMoonBrightness()
{
    return MainFunc::_moonBrightness;
}

uint8_t MainFunc::getGainSound()
{
    return _handleAudio->getGain();
}

uint8_t MainFunc::getDisplayMode()
{
    return _handleEeprom->getDisplayMode();
}

uint8_t MainFunc::getSoundReplayes()
{
    return _handleEeprom->getSoundReplay();
}

int MainFunc::procent2brightness(int prozent)
{
  
  return (int)(prozent * 10.23);
}

bool MainFunc::checkEETimeFormat(uint16_t eeWakeUpTimeFormat)
{
    bool resultValue = true;

    int hour = getHour(eeWakeUpTimeFormat);
    if (resultValue && ((hour < 0) || (hour > 23)))
    {
        resultValue = false;
    }

    int minute = getMinutes(eeWakeUpTimeFormat);
    if (resultValue && ((minute < 0) || (minute > 59)))
    {
        resultValue = false;
    }

    TimeTypes wakeTime = getWakeUpType(eeWakeUpTimeFormat);
    if (resultValue && ((wakeTime != TT_SUN) || (wakeTime != TT_MOON) || wakeTime != TT_SUN_ALARM))
    {
        resultValue = false;
    }
    return resultValue;
}

void MainFunc::sanityCheckOfWakeUpTimes()
{
    bool testSuccessful = true;
    uint8_t count = _handleEeprom->getNumberOfWakeTimes();
   
    if (count <= MAX_WAKE_TIMES)
    {
        uint8_t lastEEWakeTime = 0;  
        _handleEeprom->readWakeTimes(count, _wakeArr);
        for(int i=0; i < count; i++)
        {
            uint16_t eeWakeTimeValue = _wakeArr[i];
            if (checkEETimeFormat(eeWakeTimeValue))
            {
                Serial.println("Error: checkEETimeFormat failed!");
                testSuccessful = false;
                break;
            }

            uint16_t currentEEWakeTime = getTimeInMinutesPerDay(eeWakeTimeValue);
            if (i > 0)
            {
                if (lastEEWakeTime > currentEEWakeTime)
                {
                    Serial.println("Error: wrong order of wake times!");
                    testSuccessful = false;
                    break;
                }
            }
            lastEEWakeTime = currentEEWakeTime;
        }
    }
    else
    {
        Serial.println("Error: wakeCount does not contain a valid value");
        testSuccessful = false;
    }

    if (testSuccessful == false)
    {
        Serial.println("Error: Sanitycheck of wakeTimes failed, set count value to 0!");
        MainFunc::_countOfWakeTimes = 0;
        _handleEeprom->setWakeCountToZero();
    }
    else
    {
        //read Data
        MainFunc::_countOfWakeTimes = count;
        _handleEeprom->readWakeTimes(MainFunc::_countOfWakeTimes, MainFunc::_wakeArr);
        Serial.println("Info: sanityCheckOfWakeUpTimes->pass");
    }
}

uint16_t MainFunc::getTimeInMintuesPerDay(int hour, int minutes)
{
    return (hour * 60 + minutes);
}

uint16_t MainFunc::createWakeUpTime(int hour, int minutes, TimeTypes timeType)
{
    uint16_t wakeUpTimeInMinutes = getTimeInMintuesPerDay(hour, minutes);
    uint16_t eeWakeUpTimeFormat = (wakeUpTimeInMinutes << 4) | timeType;
    return eeWakeUpTimeFormat;
}

uint16_t MainFunc::getTimeInMinutesPerDay(uint16_t eeWakeUpTimeFormat)
{
    return eeWakeUpTimeFormat >> 4;
}

TimeTypes MainFunc::getWakeUpType(uint16_t eeWakeUpTimeFormat)
{
    return (TimeTypes) (eeWakeUpTimeFormat & 0x000F);
}

String MainFunc::getWakeUpTypeAsStr(uint16_t eeWakeUpTimeFormat)
{
    TimeTypes wakeType = getWakeUpType(eeWakeUpTimeFormat);

    String returnValue = "";

    switch (wakeType)
    {
        case TT_SUN:
            returnValue = "sun";
            break;
        case TT_SUN_ALARM:
            returnValue = "sun_alarm";
            break;
        case TT_MOON:
            returnValue = "moon";
            break;
        default:
            returnValue = "unknown";
            break;
    }
    return returnValue;
}

String MainFunc::getTimeStr(uint16_t eeWakeUpTimeFormat)
{
    char time[6];
    int hour = MainFunc::getHour(eeWakeUpTimeFormat);
    int minute = MainFunc::getMinutes(eeWakeUpTimeFormat);

    sprintf(time, "%02d:%02d", hour, minute);
    return String(time);
}

int MainFunc::getHour(uint16_t eeWakeUpTimeFormat)
{
    uint16_t wakeUpTimeInMinutes = MainFunc::getTimeInMinutesPerDay(eeWakeUpTimeFormat);
    return wakeUpTimeInMinutes/60;
}

int MainFunc::getMinutes(uint16_t eeWakeUpTimeFormat)
{
    int hour = getHour(eeWakeUpTimeFormat);
    uint16_t wakeUpTimeInMinutes = MainFunc::getTimeInMinutesPerDay(eeWakeUpTimeFormat);
    return wakeUpTimeInMinutes - hour*60;
}

TimeTypes MainFunc::checkState(uint16_t currentTimeInMinutes)
{    
    TimeTypes currentState = TT_UNKNOWN;
    
    if (MainFunc::_countOfWakeTimes > 0 && (_inConfigMode != true))
    {
        //maxBorder = Time in Minutes of last wake entry
        uint16_t maxBorder = MainFunc::getTimeInMinutesPerDay(MainFunc::_wakeArr[MainFunc::_countOfWakeTimes-1]);

        //if current time >= maxBorder then currentState = last wake entry
        if (currentTimeInMinutes >= maxBorder)
        {
            currentState = MainFunc::getWakeUpType(MainFunc::_wakeArr[MainFunc::_countOfWakeTimes-1]);
            _indexOfNextWakeTime = 0;
        }
        else
        {
            //go through wakeArray
            for(int i = 0; i < (MainFunc::_countOfWakeTimes - 1); i++)
            {
                //get wakeTime and wakeType
                uint16_t wakeTime = MainFunc::getTimeInMinutesPerDay(MainFunc::_wakeArr[i]);               

                //if time of first entry > currentTime then currentState = state of last wakeTime entry
                if ((i ==0) && (currentTimeInMinutes < wakeTime))
                {
                    currentState = MainFunc::getWakeUpType(MainFunc::_wakeArr[MainFunc::_countOfWakeTimes-1]);
                    _indexOfNextWakeTime = 0;
                    break;
                }
                else //if ((i >= 0) && i <= (MainFunc::_countOfWakeTimes-1))
                {
                    //currentState = (currentTimeInMinutes >= wakeTime[i]) && (currentTimeInMinutes < nextWakeTime[i+1])
                    uint16_t nextWakeTime = MainFunc::getTimeInMinutesPerDay(MainFunc::_wakeArr[i+1]);
                    if (currentTimeInMinutes >= wakeTime && currentTimeInMinutes < nextWakeTime)
                    {
                        currentState = MainFunc::getWakeUpType(MainFunc::_wakeArr[i]);
                        _indexOfNextWakeTime = (i + 1); 
                        break;
                    }
                }
            }
        }
    }
    
    return currentState;
}

void MainFunc::checkDisplayOnBeforeWakeTime(DateTime currentTime)
{   
    uint16_t nextWakeUpTimeFormat = _wakeArr[_indexOfNextWakeTime];
    TimeTypes nextTimeType = getWakeUpType(nextWakeUpTimeFormat);

    //show Display 
    if (((nextTimeType == TT_SUN) || (nextTimeType == TT_SUN_ALARM)) && (_displayOn == false))
    {        
        //create nextWakeTime based on currentTime     
        uint16_t year = currentTime.year();
        uint8_t month = currentTime.month();
        uint8_t day = currentTime.day();
        
        //hour and minute comes from nextWakeTime
        uint8_t hour = getHour(nextWakeUpTimeFormat);
        uint8_t minute = getMinutes(nextWakeUpTimeFormat);

        DateTime *nextWakeTime = new DateTime(year, month, day, hour, minute);
        
        //if nextTime < now add 1 day (it is next morning)
        if (*nextWakeTime < currentTime)
        {
            const TimeSpan oneDay = TimeSpan(1, 0, 0 ,0);            
            DateTime nextWakeTimeNextMorning = *nextWakeTime + oneDay;
            nextWakeTime = &nextWakeTimeNextMorning;
        }

        TimeSpan timeSpanDisplayOn = TimeSpan(60 * DISPLAY_TIME_BEFORE);
        DateTime displayOnTime = *nextWakeTime - timeSpanDisplayOn;
        _displayOnTime = &displayOnTime;
        _displayOn = true;
        Serial.printf("DisplayOn: start %d:%d\n", _displayOnTime->hour(), _displayOnTime->minute());
    }
    this->setEnableDisplay();
}

void MainFunc::enableDisplayByButton()
{
    if (_enableDisplayTimer == true && _enableDisplayTime > _milliTime)
    {
        if (_handleDisplay->getEnableDisplay() != true)
        {
            Serial.println("Enable Display by Button");
            _handleDisplay->setEnableDisplay(true, true);
        }        
    }
    else if (_enableDisplayTimer == true && _enableDisplayTime < _milliTime)
    {
         Serial.println("Disable Display by Button");
        _enableDisplayTimer = false;
        this->setEnableDisplay();
    }
}

void MainFunc::updateDisplay()
{
    updateDisplay(false);
}
//update Display checks the time every 2 sec, if the time (hours or secondes) is different update the display 
void MainFunc::updateDisplay(bool force)
{
    _milliTime = millis();

    enableDisplayByButton();

    if (!_handleAudio->isSoundPlaying() && _alarm)
    {
        if (_alarmUntilPressButton || (_numOfAlarmReplays > 0))
        {
            _handleAudio->playSound();

            if (_alarmUntilPressButton == false)
            {
                _numOfAlarmReplays = _numOfAlarmReplays - 1;
                if (_numOfAlarmReplays == 0)
                {
                    _alarm = false;
                }
            }
        }
    }
    
    if(((_milliTime - _lastMicroTime) > 2000) || force)
    {
      _lastMicroTime = _milliTime;
      DateTime rtc_now = _rtc->now();
      _now = &rtc_now;
      uint8_t hour = _now->hour();
      uint8_t minute = _now->minute();
      
      //output should be used only for tests
      //Serial.printf("Read RTC %d:%d\n", hour, minute);
      if((_lastHour != hour) || (_lastMinute != minute) || force)
      {
        _lastHour = hour;
        _lastMinute = minute;

        uint16_t minutesPerDay = hour * 60 + minute;

        char timeStr[8];
        sprintf(timeStr, "%02d:%02d", hour, minute);
        Serial.printf("UpdateDisplay: time->%s\n", timeStr);

         //set the stored brightness on LEDs
        _currentTimeType = checkState(minutesPerDay);
        checkDisplayOnBeforeWakeTime(*_now);
        
        if (_currentTimeType == TT_SUN || _currentTimeType == TT_SUN_ALARM)
        {
            analogWrite(LED_SUN_PIN, procent2brightness(MainFunc::_sunBrightness));        
            digitalWrite(LED_MOON_PIN, LOW);    
        }
        else if (_currentTimeType == TT_MOON)
        {
            analogWrite(LED_MOON_PIN, procent2brightness(MainFunc::_moonBrightness));        
            digitalWrite(LED_SUN_PIN, LOW); 
        }
        else 
        {
            //if state unknown 
            analogWrite(LED_MOON_PIN, procent2brightness(MainFunc::_moonBrightness));
            analogWrite(LED_SUN_PIN, procent2brightness(MainFunc::_sunBrightness));  
        }

        //Action if state is changing
        if ((_currentTimeType != _lastTimeType))
        {
            setEnableDisplay();

            //play sound if switching to TT_SUN_ALARM
            //avoid playing alarm if _lastTimeType is TT_UNKNOW then SleepUino is starting or comes from Config Mode
            if ((_currentTimeType == TT_SUN_ALARM) && _lastTimeType != TT_UNKNOWN)
            {
                _alarm = true;
                _numOfAlarmReplays = _handleEeprom->getSoundReplay();
                if (_numOfAlarmReplays == 0)
                {
                    _alarmUntilPressButton = true;
                }
            }

            _lastTimeType = _currentTimeType;
            _displayOn = false;
        }
        _handleDisplay->setTime(timeStr);
      }
    }
}
