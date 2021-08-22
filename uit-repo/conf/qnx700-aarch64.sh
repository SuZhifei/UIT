#!/bin/bash

export CC=nto$UIT_ARCH-gcc
export CXX=nto$UIT_ARCH-g++
export CXXFLAGS="$CXXFLAGS -std=gnu++11"
export CPPFLAGS="$CPPFLAGS -std=gnu++11"
export UIT_CMAKE="$UIT_CMAKE -DCMAKE_TOOLCHAIN_FILE=$UIT_CONF_DIR/qnx.cmake"

# Export System SDK Path
UIT_SYSTEM_SDK_PATH=/work/sdk/sv-g6sh-qnx-system-sdk

# Import OS SDK Environment
if [ ! -f "$UIT_SYSTEM_SDK_PATH/qnxsdp-env.sh" ];then
	printWarn "无法找到[$UIT_SYSTEM_SDK_PATH/qnxsdp-env.sh], 是否未下载SDK？"
else
	. $UIT_SYSTEM_SDK_PATH/qnxsdp-env.sh
fi

# Set default baseline
export UIT_BASELINE=1.0
