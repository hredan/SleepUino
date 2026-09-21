#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>

#include "AudioFileSourceLittleFS.h"
#include "AudioGeneratorMOD.h"
#include "enigma.h"
#include "AudioOutputI2S.h"

// musiklinjen.mod sample from the mod archive: https://modarchive.org/index.php?request=view_by_moduleid&query=42146

AudioGeneratorMOD *mod;
AudioFileSourceLittleFS *file;
AudioOutputI2S *out;
bool reportedDone;
bool wasStarted;
float currentGain;

void startPlayback() {
  if (mod == nullptr || out == nullptr) {
    Serial.printf("Audio engine not initialized\n");
    return;
  }

  if (mod->isRunning()) {
    Serial.printf("Playback already running\n");
    return;
  }

  if (file != nullptr) {
    delete file;
    file = nullptr;
  }

  file = new AudioFileSourceLittleFS("/musiklinjen.mod");
  if (!mod->begin(file, out)) {
    Serial.printf("Failed to start playback\n");
    return;
  }

  reportedDone = false;
  wasStarted = true;
  Serial.printf("Playback started\n");
}

void stopPlayback() {
  if (mod != nullptr && mod->isRunning()) {
    mod->stop();
    Serial.printf("Playback stopped\n");
    return;
  }

  Serial.printf("Playback is not running\n");
}

void setGainCommand(const String &command) {
  int separatorIndex = command.indexOf(' ');
  if (separatorIndex < 0) {
    Serial.printf("Usage: gain <0.0-4.0>\n");
    return;
  }

  String gainValueText = command.substring(separatorIndex + 1);
  gainValueText.trim();
  if (gainValueText.length() == 0) {
    Serial.printf("Usage: gain <0.0-4.0>\n");
    return;
  }

  float newGain = gainValueText.toFloat();
  if (newGain < 0.0f || newGain > 4.0f) {
    Serial.printf("Gain out of range: %s\n", gainValueText.c_str());
    Serial.printf("Usage: gain <0.0-4.0>\n");
    return;
  }

  currentGain = newGain;
  out->SetGain(currentGain);
  Serial.printf("Gain set to %.2f\n", currentGain);
}

void handleSerialCommands() {
  static String command;

  while (Serial.available() > 0) {
    char inChar = (char)Serial.read();

    if (inChar == '\n' || inChar == '\r') {
      command.trim();
      command.toLowerCase();

      if (command == "start") {
        startPlayback();
      } else if (command == "stop") {
        stopPlayback();
      } else if (command.startsWith("gain")) {
        setGainCommand(command);
      } else if (command.length() > 0) {
        Serial.printf("Unknown command: %s\n", command.c_str());
        Serial.printf("Use: start | stop | gain <0.0-4.0>\n");
      }

      command = "";
    } else {
      command += inChar;
    }
  }
}

void setup() {
  WiFi.mode(WIFI_OFF);
  Serial.begin(115200);
  delay(1000);
  if (!LittleFS.begin()) {
    Serial.printf("LittleFS mount failed\n");
    return;
  }

  Serial.printf("\nSleepUinoHwTest\n");
  Serial.printf("TestI2cDac\n");
  Serial.printf("Type 'start' to play, 'stop' to stop, and 'gain <0.0-4.0>' to set volume\n");
  Serial.printf("Hint: 'stop' is not working! Conflict with I2S GPIOs\n");
  Serial.printf("Workaround: use 'ESP Reset' button of ESPEasyFlasher Serial Monitor\n");

  audioLogger = &Serial;
  file = nullptr;
  out = new AudioOutputI2S();
  currentGain = 1.0f;
  out->SetGain(currentGain);
  // out->SetPinout(0, 1, 2); // Set the pinout if needed
  mod = new AudioGeneratorMOD();
  reportedDone = false;
  wasStarted = false;
}

void loop() {
  handleSerialCommands();

  if (mod != nullptr && mod->isRunning()) {
    if (!mod->loop()) {
      mod->stop();
    }
  } else {
    if (wasStarted && !reportedDone) {
      Serial.printf("MOD done\n");
      reportedDone = true;
    }
  }
}
