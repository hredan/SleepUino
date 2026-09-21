# TestRtc

This sketch is a simple hardware test for the **SleepUino** using a **DS3231 Real-Time Clock (RTC)** on an **ESP8266** board (for example, a D1 mini).

Because this is a dedicated hardware test setup, the I2C GPIOs are intentionally hard-wired.

It verifies that:

- I2C communication to the RTC works
- date and time can be read successfully
- RTC temperature data can be read
- serial output is stable over time

## What `TestRtc.ino` does

1. Starts Serial at `115200` baud.
2. Initializes I2C with custom pins:
   - `SCL` on GPIO12 (`D6`)
   - `SDA` on GPIO13 (`D7`)
3. Waits briefly (`300 ms`) to allow I2C/RTC startup.
4. Tries to initialize the DS3231 in a loop until the RTC is found.
5. Sets the RTC once in `setup()` to:
   - `2026-01-01 05:57:00`
6. In `loop()`, every 3 seconds:
   - reads current time from the RTC
   - prints `hour:minute:second`
   - prints RTC temperature in °C

## Wiring

- RTC `SCL` -> ESP8266 `D6` (GPIO12)
- RTC `SDA` -> ESP8266 `D7` (GPIO13)
- RTC `VCC` -> `3.3V` (or module-specific required voltage)
- RTC `GND` -> `GND`

## Required Library

- `RTClib` (Adafruit)

Also requires the standard `Wire` library.

## Test Procedure

- start ESPEasyFlasher with SleepUino eeps

Example output:

```text
5:57:3
Temperature: 24.25 C

5:57:6
Temperature: 24.25 C
```

## Notes

- In this hardware test, `rtc.adjust(...)` intentionally sets the clock on every reboot/reset. This is expected behavior for repeatable validation.
- If `Couldn't find RTC` appears continuously, check wiring, power, and I2C pin mapping.
