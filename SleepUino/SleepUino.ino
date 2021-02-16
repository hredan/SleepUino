/*
  SleepUino
  
  SleepUINO is a DIY-Projekt based on an ESP8266 and real time clock DS3231 I2C, to create a Alarm clock.
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

#include <ESP8266WiFi.h>    //https://github.com/esp8266/Arduino
#include <JC_Button.h>      //https://github.com/JChristensen/JC_Button

#include <Wire.h>           //DS3231 RTC is using I2C for communication
#include "RTClib.h"         //https://github.com/adafruit/RTClib

#include "Config.h"
#include "mainFunctionality.h"

//declaration of needed instances
MainFunc *mainFunc;

Button myBtn(BUTTON_PIN); //D3 -> GPIO0
   
void setup()
{
    Serial.begin(115200);
    Serial.print("\n");
    
    //init little filesystem
    MainFunc::startLittleFileSystem();

    //start button functionality
    myBtn.begin();
    
    //in operation mode wifi have to be off
    WiFi.mode(WIFI_OFF);
    
    //I2C Bus used for real time clock and display
    Wire.begin(SDA_PIN, SLC_PIN); 
    //Without the delay real time clock can not initilized successfully, seems to be that the I2C initializing needs a bit time (300 ms) 
    delay(300);
    
    //initialize needed instances    
    mainFunc = new MainFunc();
    mainFunc->startRealTimeClock();

    //set pin mode for LED PINs
    pinMode(LED_SUN_PIN, OUTPUT);
    pinMode(LED_MOON_PIN, OUTPUT);


    //init EEPROM
    mainFunc->setupEeprom();

    // if DNSServer is started with "*" for domain name, it will reply with
    // provided IP to all DNS request
    dnsServer.start(DNS_PORT, "*", apIP);
}

void loop()
{
    static states_t STATE = OPS;    // current state machine state, with static this variable will be initialzied only one time
    //microTime = millis();           // record the current time
    myBtn.read();                   // read the button

    switch (STATE)
    {
        // this state watches for short and long presses, switches the LED for
        // short presses, and moves to the TO_BLINK state for long presses.
        case OPS:
            if (myBtn.wasReleased())
            {
                Serial.println("State = Ops, enable Display");
                mainFunc->buttonReleasedInOpsState();
            }
            else if (myBtn.pressedFor(LONG_PRESS))
            {
                STATE = TO_WIFI;
                Serial.println("Change State: OPS -> TO_WIFI");
            }
            else
            {                
                mainFunc->updateDisplay();
            }
            break;

        // this is a transition state where we start the fast blink as feedback to the user,
        // but we also need to wait for the user to release the button, i.e. end the
        // long press, before moving to the BLINK state.
        case TO_OPS:
            if (myBtn.wasReleased())
            {
                Serial.println("disable WiFi");
                WiFi.mode(WIFI_OFF);
                mainFunc->setInConfigMode(false);
                mainFunc->storeValuesInEeprom();
                mainFunc->updateDisplay(true);
                Serial.println("Change State: TO_OPS -> OPS"); 
                STATE = OPS;
            }
            else
            {
                digitalWrite(LED_SUN_PIN, LOW);
                digitalWrite(LED_MOON_PIN, LOW);
                mainFunc->setWlanStatusOnDisplay(false);
            }
            break;

        // the fast-blink state. Watch for another long press which will cause us to
        // turn the LED off (as feedback to the user) and move to the TO_ONOFF state.
        case WIFI:
            if (myBtn.pressedFor(LONG_PRESS))
            {
                STATE = TO_OPS;
                Serial.println("Change State: WIFI -> TO_OPS");            
            }
            else
            {   
                if (!mainFunc->isSoundRunning())
                {
                    dnsServer.processNextRequest();
                    mainFunc->webpageCheckClient();                                                               
                }
                mainFunc->updateDisplay();
            }
            break;

        // this is a transition state where we just wait for the user to release the button
        // before moving back to the ONOFF state.
        case TO_WIFI:
            if (myBtn.wasReleased())
            {
                STATE = WIFI;
                Serial.println("Change State: TO_WIFI -> WIFI");
                Serial.println("enable WiFi");
                WiFi.mode(WIFI_AP);
                WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
                WiFi.softAP("SleepUino");               
                mainFunc->setInConfigMode(true);
                mainFunc->updateDisplay(true);
            }
            else
            {
                digitalWrite(LED_SUN_PIN, HIGH);
                digitalWrite(LED_MOON_PIN, HIGH);
                mainFunc->setWlanStatusOnDisplay(true);
            }
            break;
    }
}
