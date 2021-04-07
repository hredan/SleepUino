![SleepUino DIY Alarm-Clock with webinterface](/SleepUino/data/Logo_en.png)
# SleepUino the DIY Alarm-Clock with Web-Interface based on ESP8266
The SleepUino is a DIY Alarm-Clock with a Web-Interface based on an ESP8266. It is using jQuery Mobile as Web UI. From this point of view, it can be configured by any Wi-Fi device, like Android/iOS smartphone or a PC and Notebook with WLAN. The SleepUino is using the WLAN only for configuration (Nobody will have an access point beside the bed, I hope so!). It is generating its own WLAN access point, which you can use to connect with your smartphone or PC. Because the SleepUino is using a Single Web Application and a Captive Portal it is quite easy to connect with it. More details you can find on my German Web Page: 
<a href="https://www.sleepuino.sourcecode3d.de/" target="_blank">SleepUino</a>

# Why a Web-Interface and jQuery Mobile for an Alarm-Clock?
The Web-Interface give me the possibility to get a lot of different configurations (**this is also true for many other devices**). And a Web-Interface can be used by any WLAN device independently of the operating system. The ESP8266 is a low-cost micro controller, so the Web-Interface is bargain at that price! It is quite easy to configure an Alarm-Clock by a Web-Interface as with many buttons on the Alarm-Clock itself. The SleepUino has only one button and together with the Web-Interface it is fully enough to use it. [jQuery Mobile](https://jquerymobile.com/): *"is a user interface system designed to make responsive web sites and apps that are accessible on all smartphone, tablet and desktop devices"*. jQuery Mobile saves me a lot of time to get a single, responsive Web-Application, that is why I am using jQuery Mobile. According to the motte: *"Don't reinvent the wheel!"*. 😉

# Why two LEDs for Sun and Moon
If I am starting this journey, I want experimenting with the ESP8266 and I needed a project to do this. That is why I come up with an idea of an Alarm Clock for my children with two LEDs. One of it (Moon) to say: "Do not wake me up! It is in the middle of the night, look at the clock, turn around and continue to sleep". And the other one (Sun) to say: "Ok you can stand up". Many parents will know this problem, because the children cannot read the time and it is normal that we are waking up at the night, but we know it is in the middle of the night, the children do not, until now 😉.

# Basics of programming and technologies 
I want to do this with a Web-Interface because I want many configuration possibilities and it should be work with every device. I thought this should be possible with an ESP8266 and I learned a lot about:
* [ESP8266/Arduino](https://github.com/esp8266/Arduino)
* Java Script and HTML Request
* Dynamic Webpages
* A [Captive portal](https://en.wikipedia.org/wiki/Captive_portal) and how I can realize it with the [ESP8266/Arduino](https://github.com/esp8266/Arduino) library.
* Server/Client Communication with help of Json -> [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* How to play music and sounds with an ESP8266 -> see [ESP8266Audio](https://github.com/earlephilhower/ESP8266Audio)
* I have found an amazing graphic library that supports many monochrome I2C displays -> [U8g2 (by oliver)](https://github.com/olikraus/u8g2)
* I used the library [JC_Button](https://github.com/JChristensen/JC_Button) to have more possibilities for my only one button.

At the end I say it is not only an alarm-clock for children it could be a training for basic programming skills and technologies.
I have more than 20 years of experience as programmer. I have written many code in C++, C#, Ruby and Python. This project can give you basic skills of object-oriented programming, how you can fix compiler errors, how you can test the code and how you can use logging with Arduino.

# A clock for adults?
During the developing of the housing, I have used silver glitter paper. With different Needles I pierced a star constellation (Orion) in the paper. I am very satisfied with the result because the paper itself is a little bit transparent. It looks great. Currently I am trying to use an acrylic glass as diffuser and an image printed on a inkjet folie to get a nice light emitting image. This is also interesting for me and other adults, because we do not want to know what time is it in the night ("Oh no 2 o'clock in the morning, why I am waking up at this time?? I cannot go back to sleep. 😒"). If we wake up in the night, we want to know if we can continue the sleep or we have to stand up, we do not want to know what time it is. An image or in my case the stars we can connect with a positive, relaxing feeling. In your case it could be a photo of your sleeping child, nothing is more calming than happy, sleeping children. Or a photo of you sweetheart. The design of the housing is up to you! If you have a good design, you can share it with us, maybe you can inspire us. 

# A video said more than miles of words

# Required Hardware and Circuit Diagrams
1. ESP8266 Board with 4MB Flash Memory
2. DAC [MAX98357A](https://www.adafruit.com/product/3006)
3. Speaker 4 Ω or 8 Ω (Tip: have a look if you have an old device like a radio or an old notebook which you do not need anymore, then you can use the speaker from it)
4. DS3231 Real Time Clock
5. I2C OLED Display
6. Any Button (I have used an [Arcade Button](https://amzn.to/3aItnSU) for the "Garbage Collection Housing")
7. 2x LED
8. 2x 220 Ω resistor
9. some cable
10. Breadboard

## Circuit Diagrams
![SleepUino D1 mini Circuit Diagram](/CircuitDiagram/Images/SleepUino_ESP_Mini_Schaltplan.jpg)

[SleepUino D1 mini Circuit Diagram as pdf](/CircuitDiagram/SleepUino_ESP_D1_Mini_CircuitDiagram.pdf)

[SleepUino NodeMCU Circuit Diagram as pdf](/CircuitDiagram/SleepUino_ESP_NodeMCU_V3_CircuitDiagram.pdf)

## Breadboard Connecting
![SleepUino ESP WeMos D1 mini](/CircuitDiagram/Images/SleepUino_ESP_Mini_Breadboard.jpg)

[SleepUino D1 mini Breadboard Overview as pdf](/CircuitDiagram/SleepUino_ESP_D1_Mini_MiniBreadboard.pdf)

[SleepUino NodeMCU Breadboard Overview as pdf](/CircuitDiagram/SleepUino_ESP_NodeMCU_V3_Breadboard.pdf)

# Housing Design is up to you, here are two examples of my housings

## Plywood Housing
This housing is work in progress. Currently the button is missing and the reflectors for the LEDs are not implemented. I have some ideas, but I have no time to realizing it until now:

![Plywood Housing](/HousingExample/Plywood/PlywoodHousing_s.png)

## My Prototype Housing called "*Garbage Collection*"
Garbage Collection is an algorithm in some programming languages to find occupied memory that is not used from a program, to release it.
In case of the housing, I have used some things from the waste. Seems to be it is no waste and we can reuse it. 😀
For the housing itself I have used a container for olive from the market. And for the LED reflectors I have used two yogurt pots.

![ConfigMode GarbageCollection](/HousingExample/GarbageCollection/ConfigMode.png)
![GarbageCollection Open](/HousingExample/GarbageCollection/GC_Open.png)
![GarbageCollection Build Up Front](/HousingExample/GarbageCollection/BuildUpFront.png)

# Installation of Arduino and flashing of ESP8266
After you have build-up the hardware according the circuit diagram you can flash the ESP8266 to use the SleepUino.
1. [Install the Arduino Desktop IDE](https://www.arduino.cc/en/Guide)
2. [Add ESP8266 Arduino as Additional Board Manager](https://arduino-esp8266.readthedocs.io/en/2.7.4_a/installing.html)
    1. Start Arduino and open Preferences window.
    2. Enter https://arduino.esp8266.com/stable/package_esp8266com_index.json into Additional Board Manager URLs field. You can add multiple URLs, separating them with commas.
3. Now you can select an ESP Board and you can find Examples from ESP8266 Arduino Core. This is some of the basic things of the Arduino IDE. For every library you have installed, you can find examples under File -> Examples, also for ESP8266 Arduino Core
4. Install the following libraries in Tools -> Manage Libraries...
    1. [ESP8266Audio](https://github.com/earlephilhower/ESP8266Audio)
    2. [U8g2 (by oliver)](https://github.com/olikraus/u8g2)
    3. [JC_Button](https://github.com/JChristensen/JC_Button)
    4. [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
4. Copy or Clone the SleepUino Repository
    1. if you familiar with GIT you can clone the Repository from GitHub
    2. or download the ZIP File by pressing the TAP Code in [GitHub SleepUINO](https://github.com/hredan/SleepUino) -> Download ZIP
5. Unpack the ZIP on your PC
6. SleepUino is using the ESP8266 file system [LittleFS](https://arduino-esp8266.readthedocs.io/en/2.7.4_a/filesystem.html#littlefs-file-system-limitations). To Uploading files to the ESP8266 you must integrate the [ESP8266FS tool in the Arduino IDE](https://arduino-esp8266.readthedocs.io/en/2.7.4_a/filesystem.html#uploading-files-to-file-system)
7. Select the correct type of ESP8266 board you are using
    1. Arduino IDE -> Tools -> Board -> ESP8266 Board -> select e.g. WeMos D1 or Node MCU (depends on what you bought)
    2. CPU Frequency: "160 MHz"
    3. Flash Size: "4MB (FS: 2MB)"
    4. IwIP Variant: "v2 Higher Bandwidth"
8. Open the SleepUino.ino in Arduino IDE
9. Upload the Webpage Data to the ESP8266 with Arduino IDE -> Tools -> ESP8266 LittleFS Data Upload
10. Upload the program code by Arduino IDE -> Sketch -> Upload
    1. The Arduino IDE will be compiling the sketch, this needs some minutes
    2. The compiler Warning: *library JC_Button claims to run on avr architecture(s) and may be incompatible with your current board which runs on esp8266 architecture(s)* can be ignored.
    3. After compiling the code will be uploaded to the ESP8266
11. Now you can try if it is working. At first you can start the configuration mode by pressing the SleepUino button for a longer time. When the WLAN Symbol is shown in the SleepUino Display, the SleepUino is in the Config Mode.
12. If you have some trouble use the Serial Monitor

## Optional: Visual Studio Code with Arduino Extension as IDE
The Arduino IDE brings all things you need to compile the code and upload it to the ESP8266. For me I have missing some things to develop the code efficiently. That is why I am try out Visual Studio Code. Visual Studio Code comes up with many Plugins for many different things. For me, the navigation through the code is quite easy and supported by VS Code. I could integrate a Live Server this makes it extremely comfortable to developing the HTML and Java Script part of SleepUino. I have used the Microsoft C/C++ extension for testing some C++ functions before I have used it on ESP8266. All these things I could be use in one IDE and this was immensely helpful and makes it very efficiently to developing the code of SleepUino. If you are interested, I can share my experience and setup in more detail with you.

# Currently Features
- with a long button pressure, you will start the configuration mode, the SleepUino access point is starting and you can connecting you WLAN device with SleepUino
- In configuration Mode
  - you can synchronize the time by pressing one button in the UI
  - you can change the brightness of the LEDs
  - The Display can be enabled or disabled. Following option are available:
    - is on the whole time
    - is off the whole time
    - is off during bed time
    - is off during day time
  - you can change the volume of alarm signal
  - you set how many times the alarm signal shout be repeated
  - or you decide the alarm signal shout be repeated until you are pressing the button
  - you can define until five sleeping periods
  - for every period you can decide if the alarm signal shout be played or not
- if the display is disabled in operation mode, you can active the display for a short time with a short press if the button
- all settings are stored persistent on the ESP8266, after a power cut the settings are still available.
- Related to the RTC with a coin cell plugged in, you can keep the time also if the main power is lost.
- for my kids the display is disabled in the night, but 30 minutes before wakeing up in the morning, the display goes on. At the moment this cannot be configured and it is hard coded in Config.h

```c++
const int DISPLAY_TIME_BEFORE = 30;
```



# WLAN without password protection! My security point of view:
* the WLAN is only available if you configure the SleepUino
* the webserver used by SleepUino can only handle one connection with a client
* because the SleepUino is used as access point, it has not internet connection, no data can be shared with third parties
* so, yes there is a little chance, that any other is connecting with the SleepUino at the time when it is in configuration mode
* It is also possible to set a password for the SleepUino access point in the code, currently a password is not used. I see no dire necessity to do this because a password complicates the currently easy connecting process. If you have another opinion share the arguments with me or change the code local for you, it is open source, you can change anything.

## If you feel better, use a password
It is up to you, if you want a password or another SSID you can change the following line in the File SleepUino.ino:
```c++
WiFi.softAP("SleepUino");
``` 
change it to:
```c++
WiFi.softAP("SleepUino", "passwordYouWant");
```
[Link to ESP8266 Arduino Core Documentation #SoftAp](https://arduino-esp8266.readthedocs.io/en/2.7.4_a/esp8266wifi/soft-access-point-class.html#softap)

# Disclaimer
All this code is released under the GPL, and all of it is to be used at your own risk. If you find any bugs, please let me know via the GitHub issue tracker or drop me an email ([hredan@sourcecode3d.de](mailto:hredan@sourcecode3d.de)).

# Thanks
Many thanks to the people, who releasing the following libraries as open source:
* [ESP8266/Arduino](https://github.com/esp8266/Arduino)
* [ESP8266Audio](https://github.com/earlephilhower/ESP8266Audio)
* [U8g2 (by oliver)](https://github.com/olikraus/u8g2)
* [JC_Button](https://github.com/JChristensen/JC_Button)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)

Without these libraries SleepUino would not be possible. Thank you.


