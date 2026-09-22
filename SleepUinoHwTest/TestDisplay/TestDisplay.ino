/*

  U8g2Logo.ino

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

*/

#include <Arduino.h>
#include <LittleFS.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// End of constructor list

//#define MINI_LOGO

// I2C SLC D6, SDA D7 Used for Real Time Clock and Display
const int SLC_PIN = 12;
const int SDA_PIN = 13;

bool drawBmpFromLittleFS(const char *path, int x, int y) {
  if (!LittleFS.exists(path)) {
    Serial.printf("BMP not found: %s\n", path);
    return false;
  }

  File bmpFile = LittleFS.open(path, "r");
  if (!bmpFile) {
    Serial.printf("Could not open BMP: %s\n", path);
    return false;
  }

  if (bmpFile.size() < 54) {
    Serial.printf("BMP too small: %s\n", path);
    bmpFile.close();
    return false;
  }

  uint8_t header[54];
  if (bmpFile.read(header, sizeof(header)) != sizeof(header)) {
    Serial.printf("Could not read BMP header: %s\n", path);
    bmpFile.close();
    return false;
  }

  if (header[0] != 'B' || header[1] != 'M') {
    Serial.printf("Not a BMP file: %s\n", path);
    bmpFile.close();
    return false;
  }

  uint32_t offset = (uint32_t)header[10] |
                    ((uint32_t)header[11] << 8) |
                    ((uint32_t)header[12] << 16) |
                    ((uint32_t)header[13] << 24);

  uint32_t width = (uint32_t)header[18] |
                   ((uint32_t)header[19] << 8) |
                   ((uint32_t)header[20] << 16) |
                   ((uint32_t)header[21] << 24);

  uint32_t height = (uint32_t)header[22] |
                    ((uint32_t)header[23] << 8) |
                    ((uint32_t)header[24] << 16) |
                    ((uint32_t)header[25] << 24);

  uint16_t bitsPerPixel = (uint16_t)header[28] | ((uint16_t)header[29] << 8);
  uint32_t compression = (uint32_t)header[30] |
                         ((uint32_t)header[31] << 8) |
                         ((uint32_t)header[32] << 16) |
                         ((uint32_t)header[33] << 24);

  if (bitsPerPixel != 1 || compression != 0) {
    Serial.printf("Unsupported BMP format for %s: %u bits, compression=%lu\n", path, bitsPerPixel, (unsigned long)compression);
    bmpFile.close();
    return false;
  }

  if (width == 0 || height == 0 || width > 128 || height > 64) {
    Serial.printf("Unexpected BMP size: %s (%lux%lu)\n", path, (unsigned long)width, (unsigned long)height);
    bmpFile.close();
    return false;
  }

  uint32_t rowStride = ((width + 31) / 32) * 4;
  uint32_t pixelDataSize = rowStride * height;

  if (offset > (uint32_t)bmpFile.size() || pixelDataSize > (uint32_t)bmpFile.size() - offset) {
    Serial.printf("BMP data truncated: %s\n", path);
    bmpFile.close();
    return false;
  }

  bmpFile.seek(offset, SeekSet);

  uint8_t rowBuffer[128];
  for (uint32_t row = 0; row < height; ++row) {
    if (bmpFile.read(rowBuffer, rowStride) != rowStride) {
      Serial.printf("Could not read row %lu from %s\n", (unsigned long)row, path);
      bmpFile.close();
      return false;
    }

    int32_t displayY = y + (int32_t)height - 1 - (int32_t)row;

    for (uint32_t col = 0; col < width; ++col) {
      uint32_t byteIndex = col / 8;
      uint8_t bitMask = 0x80 >> (col % 8);
      bool pixelOn = (rowBuffer[byteIndex] & bitMask) != 0;

      if (!pixelOn) {
        continue;
      }

      int32_t displayX = x + (int32_t)col;
      if (displayX >= 0 && displayX < (int32_t)u8g2.getDisplayWidth() &&
          displayY >= 0 && displayY < (int32_t)u8g2.getDisplayHeight()) {
        u8g2.setDrawColor(1);
        u8g2.drawPixel(displayX, displayY);
      }
    }
  }

  bmpFile.close();
  return true;
}

void drawLogo(void)
{
    u8g2.setFontMode(1);	// Transparent
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(0, 30, "U");
    
    u8g2.setFontDirection(1);
    u8g2.setFont(u8g2_font_inb30_mn);
    u8g2.drawStr(21,8,"8");
        
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(51,30,"g");
    u8g2.drawStr(67,30,"\xb2");
    
    u8g2.drawHLine(2, 35, 47);
    u8g2.drawHLine(3, 36, 47);
    u8g2.drawVLine(45, 32, 12);
    u8g2.drawVLine(46, 33, 12);
}

void drawURL(void)
{
  u8g2.setFont(u8g2_font_4x6_tr);
  u8g2.drawStr(1,54,"github.com/olikraus/u8g2");
}

void setup(void) {
  //pinMode(9, OUTPUT);
  //digitalWrite(9, 0);	// default output in I2C mode for the SSD1306 test shield: set the i2c adr to 0

  Serial.begin(115200);

  // I2C Bus used for real time clock and display
  Wire.begin(SDA_PIN, SLC_PIN);
  // Without the delay real time clock can not initilized successfully, seems to
  // be that the I2C initializing needs a bit time (300 ms)
  delay(300);

  u8g2.begin();

  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }

  Serial.println("LittleFS mounted");
}

void loop(void) {
  u8g2.clearBuffer();
  drawBmpFromLittleFS("/Logo_SleepUino_128x64.bmp", 0, 0);
  u8g2.sendBuffer();
  delay(3000);
  u8g2.clearBuffer();
  drawLogo();
  drawURL();
  u8g2.sendBuffer();
  delay(3000);
}


