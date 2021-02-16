/*
  handleWebpage
  
  handleWebpage is used by SleepUino.ino. It is used to handle a webinterface for configuration of SleepUino.
  
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

#ifndef HANDLEWEBPAGE_H_INCLUDED
#define HANDLEWEBPAGE_H_INCLUDED

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "LittleFS.h"
#include <ArduinoJson.h>

const byte DNS_PORT = 53;
IPAddress apIP(172, 217, 28, 1);
DNSServer dnsServer;

class HandleWebpage
{
    //type aliasing
    //C++ version of: typedef void (*InputEvent)(const char*)
    using CallBackGetTime = DateTime (*)();
    using CallBackAdjustTime = void (*) (const DateTime&);
    using CallBackSetGain = void (*) (uint8_t gainValue);
    using CallBackSetMoonBrightness = void (*) (uint8_t moonBrightness);
    using CallBackSetSunBrightness = void (*) (uint8_t sunBrightness);

    using CallBackSetSoundReplay = void (*) (uint8_t soundReplay);
    using CallBackSetDisplayMode = void (*) (uint8_t displayMode);

    using CallBackGetGain = uint8_t (*)();
    using CallBackGetMoonBrightness = uint8_t (*)();
    using CallBackGetSunBrightness = uint8_t (*)();
    using CallBackGetDisplayMode = uint8_t (*)();
    using CallBackGetSoundReplays = uint8_t (*)();
    
    using CallBackPlaySound = void (*)();

    using CallBackWakeTimeData = void(*)(uint8_t size, uint16_t dataArr[]);

    using CallBackGetWakeTimeJson = String(*) ();
  
    public:
        HandleWebpage();
        void setupHandleWebpage();
        
        void handleClient();
        static void sendSuccess();
        
        //CallBacks
        void setCallBackGetTime(CallBackGetTime callBackGetTime);
        void setCallBackAdjustTime(CallBackAdjustTime callBackAdjustTime);
        void setCallBackSetSoundReplay(CallBackSetSoundReplay callBackSetSoundReplay);
        void setCallBackSetDisplayMode(CallBackSetDisplayMode callBackSetDisplayMode);
        
        void setCallBackSetGain(CallBackSetGain callBackSetGain);
        void setCallBackSetMoonBrightness(CallBackSetMoonBrightness callBackSetMoonBrightness);
        void setCallBackSetSunBrightness(CallBackSetSunBrightness callBackSetSunBrightness);

        void setCallBackGetGain(CallBackGetGain callBackGetGain);
        void setCallBackGetMoonBrightness(CallBackGetMoonBrightness callBackGetMoonBrightness);
        void setCallBackGetSunBrightness(CallBackGetSunBrightness callBacketSunBrightness);
        void setCallBackGetDisplayMode(CallBackGetDisplayMode callBackGetDisplayMode);
        void setCallBackGetSoundReplays(CallBackGetSoundReplays callBackGetSoundReplays);

        void setCallBackPlaySound(CallBackPlaySound callBackPlaySound);
        
        void setCallBackWakeTimeData(CallBackWakeTimeData callBackWakeTimeData);
        
        void setCallBackGetWakeTimeJson(CallBackGetWakeTimeJson callBackGetWakeTimeJson);


    private:
        CallBackGetTime _callBackGetTime = nullptr;
        CallBackAdjustTime _callBackAdjustTime = nullptr;
        CallBackSetGain _callBackSetGain = nullptr;
        CallBackSetMoonBrightness _callBackSetMoonBrightness = nullptr;
        CallBackSetSunBrightness _callBackSetSunBrightness = nullptr;
        CallBackSetSoundReplay _callBackSetSoundReplay = nullptr;
        CallBackSetDisplayMode _callBackSetDisplayMode = nullptr;

        CallBackGetMoonBrightness _callBackGetMoonBrightness = nullptr;
        CallBackGetSunBrightness _callBackGetSunBrightness = nullptr;
        CallBackGetGain _callBackGetGain = nullptr;
        CallBackGetDisplayMode _callBackGetDisplayMode = nullptr;
        CallBackGetSoundReplays _callBackGetSoundReplays = nullptr;

        CallBackPlaySound _callBackPlaySound = nullptr;
        CallBackWakeTimeData _callBackWakeTimeData = nullptr;

        CallBackGetWakeTimeJson _callBackGetWakeTimeJson = nullptr;    

        void handleRoot();
        void handleWebRequests();

        void handleGetValues();
        void handleGetTime();
        void handleSyncTime();
        void setLEDMoon();
        void setSoundReplay();
        void setDisplayMode();
        void setLEDSun();
        void handleSetGain();
        void handleSetWakeData();

        void handleGetWakeTimeData();

        void handlePlaySound();

        bool loadFromLittleFS(String path);

        void setLED(int pin);
        static ESP8266WebServer *_webServer;
        RTC_DS3231 *_rtc;        
};
#endif // HANDLEWEBPAGE_H_INCLUDED