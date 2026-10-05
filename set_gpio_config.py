#!/usr/bin/env python3
"""Set SleepUino/config.h from a board-specific config.json file."""

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

    return {
        "ledSunPin": int(gpio.get("ledSunPin")),
        "ledMoonPin": int(gpio.get("ledMoonPin")),
        "buttonPin": int(gpio.get("buttonPin")),
        "slcPin": int(i2c.get("sclPin", i2c.get("slcPin"))),
        "sdaPin": int(i2c.get("sdaPin")),
    }


def update_header(header_path: Path, values: dict) -> None:
    replacements = [
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

    with header_path.open("r", encoding="utf-8") as f:
        content = f.read()

    for pattern, replacement in replacements:
        updated, count = pattern.subn(replacement, content, count=1)
        if count == 0:
            raise ValueError(f"Could not find matching GPIO define for replacement: {pattern.pattern}")
        content = updated

    if "#define SLC_PIN" in content or "#define SDA_PIN" in content:
        slc_replacement = (
            re.compile(r"(?m)^#define\s+SLC_PIN\s+\d+\s*$"),
            f"#define SLC_PIN {values['slcPin']}",
        )
        sda_replacement = (
            re.compile(r"(?m)^#define\s+SDA_PIN\s+\d+\s*$"),
            f"#define SDA_PIN {values['sdaPin']}",
        )

        for pattern, replacement in [slc_replacement, sda_replacement]:
            updated, count = pattern.subn(replacement, content, count=1)
            if count == 0:
                raise ValueError(f"Could not find matching I2C define for replacement: {pattern.pattern}")
            content = updated

    with header_path.open("w", encoding="utf-8") as f:
        f.write(content)


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Read a board config.json and update SleepUino GPIO settings in one of the header files."
    )
    parser.add_argument(
        "board_dir",
        help="Directory name under Config/, e.g. ESP32C3_Super_Mini",
    )
    parser.add_argument(
        "--target",
        choices=["main", "test-led-button", "testledbutton"],
        default="main",
        help="Which header file to update: main or test-led-button",
    )
    args = parser.parse_args()

    repo_root = Path(__file__).resolve().parent
    config_dir = repo_root / "Config" / args.board_dir
    if args.target == "main":
        header_path = repo_root / "SleepUino" / "Config.h"
    else:
        header_path = repo_root / "SleepUinoHwTest" / "TestLedButton" / "config.h"

    values = load_board_config(config_dir)
    update_header(header_path, values)

    print(f"Updated {header_path} from {config_dir / 'config.json'}")
    print(
        f"GPIOs:\nLED_SUN_PIN={values['ledSunPin']}, "
        f"LED_MOON_PIN={values['ledMoonPin']}, "
        f"BUTTON_PIN={values['buttonPin']}, "
        f"SLC_PIN={values['slcPin']}, SDA_PIN={values['sdaPin']}"
    )


if __name__ == "__main__":
    main()
