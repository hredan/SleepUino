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

#ifndef HANDLE_AUDIO_H_INCLUDED
#define HANDLE_AUDIO_H_INCLUDED


#define USE_I2S_DAC //else I2SNoDAC
//#define USE_I2S_WORKAROUND

//Audio functionality comes from:
//https://github.com/earlephilhower/ESP8266Audio

#include "AudioFileSourceLittleFS.h"
#include "AudioGeneratorWAV.h"
#ifdef USE_I2S_DAC
  #include <I2S.h>
  #include "AudioOutputI2S.h"
#else
  #include "AudioOutputI2SNoDAC.h"
#endif

class HandleAudio{
  using CallBackSoundIsDone = void (*) ();
  
  public:
    HandleAudio();
    void playSound();
    void stopSound();
    uint8_t getGain();
    void setGain(uint8_t gainValue);
    bool isSoundPlaying();
    void setCallBackSoundIsDone(CallBackSoundIsDone callBack);
  
  private:
    bool _soundIsPlaying;
    uint8_t _gainSound;
    CallBackSoundIsDone _callBackSoundIsDone = nullptr;
  	AudioGeneratorWAV *_wav;
    AudioFileSourceLittleFS *_file;
    #ifdef USE_I2S_DAC
      AudioOutputI2S *_out;
    #else
      AudioOutputI2SNoDAC *out;
    #endif
};


#endif // HANDLE_AUDIO_H_INCLUDED
