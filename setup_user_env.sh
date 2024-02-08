#!/bin/bash

# Configure: CMSIS Version 
export CMSIS_VERSION=5.9.0

# Configure: CMSIS Compiler Version
export CMSIS_COMPILER_VERSION=1.0.0

# Configure: compiler (ARMCLANG or GCC)
export COMPILER=ARMCLANG

# Configure: ARM License Type
export ARM_PRODUCT_DEF=/opt/arm/developmentstudio-2022.0/sw/mappings/gold.elmap

# Configure: Complete Compiler Path
export COMPILER_BIN_PATH=/opt/arm/developmentstudio-2022.0/sw/ARMCompiler6.18/bin





#**************************************************************************************************
#                                           NO EDIT
#**************************************************************************************************
os_specific_str()  {

    if [[ "$OSTYPE" = "linux-gnu"* ]] ; then
        if uname -r | grep -q "microsoft-standard-WSL*"; then
            local  str1=$(wslpath "$(wslvar USERPROFILE)")
            local  str2="$str1/AppData/Roaming/"
            str1="$str1/AppData/Local/Arm/Packs"
        else
            local  str1="$HOME/.cache/arm/packs"
            local  str2="$HOME/."
        fi
    elif [[ "$OSTYPE" = "darwin"* ]]; then
        echo "Not Supported !!! $OSTYPE"
        exit 1
    elif [[ "$OSTYPE" = "cygwin" ]]; then
        echo "Not Supported !!! $OSTYPE"
        exit 1
    elif [[ "$OSTYPE" = "msys" ]]; then
        local  str1="$HOME/AppData/Local/Arm/Packs"
        local  str2="$HOME/AppData/Roaming/"
    elif [[ "$OSTYPE" = "win32" ]]; then
        local  str1="%USERPROFILE%\AppData\Local\Arm\Packs"
        local  str2="%USERPROFILE%\AppData\Roaming\\"
    elif [[ "$OSTYPE" = "freebsd"* ]]; then
        echo "Not Supported !!! $OSTYPE"
        exit 1
    else
        echo "UNKNOWN OS !!!"
        exit 1
    fi

   eval "$1=$str1"
   eval "$2=$str2"
}

os_specific_str     ALIF_PACK_BASE      ARM_LICENSE_BASE_PATH

if [ -z $CMSIS_PACK_PATH ]; then
    export CMSIS_PACK_PATH="$ALIF_PACK_BASE/ARM/CMSIS/$CMSIS_VERSION"
fi

if [ -z $CMSIS_COMPILER_PATH ]; then
    export CMSIS_COMPILER_PATH="$ALIF_PACK_BASE/ARM/CMSIS-Compiler/$CMSIS_COMPILER_VERSION"
fi

if [ -z $ARMLMD_LICENSE_FILE ]; then
    export ARMLMD_LICENSE_FILE="${ARM_LICENSE_BASE_PATH}arm/ds/licenses"
fi

if [ ! -d $CMSIS_PACK_PATH ]; then
  echo " \"CMSIS_PACK_PATH\" does not exist...."
fi

if [ ! -d $CMSIS_COMPILER_PATH ]; then
  echo " \"CMSIS_COMPILER_PATH\" does not exist...."
fi

if [ ! -d $ARMLMD_LICENSE_FILE ]; then
  echo " \"ARMLMD_LICENSE_FILE\" does not exist...."
fi

if [ ! -d $COMPILER_BIN_PATH ]; then
  echo " \"COMPILER_BIN_PATH\" compiler does not exist...."
fi

# Paths of Directories for the build of cmake_rtss and cmake_gnss
export CMAKE_REPO_PATH=$PWD/source
export CMAKE_RTSS_BUILD_DIR=$PWD/build_rtss
#export CMAKE_GNSS_BUILD_DIR=$PWD/build_gnss

# Paths of Directories for the build of cmake_packs
#export CMAKE_PACKS_DIR=$PWD/packs
#export CMAKE_BUILD_DIR=$PWD/build_packs

# Setting the Compiler bin to PATH environment variable
export PATH=$COMPILER_BIN_PATH:$PATH
