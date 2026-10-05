#include <Arduino.h>
#include <JC_Button.h>
#include "config.h"

const int LONG_PRESS = 1000;

Button button(BUTTON_GPIO);

bool shortPatternSunOn = true;
bool overrideAllOn = false;
bool overrideAllOff = false;
bool longPressHandled = false;

void applyLedState() {
  bool sunOn = false;
  bool moonOn = false;

  if (overrideAllOn) {
    sunOn = true;
    moonOn = true;
  } else if (!overrideAllOff) {
    sunOn = shortPatternSunOn;
    moonOn = !shortPatternSunOn;
  }

  digitalWrite(LED_SUN_GPIO, sunOn ? HIGH : LOW);
  digitalWrite(LED_MOON_GPIO, moonOn ? HIGH : LOW);

  Serial.printf("LED Sun: %s, LED Moon: %s\n", sunOn ? "ON" : "OFF", moonOn ? "ON" : "OFF");
}

void toggleShortPattern() {
  shortPatternSunOn = !shortPatternSunOn;
  overrideAllOn = false;
  overrideAllOff = false;
  applyLedState();
  Serial.println("Short press: toggled LED pattern");
}

void toggleLongPattern() {
  if (overrideAllOff) {
    overrideAllOff = false;
    overrideAllOn = true;
    Serial.println("Long press: all LEDs ON");
  } else {
    overrideAllOn = false;
    overrideAllOff = true;
    Serial.println("Long press: all LEDs OFF");
  }
  applyLedState();
}

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(LED_SUN_GPIO, OUTPUT);
  pinMode(LED_MOON_GPIO, OUTPUT);

  button.begin();

  Serial.println();
  Serial.println("SleepUino TestLedButton");
  Serial.println("Short press: alternate LEDs");
  Serial.println("Long press: toggle all OFF / all ON");

  applyLedState();
}

void loop() {
  button.read();

  if (button.isPressed() && !longPressHandled && button.pressedFor(LONG_PRESS)) {
    toggleLongPattern();
    longPressHandled = true;
  }

  if (button.wasReleased()) {
    if (!longPressHandled) {
      toggleShortPattern();
    }
    longPressHandled = false;
  }
}
