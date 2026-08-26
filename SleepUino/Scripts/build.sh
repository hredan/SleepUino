#!/usr/bin/bash
# to use the script, https://github.com/hredan/eep-build-action must be cloned before.
# set path in BUILD_ACTIOM_DIR to the path of the cloned repository.
# With GitBash the scripts are working also on Windows.

BUILD_ACTION_DIR="../../eep-build-action/"
export INPUT_SKETCH_NAME="SleepUino"
export INPUT_CORE="esp8266"
export INPUT_BOARD="d1_mini"
export INPUT_CORE_VERSION=""
export INPUT_CPU_F="80"
# export INPUT_FLASH="4M1M"
export INPUT_LIBS="ArduinoJson,ESP8266Audio,JC_Button,RTClib,U8g2"

URL=$(git remote get-url origin)
HASH=$(git rev-parse HEAD)


#$BUILD_ACTION_DIR/build_sketch.sh -s $INPUT_SKETCH_NAME -c $INPUT_CORE -b $INPUT_BOARD -f $INPUT_CPU_F -l $INPUT_LIBS
python3 $BUILD_ACTION_DIR/build_sketch.py
$BUILD_ACTION_DIR/create_build_info.sh -r $URL -s $HASH
$BUILD_ACTION_DIR/create_eep_package.sh -s $INPUT_SKETCH_NAME -c $INPUT_CORE -b $INPUT_BOARD
