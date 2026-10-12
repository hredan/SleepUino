#!/usr/bin/env python3
"""Set all project config.h files from a board-specific config.json file."""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


def load_board_config(config_dir: Path) -> dict:
    config_path = config_dir / "config.json"
    if not config_path.exists():
        raise FileNotFoundError(f"Config file not found: {config_path}")

    with config_path.open("r", encoding="utf-8") as f:
        data = json.load(f)

    gpio = data.get("gpio", {})
    i2c = gpio.get("i2c", {})
    i2s = gpio.get("i2s", {})

    return {
        "ledSunPin": int(gpio.get("ledSunPin")),
        "ledMoonPin": int(gpio.get("ledMoonPin")),
        "buttonPin": int(gpio.get("buttonPin")),
        "slcPin": int(i2c.get("sclPin", i2c.get("slcPin"))),
        "sdaPin": int(i2c.get("sdaPin")),
        "i2sBclkPin": int(i2s.get("bclkPin", i2s.get("sckPin", -1))),
        "i2sLrcPin": int(i2s.get("lrcPin", i2s.get("wsPin", i2s.get("lrckPin", -1)))),
        "i2sDoutPin": int(i2s.get("doutPin", i2s.get("dataPin", -1))),
    }


def update_header(header_path: Path, values: dict) -> None:
    replacements = []
    if "LED_SUN_GPIO" in header_path.read_text(encoding="utf-8"):
        replacements.extend(
            [
                (
                    re.compile(r"(?m)^#define\s+LED_SUN_GPIO\s+\d+\s*$"),
                    f"#define LED_SUN_GPIO {values['ledSunPin']}",
                ),
                (
                    re.compile(r"(?m)^#define\s+LED_MOON_GPIO\s+\d+\s*$"),
                    f"#define LED_MOON_GPIO {values['ledMoonPin']}",
                ),
                (
                    re.compile(r"(?m)^#define\s+BUTTON_GPIO\s+\d+\s*$"),
                    f"#define BUTTON_GPIO {values['buttonPin']}",
                ),
            ]
        )

    with header_path.open("r", encoding="utf-8") as f:
        content = f.read()

    for pattern, replacement in replacements:
        updated, count = pattern.subn(replacement, content, count=1)
        if count == 0:
            raise ValueError(
                f"Could not find matching GPIO define for replacement: {pattern.pattern}")
        content = updated

    if "#define SLC_PIN" in content or "#define SDA_PIN" in content:
        i2c_replacements = [
            (
                re.compile(r"(?m)^#define\s+SLC_PIN\s+\d+\s*$"),
                f"#define SLC_PIN {values['slcPin']}",
            ),
            (
                re.compile(r"(?m)^#define\s+SDA_PIN\s+\d+\s*$"),
                f"#define SDA_PIN {values['sdaPin']}",
            ),
        ]

        for pattern, replacement in i2c_replacements:
            updated, count = pattern.subn(replacement, content, count=1)
            if count == 0:
                raise ValueError(
                    f"Could not find matching I2C define for replacement: {pattern.pattern}")
            content = updated

    if "#define I2S_BCLK_PIN" in content or "#define I2S_LRC_PIN" in content or "#define I2S_DOUT_PIN" in content:
        i2s_replacements = [
            (
                re.compile(r"(?m)^#define\s+I2S_BCLK_PIN\s+-?\d+\s*$"),
                f"#define I2S_BCLK_PIN {values['i2sBclkPin']}",
            ),
            (
                re.compile(r"(?m)^#define\s+I2S_LRC_PIN\s+-?\d+\s*$"),
                f"#define I2S_LRC_PIN {values['i2sLrcPin']}",
            ),
            (
                re.compile(r"(?m)^#define\s+I2S_DOUT_PIN\s+-?\d+\s*$"),
                f"#define I2S_DOUT_PIN {values['i2sDoutPin']}",
            ),
        ]

        for pattern, replacement in i2s_replacements:
            updated, count = pattern.subn(replacement, content, count=1)
            if count == 0:
                raise ValueError(
                    f"Could not find matching I2S define for replacement: {pattern.pattern}")
            content = updated

    with header_path.open("w", encoding="utf-8") as f:
        f.write(content)


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Read a board config.json and update GPIO settings in all config.h files."
    )
    parser.add_argument(
        "board_dir",
        help="Directory name under Config/, e.g. ESP32C3_Super_Mini",
    )
    args = parser.parse_args()

    repo_root = Path(__file__).resolve().parent
    config_dir = repo_root / "Config" / args.board_dir
    values = load_board_config(config_dir)
    header_paths = [
        repo_root / "SleepUino" / "config.h",
        repo_root / "SleepUinoHwTest" / "TestLedButton" / "config.h",
        repo_root / "SleepUinoHwTest" / "TestDisplay" / "config.h",
        repo_root / "SleepUinoHwTest" / "TestRtc" / "config.h",
        repo_root / "SleepUinoHwTest" / "TestI2cDac" / "config.h",
    ]

    for header_path in header_paths:
        update_header(header_path, values)
        print(f"Updated {header_path} from {config_dir / 'config.json'}")

    print(
        f"GPIOs:\nLED_SUN_PIN={values['ledSunPin']}, "
        f"LED_MOON_PIN={values['ledMoonPin']}, "
        f"BUTTON_PIN={values['buttonPin']}, "
        f"SLC_PIN={values['slcPin']}, SDA_PIN={values['sdaPin']}, "
        f"I2S_BCLK_PIN={values['i2sBclkPin']}, I2S_LRC_PIN={values['i2sLrcPin']}, I2S_DOUT_PIN={values['i2sDoutPin']}"
    )


if __name__ == "__main__":
    main()
