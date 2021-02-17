![SleepUino DIY Alarm-Clock with webinterface](/SleepUino/data/Logo_en.png)
# SleepUino the DIY Alarm-Clock with Web-Interface
The SleepUino is a DIY Alarm-Clock with a Web-Interface based on a ESP8266. It is using jQuery Mobile as Web UI. From this point of view it can be configured by any WiFi device, link Android/iOS mobile phones or a PC and Notebook with WLAN. The SleepUino is using the WLAN only for configuration (Nobody will have an access point beside the bed, I hope it!). It is generating his own WLAN access point, which you can use to connect with you mobile phone or PC. Because the SleepUino is using a Single Web Application and a Captive Portal it is very easy to connecting with it. More details you can find on my German Web Page: [SleepUino](https://www.sleepuino.sourcecode3d.de/){:target="_blank"}

# Why a Web-Interface for a Alarm-Clock
The Web-Interface give me the possibility to get a lot of different configurations (this is also true for many other devices). And a Web-Interface can be used by any WLAN device independently of the operating system. The ESP8266 is a low cost micro controller, so the Web-Interface is bargain at that price! It is very easy to configure a Alarm-Clock by a Web-Interface as with many buttons on the device self. The SleepUino has only one button and this is fully enough to use it.

# A video said more than milles of words

# Currently Features
- The Display can be enabled or disabled in the configuration. Flowing option are available:
  - is on the whole time
  - is off the whole time
  - is off during bed time
  - is off during day time
- with a long button pressure you will start the configuration mode, the SleepUino access point is starting and you can connecting you WLAN device with SleepUino for configuration
- In confuration Mode

# WLAN without password protection! My security point of view:
* the WLAN is only available if you configure the SleepUino
* the webserver used by SleepUino can only handle one connection with a client
* so, yes there is a little chance, that any other is connecting with the SleepUino at the time when it is in configuration mode
* It is also possible to set a password for the Sleep Uino access point in the code, currently a password is not used. I see no dire necessity to do this because a password complicates the currently easy connecting process. If you have an other opinion share the arguments with me or change the code local for you.

# Required Hardware
1. ESP8266 Board with 4MB Flash Memory
2. DAC [MAX98357A](https://www.adafruit.com/product/3006)
3. Speaker 4 Ω or 8 Ω (Tip: have a look if you have an old device like a radio or an old notebook which you do not need anymore, then you can use the speaker from it)
4. DS3231 Real Time Clock
5. I2C OLED Display
6. Any Button
7. 2x LED
8. 2x 220 Ω resistor
9. some cable
10. Breadboard

# Disclaimer
All this code is released under the GPL, and all of it is to be used at your own risk. If you find any bugs, please let me know via the GitHub issue tracker or drop me an email ([hredan@sourcecode3d.de](mailto:hredan@sourcecode3d.de)).


