#!/bin/bash
CC=gcc
CXX=g++
export CXXFLAGS="$CXXFLAGS -std=c++11"
export CPPFLAGS="$CPPFLAGS -std=c++11"

# Export System SDK Path
export UIT_SYSTEM_SDK_PATH=/

# Import OS SDK Environment
#

# Set default baseline
export UIT_BASELINE=1.0
