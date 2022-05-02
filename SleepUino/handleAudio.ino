/*
  handleAudio

  handleAudio is used by SleepUino to handle the alarm sound.
  It is using the library ESP8266Audio by earlephilhower https://github.com/earlephilhower/ESP8266Audio.

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

#include "handleAudio.h"

HandleAudio::HandleAudio() {
  _soundIsPlaying = false;
  audioLogger = &Serial;
#ifdef USE_I2S_DAC
  _out = new AudioOutputI2S();

#ifdef USE_I2S_WORKAROUND
  i2s_end();
#endif
#else
  HandleAudio::_out = new AudioOutputI2SNoDAC();
#endif
  _wav = new AudioGeneratorWAV();
}

void HandleAudio::setCallBackSoundIsDone(CallBackSoundIsDone callBack) {
  _callBackSoundIsDone = callBack;
}

uint8_t HandleAudio::getGain() { return HandleAudio::_gainSound; }

void HandleAudio::setGain(uint8_t gainValue) {
  HandleAudio::_gainSound = gainValue;
}

void HandleAudio::stopSound() {
  if (_wav->isRunning()) {
    _wav->stop();
    _soundIsPlaying = false;
    Serial.printf("WAV stoped\n");
  }
#ifdef USE_I2S_WORKAROUND
  i2s_end();
#endif
}

bool HandleAudio::isSoundPlaying() {
  if (_wav->isRunning()) {
    if (!_wav->loop()) {
      _wav->stop();
      _soundIsPlaying = false;
      Serial.printf("WAV done\n");

      if (_callBackSoundIsDone != nullptr) {
        _callBackSoundIsDone();
        // handleWebpage->sendSuccess();
      }
#ifdef USE_I2S_WORKAROUND
      i2s_end();
#endif
    }
  }
  return _soundIsPlaying;
}

void HandleAudio::playSound() {
  const char *soundFile = "/AlarmSound_16bit.wav";
  if (LittleFS.exists(soundFile)) {
    Serial.println("initialize all instances, needed for sound");
#ifdef USE_I2S_DAC
    // max gain value < 1
    float gain = (_gainSound - 1) * 0.01;
    _out->SetGain(gain);
    Serial.println("use gain to play sound: " + String(gain));

#ifdef USE_I2S_WORKAROUND
    i2s_begin();
#endif  // USE_I2S_WORKAROUND
#endif

    Serial.println("load file");
    _file = new AudioFileSourceLittleFS(soundFile);
    Serial.println("play sound");
    if (_wav->begin(_file, _out)) {
      _soundIsPlaying = true;
    }
  } else {
    Serial.printf("Sound File (%s) does not exists!", soundFile);
  }
}
