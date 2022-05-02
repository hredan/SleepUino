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

#include "handleWebpage.h"
#include "Config.h"

ESP8266WebServer* HandleWebpage::_webServer = nullptr;

HandleWebpage::HandleWebpage(RTC_DS3231* rtc) {
  _rtc = rtc;
  _webServer = new ESP8266WebServer(80);
}

void HandleWebpage::handleRoot() {  // When URI / is requested, send a web page with a button to toggle the LED
  handleWebRequests();
}

/* void handleRoot(){
  webServer.sendHeader("Location", "/index.html",true);   //Redirect to our html web page
  webServer.send(302, "text/plane","");
} */

void HandleWebpage::setCallBackGetTime(CallBackGetTime callBackGetTime) { _callBackGetTime = callBackGetTime; }

void HandleWebpage::setCallBackAdjustTime(CallBackAdjustTime callBackAdjustTime) {
  _callBackAdjustTime = callBackAdjustTime;
}

void HandleWebpage::setCallBackSetGain(CallBackSetGain callBackSetGain) { _callBackSetGain = callBackSetGain; }

void HandleWebpage::setCallBackSetMoonBrightness(CallBackSetMoonBrightness callBackSetMoonBrightness) {
  _callBackSetMoonBrightness = callBackSetMoonBrightness;
}

void HandleWebpage::setCallBackSetSunBrightness(CallBackSetSunBrightness callBackSetSunBrightness) {
  _callBackSetSunBrightness = callBackSetSunBrightness;
}

void HandleWebpage::setCallBackSetSoundReplay(CallBackSetSoundReplay callBackSetSoundReplay) {
  _callBackSetSoundReplay = callBackSetSoundReplay;
}

void HandleWebpage::setCallBackSetDisplayMode(CallBackSetDisplayMode callBackSetDisplayMode) {
  _callBackSetDisplayMode = callBackSetDisplayMode;
}

void HandleWebpage::setCallBackGetGain(CallBackGetGain callBackGetGain) { _callBackGetGain = callBackGetGain; }

void HandleWebpage::setCallBackGetMoonBrightness(CallBackGetMoonBrightness callBackGetMoonBrightness) {
  _callBackGetMoonBrightness = callBackGetMoonBrightness;
}

void HandleWebpage::setCallBackGetSunBrightness(CallBackGetSunBrightness callBackGetSunBrightness) {
  _callBackGetSunBrightness = callBackGetSunBrightness;
}

void HandleWebpage::setCallBackGetDisplayMode(CallBackGetDisplayMode callBackGetDisplayMode) {
  _callBackGetDisplayMode = callBackGetDisplayMode;
}

void HandleWebpage::setCallBackGetSoundReplays(CallBackGetSoundReplays callBackGetSoundReplays) {
  _callBackGetSoundReplays = callBackGetSoundReplays;
}

void HandleWebpage::setCallBackPlaySound(CallBackPlaySound callBackPlaySound) {
  _callBackPlaySound = callBackPlaySound;
}

void HandleWebpage::setCallBackWakeTimeData(CallBackWakeTimeData callBackWakeTimeData) {
  _callBackWakeTimeData = callBackWakeTimeData;
}

void HandleWebpage::setCallBackGetWakeTimeJson(CallBackGetWakeTimeJson callBackGetWakeTimeJson) {
  _callBackGetWakeTimeJson = callBackGetWakeTimeJson;
}

void HandleWebpage::handleSetGain() {
  Serial.println("handleSetGain: " + _webServer->arg("plain"));
  if (_callBackSetGain != nullptr) {
    if (_webServer->hasArg("gain")) {
      int gainSound = _webServer->arg("gain").toInt();
      if (_callBackSetGain != nullptr) {
        _callBackSetGain(gainSound);
      } else {
        Serial.println("Error: _callBackSetGain == nullptr");
      }

      _webServer->send(200, "text/plane", "{\"success\": true}");
    } else {
      Serial.println("Error handleSetGain: missing argument gain!");
      _webServer->send(200, "text/plane", "{\"success\": false}");
    }
  } else {
    Serial.println("Error: _callBackSetGain == nullptr");
    _webServer->send(200, "text/plane", "{\"success\": false}");
  }
}

void HandleWebpage::handleSetWakeData() {
  Serial.println("handleSetWakeData: " + _webServer->arg("plain"));

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, _webServer->arg("plain"));

  if (doc.containsKey("size") && doc.containsKey("dataArr")) {
    uint8_t size = doc["size"];
    uint16_t dataArr[MAX_WAKE_TIMES];

    for (int i = 0; i < size; i++) {
      dataArr[i] = doc["dataArr"][i];
      Serial.printf("dataArr[%d] = %d\n", i, dataArr[i]);
    }

    if (_callBackWakeTimeData != nullptr) {
      _callBackWakeTimeData(size, dataArr);
      _webServer->send(200, "text/plane", "{\"success\": true}");
    } else {
      Serial.println("Error handleSetWakeData: _callBackWakeTimeData == nullptr!");
      _webServer->send(200, "text/plane", "{\"success\": false}");
    }

  } else {
    Serial.println("Error handleSetWakeData: missing argument dataArr and size!");
    _webServer->send(200, "text/plane", "{\"success\": false}");
  }
  // if(_webServer->hasArg("dataArr"))
  // {
  //   const String dataStr = _webServer->arg("dataArr");
  //   Serial.println("handleSetWakeData: DataArr -> " + dataStr);
  //   _webServer->send(200, "text/plane", "{\"success\": true}");
  // }
  // else
  // {
  //   Serial.println("Error handleSetWakeData: missing argument dataArr!");
  //   _webServer->send(200, "text/plane", "{\"success\": false}");
  // }
}

void HandleWebpage::handleSyncTime() {
  Serial.println("handleSyncTime: " + _webServer->arg("plain"));

  uint16_t year = _webServer->arg("year").toInt();
  // convert javasript month->0-11 to 1-12
  uint8_t month = _webServer->arg("month").toInt() + 1;
  uint8_t day = _webServer->arg("day").toInt();
  uint8_t hours = _webServer->arg("hours").toInt();
  uint8_t minutes = _webServer->arg("minutes").toInt();
  uint8_t seconds = _webServer->arg("seconds").toInt();

  DateTime receivedTime = DateTime(year, month, day, hours, minutes, seconds);
  if (!receivedTime.isValid()) {
    _webServer->send(400, "DateTime not valid!");
    Serial.println("Error: DateTime not valid!");
    return;
  }

  if (_callBackAdjustTime != nullptr) {
    Serial.printf("set time to: %02d.%02d.%d %02d:%02d:%02d\n", receivedTime.day(), receivedTime.month(),
                  receivedTime.year(), receivedTime.hour(), receivedTime.minute(), receivedTime.second());
    Serial.println("start time adjustment");
    if (_rtc != nullptr) {
      _rtc->adjust(receivedTime);
    } else {
      _webServer->send(404, "text/plane", "Error: _rtc not initialized!");
      Serial.println("Error: _rtc == nullptr");
    }
    Serial.println("set time successfully");

    _webServer->send(200, "text/plane", "{\"success\": true}");
  } else {
    _webServer->send(404, "text/plane", "Error: Callback not available.");
    Serial.println("Error: _callBackAdjustTime == nullptr");
  }
}

void HandleWebpage::setLED(int pin) {
  String strBrightness = _webServer->arg("plain");
  Serial.println(strBrightness);

  int brightness = strBrightness.toInt();

  if (pin == LED_MOON_PIN) {
    if (_callBackSetMoonBrightness != nullptr) {
      Serial.printf("setLEDMoon: %d\n", brightness);
      _callBackSetMoonBrightness(brightness);
    } else {
      Serial.println("Error: callBackSetMoonBrightness == nullptr");
    }
  } else {
    if (_callBackSetSunBrightness != nullptr) {
      Serial.printf("setLEDMoon: %d\n", brightness);
      _callBackSetSunBrightness(brightness);
    } else {
      Serial.println("Error: _callBackSetSunBrightness == nullptr");
    }
  }
  _webServer->send(202, "text/plane", "");
}

void HandleWebpage::setDisplayMode() {
  String strDisplayMode = _webServer->arg("plain");
  Serial.println("SetDisplayMode: " + strDisplayMode);

  int displayMode = strDisplayMode.toInt();
  if (_callBackSetDisplayMode != nullptr) {
    _callBackSetDisplayMode(displayMode);
  } else {
    Serial.println("Error: _callBackSetDisplayMode == nullptr");
  }
}

void HandleWebpage::setSoundReplay() {
  String strSoundReplay = _webServer->arg("plain");
  Serial.println("SetSoundReplay: " + strSoundReplay);

  int soundReplay = strSoundReplay.toInt();
  if (_callBackSetSoundReplay != nullptr) {
    _callBackSetSoundReplay(soundReplay);
  } else {
    Serial.println("Error: _callBackSetSoundReplay == nullptr");
  }
}

void HandleWebpage::setLEDMoon() { setLED(LED_MOON_PIN); }

void HandleWebpage::setLEDSun() { setLED(LED_SUN_PIN); }

void HandleWebpage::handlePlaySound() {
  Serial.println("handlePlaySound" + _webServer->arg("plain"));
  if (_callBackPlaySound != nullptr) {
    _callBackPlaySound();
  } else {
    Serial.println("Error: _callBackPlaySound ==nullptr");
    _webServer->send(200, "text/plane", "{\"success\": false}");
  }
}

void HandleWebpage::handleGetValues() {
  String strMoonValue = "0";
  if (_callBackGetMoonBrightness != nullptr) {
    strMoonValue = String(_callBackGetMoonBrightness());
  } else {
    Serial.println("Error: _callBackGetMoonBrightness == null");
  }

  String strSunValue = "0";
  if (_callBackGetSunBrightness != nullptr) {
    strSunValue = String(_callBackGetSunBrightness());
  } else {
    Serial.println("Error: _callBackGetSunBrightness == null");
  }

  String strGainSound = "0";
  if (_callBackGetGain != nullptr) {
    strGainSound = String(_callBackGetGain());
  } else {
    Serial.println("Error: _callBackGetGainSound == null");
  }

  String strDisplayMode = "0";
  if (_callBackGetDisplayMode != nullptr) {
    strDisplayMode = String(_callBackGetDisplayMode());
  } else {
    Serial.println("Error: _callBackGetDisplayMode == null");
  }

  String strSoundReplay = "0";
  if (_callBackGetSoundReplays != nullptr) {
    uint8_t soundReplays = _callBackGetSoundReplays();
    Serial.println("Sound Replays:" + soundReplays);
    strSoundReplay = String(soundReplays);
  } else {
    Serial.println("Error: _callBackGetSoundReplays == null");
  }

  String jsonAnswer = "{\"moon\": " + strMoonValue + ", \"sun\": " + strSunValue + ", \"gain\": " + strGainSound +
                      ",\"displayMode\": " + strDisplayMode + ",\"soundReplay\": " + strSoundReplay + "}";
  Serial.println("handleGetValues :send  value: " + jsonAnswer);
  _webServer->send(200, "text/plane", jsonAnswer);
}

void HandleWebpage::handleGetTime() {
  if (_callBackGetTime != nullptr) {
    DateTime now = _callBackGetTime();
    char buf[MAX_GET_TIME_BUFFER_SIZE];
    // month() - 1 to be compatible with javascript month -> 0-11
    snprintf(buf, MAX_GET_TIME_BUFFER_SIZE,
            "{\"year\": %d, \"month\": %d, \"day\": %d, \"hours\": %d, \"minutes\": %d, \"seconds\": %d, "
            "\"milliseconds\": %d}",
            now.year(), now.month() - 1, now.day(), now.hour(), now.minute(), now.second(), 0);

    String jsonAnswer = String(buf);
    Serial.println("Send time: " + jsonAnswer);
    _webServer->send(200, "text/plane", jsonAnswer);
  } else {
    _webServer->send(404, "text/plane", "Error: Callback not available.");
    Serial.println("Error: _callBackGetTime==nullptr");
  }
}

void HandleWebpage::handleGetWakeTimeData() {
  if (_callBackGetWakeTimeJson != nullptr) {
    String wakeTimeJson = _callBackGetWakeTimeJson();
    Serial.println("Send WakeTimeData: " + wakeTimeJson);
    _webServer->send(200, "text/plane", wakeTimeJson);
  } else {
    _webServer->send(404, "text/plane", "Error: Callback not available.");
    Serial.println("Error: _callBackGetWakeTimeJson==nullptr");
  }
}

void HandleWebpage::handleWebRequests() {
  if (!loadFromLittleFS(_webServer->uri())) {
    Serial.println("Error: handleWebRequests");
    String message = "File Not Detected\n\n";
    message += "URI: ";
    message += _webServer->uri();
    message += "\nMethod: ";
    message += (_webServer->method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += _webServer->args();
    message += "\n";
    for (uint8_t i = 0; i < _webServer->args(); i++) {
      message += " NAME:" + _webServer->argName(i) + "\n VALUE:" + _webServer->arg(i) + "\n";
    }
    _webServer->send(404, "text/plain", message);
    // Serial.println(message);
  }
}

bool HandleWebpage::loadFromLittleFS(String path) {
  bool returnValue = true;
  Serial.println("Load path: " + path);
  String dataType = "text/plain";

  if (path.endsWith("/")) path += "index.html";

  if (path.endsWith(".src"))
    path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".html"))
    dataType = "text/html";
  else if (path.endsWith(".htm"))
    dataType = "text/html";
  else if (path.endsWith(".css"))
    dataType = "text/css";
  else if (path.endsWith(".js"))
    dataType = "application/javascript";
  else if (path.endsWith(".png"))
    dataType = "image/png";
  else if (path.endsWith(".gif"))
    dataType = "image/gif";
  else if (path.endsWith(".jpg"))
    dataType = "image/jpeg";
  else if (path.endsWith(".ico"))
    dataType = "image/x-icon";
  else if (path.endsWith(".xml"))
    dataType = "text/xml";
  else if (path.endsWith(".pdf"))
    dataType = "application/pdf";
  else if (path.endsWith(".zip"))
    dataType = "application/zip";
  if (LittleFS.exists(path)) {
    File dataFile = LittleFS.open(path.c_str(), "r");
    if (_webServer->hasArg("download")) dataType = "application/octet-stream";

    if (_webServer->streamFile(dataFile, dataType) != dataFile.size()) {
      // Serial.println("Error: streamed file has different size!");
      // returnValue = false;
    }
    dataFile.close();
  } else {
    Serial.println("Error: Path does not exist and will be redirect to root:");
    Serial.println(path);
    returnValue = loadFromLittleFS("/");
  }
  return returnValue;
}

void HandleWebpage::setupHandleWebpage() {
  // replay to all requests with same HTML
  _webServer->onNotFound(std::bind(&HandleWebpage::handleWebRequests, this));

  // replay to all requests with same HTML
  // Information about using std::bind -> https://github.com/esp8266/Arduino/issues/1711

  _webServer->on("/", HTTP_GET, std::bind(&HandleWebpage::handleRoot, this));
  _webServer->on("/getValues", HTTP_GET, std::bind(&HandleWebpage::handleGetValues, this));
  _webServer->on("/getTime", HTTP_GET, std::bind(&HandleWebpage::handleGetTime, this));
  _webServer->on("/playSound", HTTP_GET, std::bind(&HandleWebpage::handlePlaySound, this));
  _webServer->on("/setDisplayMode", HTTP_POST, std::bind(&HandleWebpage::setDisplayMode, this));
  _webServer->on("/setSoundReplay", HTTP_POST, std::bind(&HandleWebpage::setSoundReplay, this));
  _webServer->on("/setLEDMoon", HTTP_POST, std::bind(&HandleWebpage::setLEDMoon, this));
  _webServer->on("/setLEDSun", HTTP_POST, std::bind(&HandleWebpage::setLEDSun, this));
  _webServer->on("/syncTime", HTTP_POST, std::bind(&HandleWebpage::handleSyncTime, this));
  _webServer->on("/setGain", HTTP_POST, std::bind(&HandleWebpage::handleSetGain, this));
  _webServer->on("/setWakeData", HTTP_POST, std::bind(&HandleWebpage::handleSetWakeData, this));
  _webServer->on("/getWakeTimeData", HTTP_GET, std::bind(&HandleWebpage::handleGetWakeTimeData, this));

  // webServer.on("/config/changed", HTTP_POST, configChanged);
  _webServer->begin();
}

void HandleWebpage::sendSuccess() { _webServer->send(200, "text/plane", "{\"success\": true}"); }

void HandleWebpage::handleClient() { _webServer->handleClient(); }
