#!/bin/bash
export UIT_REPO_DOMAIN_PREFIX=git@scm-gitlab:uit/uniform/kunp
export UIT_ROOT="`pwd`"
export UIT_CONF_DIR=$UIT_ROOT/conf
export UIT_HOST=linux
export UIT_SUPPORTED_PLATFORMS=
export UIT_OS=
export UIT_ARCH=
export UIT_TARGET=

#Args Check(UIT_OS & UIT_SUPPORTED_PLATFORMS)
i=0
for file in $UIT_CONF_DIR/*.sh; do
	one=${file##*/} && one=${one%%.sh} && UIT_SUPPORTED_PLATFORMS[i]=$one && i=`expr $i + 1`
done
for i in ${UIT_SUPPORTED_PLATFORMS[@]}; do
	if [ "$1" == "$i" ]; then
		var=$1
		UIT_OS=${var%%-*}
		UIT_ARCH=${var#*-}
		UIT_TARGET=$var
	fi
done

if [ "$UIT_OS" == "" ]; then
	echo -e "\033[0;32m	useage: env.sh [${UIT_SUPPORTED_PLATFORMS[*]}]\033[0;0m" && return 0
fi

export UIT_TOOL_DIR=$UIT_ROOT/tool/$UIT_HOST
export UIT_PACKAGE_DIR=$UIT_ROOT/package
export UIT_SYSTEM_SDK_PATH=
export UIT_COMPILER_EXE=
export UIT_COMPILER_VERSION=
export UIT_CMAKE_EXE=cmake
export CAPI_CORE_GEN="$UIT_TOOL_DIR/commonapi-generator/commonapi-generator-linux-x86_64"
export CAPI_DBUS_GEN="$UIT_TOOL_DIR/commonapi_dbus_generator/commonapi-dbus-generator-linux-x86_64"
export CAPI_SOMEIP_GEN="$UIT_TOOL_DIR/commonapi_someip_generator/commonapi-someip-generator-linux-x86_64"
export UIT_BUILD_DIR=build/$UIT_TARGET
export UIT_INSTALL_DIR=$UIT_ROOT/dist/$UIT_TARGET/uit
export UIT_CMAKE="$UIT_CMAKE_EXE \
-DCMAKE_INSTALL_PREFIX:PATH=$UIT_INSTALL_DIR \
-DCMAKE_INSTALL_BINDIR:PATH=$UIT_INSTALL_DIR/bin \
-DCMAKE_INSTALL_ETCDIR:PATH=$UIT_INSTALL_DIR/etc \
-DCMAKE_INSTALL_LIBDIR:PATH=$UIT_INSTALL_DIR/lib \
-DCMAKE_INSTALL_INCLUDEDIR:PATH=$UIT_INSTALL_DIR/include \
-DCMAKE_INSTALL_DOCDIR:PATH=$UIT_INSTALL_DIR/doc \
-DCMAKE_INSTALL_TEMPDIR:PATH=$UIT_INSTALL_DIR/temp \
-DCMAKE_INSTALL_SHAREDIR:PATH=$UIT_INSTALL_DIR/share \
-DCMAKE_INSTALL_RESOURCEDIR:PATH=$UIT_INSTALL_DIR/resource \
-DCMAKE_INSTALL_EXAMPLEDIR:PATH=$UIT_INSTALL_DIR/example \
-DCMAKE_BUILD_TYPE=release \
-Wno-dev"

#Source Target Config
. $UIT_CONF_DIR/$UIT_TARGET.sh
. $UIT_CONF_DIR/util/helper.sh
export UIT_UPDATE="$UIT_CONF_DIR/util/update.sh"

# Print Environment Configuration

echo -e "\033[0;34m--------------------------- Development Environment ---------------------------\033[0;0m"
echo -e "\033[0;31m  UIT Root      : \033[0;32m"$UIT_ROOT
echo -e "\033[0;31m  UIT Baseline  : \033[0;32m"$UIT_BASELINE
echo -e "\033[0;31m  Build Target  : \033[0;32m"$UIT_TARGET
echo -e "\033[0;31m  System SDK    : \033[0;32m"$UIT_SYSTEM_SDK_PATH
echo -e "\033[0;31m  GCC Exec      : \033[0;32m"`which $CC`
echo -e "\033[0;31m  G++ Exec      : \033[0;32m"`which $CXX`
echo -e "\033[0;31m  GCC Version   : \033[0;32m"$($CC -dumpversion)
echo -e "\033[0;31m  GCC Machine   : \033[0;32m"$($CC -dumpmachine)
echo -e "\033[0;31m  Install Path  : \033[0;32m"$UIT_INSTALL_DIR
echo -e "\033[0;34m-------------------------------------------------------------------------------\033[0;0m"
