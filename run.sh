#!/bin/bash
# Number of Arguments passed
ARGUMENTS_PASSED=$#

#Total number of Arguments required
MIN_ARGUMENTS=5
MAX_ARGUMENTS=8
cmake_build_error=0

NC='\033[0m' # No Color
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
BROWN='\033[0;37m'
HIGH_CYAN='\033[38;5;45m'

run_start_time=$SECONDS
echo -e "\n\n==================================================================================================="
# Checking the Host Machine's OS type
echo "HOST_OSTYPE --> $OSTYPE"
curr_dir=$PWD

# Checking for Number of Arguments passed
if [ $ARGUMENTS_PASSED -lt $MIN_ARGUMENTS -o $ARGUMENTS_PASSED -gt $MAX_ARGUMENTS ]
  then
    echo -e "${RED}<--- Arguments Missing --->"
    echo -e "<---  Arguments required are, MIN_ARGUMENTS=${MIN_ARGUMENTS} and MAX_ARGUMENTS=${MAX_ARGUMENTS}  --->"
    echo -e "${NC}"
    echo -e "${GREEN}script usage --> \n"
    echo -e "./run.sh DEVICE=[RTSS_HP or RTSS_HE or M55_HP or M55_HE] PACK=[ENSEMBLE or ENSEMBLE_GNSS or GNSS_M55] OS=[THREADX or FREERTOS or CMSISRTOS or NONE] BOOT=[MRAM or TCM] TEST_APP=[demo_iperf_modem or ALL] CLEAN=[NO or YES or FORCE] JOB=NUMBER_OF_PARALLEL_THREADS DEVELOPER=[NO or YES]"
    echo -e "${NC}"
    exit 1
fi

# Assigning Arguments to their respective variables
for ARGUMENT in "$@"
do
    KEY=$(echo $ARGUMENT | cut -f1 -d=)
    KEY_LENGTH=${#KEY}
    VALUE="${ARGUMENT:$KEY_LENGTH+1}"

    export "$KEY"="$VALUE"

    # Enabling flag if compiler arguments are passed
    if [[ "$KEY" == "C_COMPILER_ARG" ]]
      then
        C_ARG_FLAG=1
    elif [[ "$KEY" == "ASM_COMPILER_ARG" ]]
      then
        S_ARG_FLAG=1
    fi
done

# Current Working Directory
CMAKE_SHELL_SCRIPT_PATH=$PWD

# Path for CMakeLists.txt file for rtss projects
CMAKE_LIST_PATH=$PWD/cmake_rtss      # $HOME/path/cmake_rtss

# Path for CMakeLists.txt file for gnss project
CMAKE_LIST_GNSS=$PWD/cmake_gnss      # $HOME/path/cmake_gnss

# Checking if Test app name is given
if [[ "$TEST_APP" == "ALL" ]]
  then
    APP_FLAG=0
else
    APP_FLAG=1                      # If Test App name is given, then make the flag high
fi

# Cleaning the directories from previous build
if [ "$CLEAN" = "FORCE" ]
  then
    echo  -e  "${RED} Deleting Build folder ...${NC}\n"
    rm -rf build_*
fi

if [ -z $DEVELOPER ] ; then
   DEVELOPER="NO"
fi

# Creating Build directory based on Compiler
if [[ "${COMPILER}" == "GCC" ]]
  then
    BUILD_DIR=${CMAKE_RTSS_BUILD_DIR}/GCC
elif [[ "${COMPILER}" == "ARMCLANG" ]]
  then
    BUILD_DIR=${CMAKE_RTSS_BUILD_DIR}/ARMCLANG
else
    echo -e "\n\n${RED}Build Environment is not set ...!!! "
    echo -e "User has to edit [\"cmake_env.sh\"] and [\"env_windows_setup.sh\" or \"env_linux_setup.sh\"] as per local setup"
    echo -e "After editing Run [\"source cmake_env.sh\"] and [\"source  env_windows_setup.sh or source  env_linux_setup.sh\"]\n\n${NC}"
fi

# CMake command Arguments for RTSS
CMAKE_RTSS_CMD_ARG="-S ${CMAKE_LIST_PATH} -B ${BUILD_DIR} -DCMAKE_REPO_PATH=${CMAKE_REPO_PATH}      \
                    -DCMSIS_PACK_PATH=${CMSIS_PACK_PATH} -DDEV_NAME=${DEVICE} -DPACK=${PACK}        \
                    -DOS=${OS} -DBOOT=${BOOT} -DEN_APP_FLAG=${APP_FLAG} -DTEST_APP=${TEST_APP}      \
                    -DC_COMPILER_ARG="${C_COMPILER_ARG}" -DASM_COMPILER_ARG="${ASM_COMPILER_ARG}"   \
                    -DC_ARG_FLAG=${C_ARG_FLAG} -DS_ARG_FLAG=${S_ARG_FLAG} -DCOMPILER=${COMPILER}    \
                    -DOS_TYPE=${OSTYPE} -DCMSIS_COMPILER_PATH=${CMSIS_COMPILER_PATH} -Wno-dev       \
                    -DDEVELOPER=${DEVELOPER}"

# CMake command Arguments for GNSS
CMAKE_GNSS_CMD_ARG="-S ${CMAKE_LIST_GNSS} -B ${CMAKE_GNSS_BUILD_DIR}    \
                    -DCMAKE_REPO_PATH=${CMAKE_REPO_PATH}                \
                    -DCMSIS_PACK_PATH=${CMSIS_PACK_PATH}                \
                    -DDEV_NAME=${DEVICE} -DEN_APP_FLAG=${APP_FLAG}      \
                    -DTEST_APP=${TEST_APP}                              \
                    -DC_COMPILER_ARG="${C_COMPILER_ARG}"                \
                    -DASM_COMPILER_ARG="${ASM_COMPILER_ARG}"            \
                    -DC_ARG_FLAG=${C_ARG_FLAG}                          \
                    -DS_ARG_FLAG=${S_ARG_FLAG} -DOS_TYPE=${OSTYPE}      \
                    -DCMSIS_COMPILER_PATH=${CMSIS_COMPILER_PATH} -Wno-dev"

# CMake command to run CMakeLists.txt file to configure and generate Makefile for the project
if [[ "$DEVICE" != "M55_G" ]]
  then

    # If the Host is Linux Machine
    if [ "$OSTYPE" == "linux-gnu" ]
      then
        cmake -E time cmake ${CMAKE_RTSS_CMD_ARG} || { exit 1; }
    # If the Host is Git Bash on Windows Machine
    elif [ "$OSTYPE" == "msys" ]
      then
        cmake -E time cmake -G "Unix Makefiles" ${CMAKE_RTSS_CMD_ARG} || { exit 1; }
    fi

    # going to the directory where the Makefile is generated
    cd ${BUILD_DIR}

# If the Device selected is M55-G (GNSS_M55)
elif [[ "$DEVICE" == "M55_G" ]]
  then

    # If the Host is Linux Machine
    if [ "$OSTYPE" == "linux-gnu" ]
      then
        cmake -E time cmake ${CMAKE_GNSS_CMD_ARG} || { exit 1; }
    # If the Host is Git Bash on Windows Machine
    elif [ "$OSTYPE" == "msys" ]
      then
        cmake -E time cmake -G "Unix Makefiles" ${CMAKE_GNSS_CMD_ARG} || { exit 1; }
    fi

    # going to the directory where the Makefile is generated
    cd ${CMAKE_GNSS_BUILD_DIR}

fi

# Cleaning the directories from previous build
if [ "$ARGUMENTS_PASSED" -le "$MAX_ARGUMENTS" -a "$CLEAN" = "YES" ]
  then
    make clean
fi

# Running the makefile which is genereated by Cmake
echo -e "\n\n"
build_start_time=$SECONDS
if [ $JOB ] ; then
    make -k -j $JOB
else
    make -k -j 16
fi

cmake_build_error=$?

elapsed_time_for_build=$(( SECONDS - build_start_time ))

echo -e "\n"
for i in $(seq 1 60);
do
    printf  $'\U2728'
done

if [ -e $curr_dir/.tmp ]
then
    line1=$(sed -n '1p' $curr_dir/.tmp)
    line2=$(sed -n '2p' $curr_dir/.tmp)
    available_elf_files=$(find $line1 -maxdepth 1 -type f -name '*.elf' | wc -l)
    requested_application_cnt=$(echo $line2 | cut -d "," -f 1 | cut -d " " -f 2)
    invalid_application_cnt=`tail -1 "$curr_dir/.tmp"`

    echo -e "\n"
    generated_elf_files_cnt=0
    old_elf_files_cnt=0

    # First 2 line is already read in previous lines
    for i in `seq 1 $((requested_application_cnt - invalid_application_cnt))`
    do
        k=$((i+2))         # Skipping first two line
        line=$(sed -n "${k}p" $curr_dir/.tmp)

        eval "grep -sqF \"${line}\" \"$curr_dir/.tmp1\""
        if [ $? -eq 0 ] ; then
            execGeneratedFlag=1
        else
            execGeneratedFlag=0 # do something else if the string is not found
        fi

        elf_file=$line.elf
        if [ -e "$line1/$elf_file" ] ; then
            if [ ${execGeneratedFlag} -eq 1 ]; then
                echo -e $'\U2705'" ${GREEN}-- Generate App Name          --> $elf_file ${NC}"
                generated_elf_files_cnt=$((generated_elf_files_cnt+1))
            else
                echo -e $'\U2757'" ${YELLOW}-- Couldn't Generate App Name --> $elf_file ${BLUE}(last successful build elf exist) ${NC}"
                old_elf_files_cnt=$((old_elf_files_cnt+1))
            fi

        else
            echo -e '\U26D4'" ${RED}-- Couldn't Generate App Name --> $elf_file ${NC}"
        fi
    done

    failed_elf_cnt=$((requested_application_cnt - invalid_application_cnt - generated_elf_files_cnt - old_elf_files_cnt))
    echo -e ""
    for i in $(seq 1 60);
    do
        printf  $'\U2728'
    done
    echo -e "\n\n\n ${HIGH_CYAN}-- Application Build Statistics  [ ${GREEN}Generated: $generated_elf_files_cnt, ${RED}Failed: $failed_elf_cnt, Invalid: $invalid_application_cnt, ${YELLOW}$line2 ] ${NC}"
    rm   $curr_dir/.tmp
    rm   $curr_dir/.tmp1
fi

echo -e "\n"
eval "echo ========================== Build Time Taken :: $(date -ud "@$elapsed_time_for_build" +'$((%s/3600/24)) days %H hr %M min %S sec') =========================="
echo -e "\n\n"
total_run_elapsed_time=$(( SECONDS - run_start_time ))
eval "echo  Total Elapsed Time: $(date -ud "@$total_run_elapsed_time" +'$((%s/3600/24)) days %H hr %M min %S sec')"
echo -e "\n"

# Returning to the Shell Script directory
cd ${CMAKE_SHELL_SCRIPT_PATH}
exit $((cmake_build_error + invalid_application_cnt))
