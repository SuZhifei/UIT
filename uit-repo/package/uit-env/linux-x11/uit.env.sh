#!/bin/sh

####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

# Runtime Environment

#uit path
export UIT_RT_HOME_PATH=`pwd`/..
export UIT_RT_LIB_PATH=$UIT_RT_HOME_PATH/lib
export UIT_RT_BIN_PATH=$UIT_RT_HOME_PATH/bin
export UIT_RT_ETC_PATH=$UIT_RT_HOME_PATH/etc
export UIT_RT_DATA_PATH=$UIT_RT_HOME_PATH/data
export UIT_RT_RES_PATH=$UIT_RT_HOME_PATH/resource
export UIT_RT_SHARE_PATH=$UIT_RT_HOME_PATH/share
export UIT_RT_TEMP_PATH=$UIT_RT_HOME_PATH/temp

#someip
export COMMONAPI_CONFIG=$UIT_RT_ETC_PATH/commonapi.ini
export VSOMEIP_CONFIGURATION=$UIT_RT_ETC_PATH/vsomeip-dataservice.json

export PATH=$PATH:$UIT_RT_BIN_PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$UIT_RT_LIB_PATH

# SVP Version
#. ${SVP_ETC_PATH}/svp.version.sh
#echo -e "[SVP] SVP_VERSION      : $SVP_VERSION"
#echo -e "[SVP] SVP_BUILD_DATE   : $SVP_BUILD_DATE"


