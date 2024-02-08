# minimum version of cmake = 3.25 for legit reason:
# armclang support doesn't work work in previous releases
cmake_minimum_required(VERSION 3.25.0)

# Setting the Toolchain path
set (TOOLCHAIN_DIRECTORY    "${CMAKE_CURRENT_SOURCE_DIR}/toolchains")

if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m"     CACHE INTERNAL "")
  set(ColourBold  "${Esc}[1m"    CACHE INTERNAL "")
  set(Red         "${Esc}[31m"   CACHE INTERNAL "")
  set(Green       "${Esc}[32m"   CACHE INTERNAL "")
  set(Yellow      "${Esc}[33m"   CACHE INTERNAL "")
  set(Blue        "${Esc}[34m"   CACHE INTERNAL "")
  set(Magenta     "${Esc}[35m"   CACHE INTERNAL "")
  set(Cyan        "${Esc}[36m"   CACHE INTERNAL "")
  set(White       "${Esc}[37m"   CACHE INTERNAL "")
endif()

#To Debug Cmake environment
#set(CMAKE_VERBOSE_MAKEFILE      ON)

#Error Check
if(NOT CMSIS_PACK_PATH)
    message(FATAL_ERROR     "${Red}CMSIS PACK IS NOT ASSIGNED TO ANY VALUE...${ColourReset}")
elseif (NOT EXISTS ${CMSIS_PACK_PATH})
    message(FATAL_ERROR   "${Red}GIVEN CMSIS PACK PATH IS NOT AVAILABLE...${ColourReset}")
endif()


# Source directory paths
if (DEFINED CMAKE_REPO_PATH)
    set (SRC_DIRECTORY      "${CMAKE_REPO_PATH}")
else ()
    message (FATAL_ERROR "${Red}GIT REPO PATH NOT DEFINED${ColourReset}")
endif()

message (STATUS "SELECTED COMPILER --> ${Cyan}${COMPILER}${ColourReset}")

if (COMPILER STREQUAL ARMCLANG)
    include (rtss_compiler_config.cmake)
    set(CMAKE_TOOLCHAIN_FILE        "${TOOLCHAIN_DIRECTORY}/armclang_toolchain.cmake")
    file(GLOB RETARGET_IO_SRCS      "${CMSIS_COMPILER_PATH}/source/armcc/retarget_io.c")
elseif (COMPILER STREQUAL GCC)
    include (rtss_gcc_compiler_config.cmake)
    set(CMAKE_TOOLCHAIN_FILE        "${TOOLCHAIN_DIRECTORY}/arm-none-eabi-gcc.cmake")
    file(GLOB RETARGET_IO_SRCS      "${CMSIS_COMPILER_PATH}/source/gcc/retarget_syscalls.c")
endif ()

#Include common Include path
#set(RETARGET_IO_INC "${CMSIS_COMPILER_PATH}/include;${SRC_DIRECTORY}/bolt_apps/retarget_io/config")
#include_directories(${RETARGET_IO_INC})
#include_directories ("${CMSIS_PACK_PATH}/CMSIS/RTOS2/Include")
#file(GLOB IO_SRC "${SRC_DIRECTORY}/bolt_apps/retarget_io/source/*.c")
#list(APPEND RETARGET_IO_SRCS "${IO_SRC}")

#SE service common files
#include_directories ("${SRC_DIRECTORY}/bolt_apps/libs/se_services/port/include")
#include_directories ("${SRC_DIRECTORY}/bolt_apps/libs/se_services/include")
#file(GLOB SE_SERVICES_SRC  "${SRC_DIRECTORY}/bolt_apps/libs/se_services/*.c" "${SRC_DIRECTORY}/bolt_apps/libs/se_services/port/*.c")
#list(APPEND COMMON_SRCS "${SE_SERVICES_SRC}")

# Setting the Paths for Device and Chip
set (ENSEMBLE_CMAKE_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/ensemble_cmake")

# Setting Paths for OS selection cmake files
set (OS_CMAKE_DIR           "${CMAKE_CURRENT_SOURCE_DIR}/os_cmake")

# Setting path for Driver cmake files
set (DRIVERS_DIR            "${CMAKE_CURRENT_SOURCE_DIR}/drivers_cmake")

# SEtting path for Modem_ss library cmake file
#set (MODEM_SS_CMAKE_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/modem_ss_cmake")

# Setting path for NETXDUO camke files
set (NETXDUO_CMAKE_DIR      "${CMAKE_CURRENT_SOURCE_DIR}/netxduo_cmake")

# Setting path for AzureRTOS USBX cmake file
#set (USBX_CMAKE_DIR         "${CMAKE_CURRENT_SOURCE_DIR}/usbx_cmake")

# Setting path for Device related cmake file
set (DEVICE_CMAKE_DIR        "${CMAKE_CURRENT_SOURCE_DIR}/device_cmake")

# Setting path for FileX related cmake file
#set (FILEX_CMAKE_DIR        "${CMAKE_CURRENT_SOURCE_DIR}/filex_cmake")

# Setting path for se_host_services related cmake file
#set (HOST_SERVICES_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/host_services_cmake")

# Binary Directories
set (BIN                 ${CMAKE_BINARY_DIR}/exec)

if ((PACK STREQUAL ENSEMBLE) OR (PACK STREQUAL ENSEMBLE_GNSS))
    set (BINS_DIR       ${BIN}/ensemble)
else()
    message(FATAL_ERROR "${Red} GIVEN PACK IS NOT SUPPORTED ...!!! ${ColourReset}")
endif ()

set (THREADX_BIN_DIR     ${BINS_DIR}/threadx)
#set (FREE_RTOS_BIN_DIR   ${BINS_DIR}/freertos)
#set (CMSIS_RTOS_BIN_DIR  ${BINS_DIR}/cmsis_rtos)
#set (BAREMETAL_BIN_DIR   ${BINS_DIR}/bare_metal)

# Library directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Including Utilities cmake
include (utilities_func.cmake)

# Including Device related cmake
if ((DEV_NAME STREQUAL RTSS_HP) OR (DEV_NAME STREQUAL RTSS_HE))
    if (DEV_NAME STREQUAL RTSS_HP)
        set (DEV_NAME   "M55_HP")
    elseif (DEV_NAME STREQUAL RTSS_HE)
        set (DEV_NAME   "M55_HE")
    endif ()

    include (${DEVICE_CMAKE_DIR}/b0_device.cmake)

    if (DEVELOPER STREQUAL "NO")
        GET_GIT_PARAMS("${SRC_DIRECTORY}/bolt_apps" "b0_develop" FATAL_ERROR)
        GET_GIT_PARAMS("${SRC_DIRECTORY}/bolt_extsys_driver" "b0_develop" FATAL_ERROR)
    endif()
else ()
    include (${DEVICE_CMAKE_DIR}/cortex_m55_device.cmake)

    if (DEVELOPER STREQUAL "NO")
        GET_GIT_PARAMS("${SRC_DIRECTORY}/bolt_apps" "master" FATAL_ERROR)
        GET_GIT_PARAMS("${SRC_DIRECTORY}/bolt_extsys_driver" "master" FATAL_ERROR)
    endif()
endif ()

# Directory paths of OS cmake files
if (OS STREQUAL THREADX)
    message (STATUS "SELECTED OS       --> ${Cyan}THREADX${ColourReset}")

    GET_MACRO_VALUE ("${RTEcomponentFile}"  RTE_AZURE_RTOS_NETXDUO  netxduo_ret)
    GET_MACRO_VALUE ("${RTEcomponentFile}"  RTE_AZURE_RTOS_USBX     usbx_ret)
    GET_MACRO_VALUE ("${RTEcomponentFile}"  RTE_AZURE_RTOS_FILEX    filex_ret)

    # Including cmake for NETXDUO
    if (netxduo_ret)
        include (${NETXDUO_CMAKE_DIR}/netxduo.cmake)
    endif ()

    # Including cmake for USBX repo
    if (usbx_ret)
        include (${USBX_CMAKE_DIR}/usbx.cmake)
    endif ()

    # Including cmake for Threadx OS
    include (${OS_CMAKE_DIR}/threadx.cmake)

    # Including cmake for FileX source
    if (filex_ret)
        include (${FILEX_CMAKE_DIR}/azurertos_filex.cmake)
    endif ()

elseif (OS STREQUAL FREERTOS)
    message (STATUS "SELECTED OS       --> ${Cyan}FREE-RTOS${ColourReset}")
    include (${OS_CMAKE_DIR}/freertos.cmake)

elseif (OS STREQUAL CMSISRTOS)
    set (OS_BIN_DIR     ${CMSIS_RTOS_BIN_DIR})

elseif (OS STREQUAL NONE)
    message (STATUS "SELECTED OS       --> ${Cyan}Bare-Metal${ColourReset}")
    include (${OS_CMAKE_DIR}/baremetal.cmake)

else()
    message (FATAL_ERROR "${Red}INVALID OS ...!!!${ColourReset}")
endif ()

# Pack inclusion based on the configuration
if ((PACK STREQUAL ENSEMBLE) OR (PACK STREQUAL ENSEMBLE_GNSS))
    message (STATUS "SELECTED PACK     --> ${Cyan}ENSEMBLE ${ColourReset}")
    include (${ENSEMBLE_CMAKE_DIR}/ensemble.cmake)
else ()
    message (FATAL_ERROR "${Red}DEFINE PROPER PACK NAME (Please run ./run.sh to see options)${ColourReset}")
endif()

# Including cmake for Drivers
set (DRIVER_DIR     "${SRC_DIRECTORY}/bolt_extsys_driver")
include (${DRIVERS_DIR}/bolt_drivers.cmake)

# Including Host Services cmake
#include (${HOST_SERVICES_CMAKE_DIR}/se_host_services.cmake)

# Mark the language as C so that CMake doesn't try to test the C++
# cross-compiler's ability to compile a simple program because that will fail
project("Alif_CMake_Testapps"
        VERSION 1.0.0
        DESCRIPTION "COLLECTION_OF_ALIF_TESTAPPS "
        LANGUAGES C ASM)

# Creating a Library file for Retarget IO & Common Source Files
if ((RETARGET_IO_SRCS) OR (COMMON_SRCS))
    set(COMMON_LIB     "COMMON")
    list(APPEND COMMON_SRCS ${RETARGET_IO_SRCS})
    add_library(${COMMON_LIB} STATIC ${COMMON_SRCS})
endif()

# Collecting all the Test application names
list(LENGTH    TEST_APP_SRCS  TOTAL_NUM_OF_APPLICATION)
message (STATUS "<--- Test Applications to be Built -->")

string (REPLACE     ","     ";"     TEST_APPS           ${TEST_APP} )

if (EN_APP_FLAG)
    list(LENGTH     TEST_APPS   TOTAL_NUM_OF_REQUESTED_APPLICATION)
    list(FIND       TEST_APPS   "ALL"   ALL_APPS_FLAG)

    if( NOT ALL_APPS_FLAG EQUAL -1)
        set(EN_APP_FLAG     0)
        MATH(EXPR TOTAL_NUM_OF_REQUESTED_APPLICATION "${TOTAL_NUM_OF_APPLICATION} - ${TOTAL_NUM_OF_REQUESTED_APPLICATION} + 1")
        set(EXCLUDED_APP_LIST   "${TEST_APPS}")
        set(TEST_APPS           "ALL")
        list(REMOVE_ITEM ${EXCLUDED_APP_LIST} "ALL")
    endif()
    message (STATUS "<--- ${TOTAL_NUM_OF_REQUESTED_APPLICATION} application requested to build out of ${TOTAL_NUM_OF_APPLICATION} -->")
else ()
    set(TOTAL_NUM_OF_REQUESTED_APPLICATION ${TOTAL_NUM_OF_APPLICATION})
    message (STATUS "<--- All ${TOTAL_NUM_OF_APPLICATION} applications will build -->")
endif()

set(TMP_FILE    "../.tmp" )
set(TMP_FILE1   "../.tmp1")
file(WRITE      ${TMP_FILE}     ${OUTPUT_DIR})
file(WRITE      ${TMP_FILE1}    "")
file(APPEND     ${TMP_FILE}     "\nRequested: ${TOTAL_NUM_OF_REQUESTED_APPLICATION}, Total: ${TOTAL_NUM_OF_APPLICATION}")
SET(INVALID_TESTAPP_CNT     0)

foreach (testApp ${TEST_APPS})
    set(TEST_FOUND_FLAG "Not Found")
    foreach (testsourcefile ${TEST_APP_SRCS})

        # Collectiong Test app names one by one
        get_filename_component (tempname "${testsourcefile}" NAME)
        string (REPLACE ".c" "" testname ${tempname} )

        # If the Flag is high, build only for the given Test Application
        if (EN_APP_FLAG)

            if (testname STREQUAL testApp)

                message (STATUS "App Name --> ${Yellow}${testname}${ColourReset}")
                set(TEST_FOUND_FLAG "found")
                file(APPEND ${TMP_FILE} "\n${testname}")

                # Calling the build function
                BUILD_PROJECT ()

                break ()

            endif ()

        # Build for all the test Applications
        elseif (NOT EN_APP_FLAG)
            list(FIND       EXCLUDED_APP_LIST   ${testname}   EXCLUDED_APP_LIST_FLAG)

            if (EXCLUDED_APP_LIST_FLAG EQUAL -1)
                message (STATUS "App Name --> ${Yellow}${testname}${ColourReset}")
                file(APPEND ${TMP_FILE} "\n${testname}")

                # Calling the build function
                BUILD_PROJECT ()

            endif()

        endif ()

    endforeach (testsourcefile )

    if ( (TEST_FOUND_FLAG STREQUAL "Not Found") AND (NOT testApp STREQUAL "ALL" ) )
        message (STATUS "App Name --> ${Yellow}${testApp}    => [ :${Red}${TEST_FOUND_FLAG} ]${ColourReset}")
        MATH(EXPR INVALID_TESTAPP_CNT "${INVALID_TESTAPP_CNT}+1")
    endif ()

endforeach (testApp)

message("\n")
message("${Cyan}Disabled App List as follows ${ColourReset}")

foreach(element IN LISTS RM_TEST_APPS_LIST)
    message(STATUS  "${Yellow} ${element} ${ColourReset}")
endforeach()

file(APPEND ${TMP_FILE} "\n${INVALID_TESTAPP_CNT}")
