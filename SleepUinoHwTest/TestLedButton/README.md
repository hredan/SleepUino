# TestLedButton

`TestLedButton` is a hardware test sketch for the SleepUino button and the two LEDs.

It uses the `JC_Button` library and the same GPIO mapping as the main SleepUino project.

## Purpose

This test verifies:

- the button wiring and debounce behavior
- both LEDs can be switched individually
- long-press handling works as expected

## Hardware Mapping

- Button: `BUTTON_PIN` = GPIO0 (`D3`)
- Sun LED: `LED_SUN_PIN` = GPIO5 (`D1`)
- Moon LED: `LED_MOON_PIN` = GPIO4 (`D2`)

## Button Behavior

- Short press: toggles the LEDs so one is ON and the other is OFF
- Long press: toggles both LEDs OFF; if both are already OFF, they turn ON again
- After a long press, short presses continue the alternating ON/OFF pattern

## Test Output

The sketch prints the current LED state and the detected action to Serial at `115200` baud.

## Required Library

- `JC_Button`

## Notes

- The button uses the same `JC_Button` handling as the main SleepUino project.
- GPIO0 is a boot strap pin on ESP8266, so make sure the button wiring keeps the pin in the expected state during boot.
