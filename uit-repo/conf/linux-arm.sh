#!/bin/bash
# Export System SDK Path
export UIT_SYSTEM_SDK_PATH=/work/sdk/sv-g6s-system-sdk

# Import OS SDK Environment
. $UIT_SYSTEM_SDK_PATH/environment-setup-aarch64-poky-linux

# Set default baseline
export UIT_BASELINE=1.0
