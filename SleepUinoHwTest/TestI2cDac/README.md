# TestI2cDac

`TestI2cDac` is a hardware test sketch for SleepUino audio playback on ESP8266 using I2S output and a MOD file from LittleFS.

## Purpose

This test verifies:

- LittleFS mounting and file access
- MOD playback through `ESP8266Audio`
- serial runtime control for playback and gain

## Test Behavior

At startup, the sketch:

1. Disables Wi-Fi.
2. Initializes Serial (`115200`).
3. Mounts LittleFS.
4. Prepares `AudioOutputI2S` and `AudioGeneratorMOD`.

Playback is controlled via Serial commands.

## Serial Commands

- `start`: starts MOD playback from `/musiklinjen.mod`
- `stop`: requests stopping playback
- `gain <0.0-4.0>`: sets output gain (default `1.0`)

Examples:

```text
gain 0.70
start
```

## Known Limitation

The sketch currently reports a known hardware/runtime conflict where `stop` may not reliably stop playback due to I2S GPIO interaction.

Current workaround:

- Use the `ESP Reset` button in the ESPEasyFlasher Serial Monitor.

## Required Assets

- MOD file in LittleFS data: `/musiklinjen.mod`
- Download source for `musiklinjen.mod`: https://modarchive.org/index.php?request=view_by_moduleid&query=42146
- Library: `ESP8266Audio`

## Build Script

Use the provided helper script in this folder:

```bash
./build_test_i2c_dac.sh
```
