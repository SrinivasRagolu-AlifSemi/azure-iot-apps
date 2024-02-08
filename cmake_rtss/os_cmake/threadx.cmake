# Threadx OS related files
set (THREADX_DIR        "${SRC_DIRECTORY}/threadx")
set (THREADX_COMMON     "${THREADX_DIR}/common")

# Based on the compiler, collecting the Port source files
if (COMPILER STREQUAL ARMCLANG)
    set (THREADX_PORTS      "${THREADX_DIR}/ports/cortex_m55/ac6")
elseif (COMPILER STREQUAL GCC)
    set (THREADX_PORTS      "${THREADX_DIR}/ports/cortex_m55/gnu")
endif ()

# Directories to include from AZURE RTOS
include_directories (${THREADX_COMMON}/inc)
include_directories (${THREADX_PORTS}/inc)
include_directories (${IOT_APP_DIR}/cert)

# Source Files from AZURE RTOS
file (GLOB OS_S_SRC "${THREADX_PORTS}/src/*.S")
file (GLOB OS_C_SRC "${THREADX_COMMON}/src/*.c" "${THREADX_PORTS}/src/*.c")

# Test application source file and dependency files for RTOS Networking
if (netxduo_ret AND filex_ret)
    set (RTOS_NW_DIR                "${SRC_DIRECTORY}/RTOS_networking/netxduo")
    file (GLOB RTOS_NW_ETH_APP      "${RTOS_NW_DIR}/ethernet/apps/demo_netx.c")
    file (GLOB RTOS_NW_APP           ${RTOS_NW_ETH_APP})
endif ()

if (netxduo_ret)
    set (IOT_APP_DIR                "${SRC_DIRECTORY}/netxduo/addons/azure_iot/samples")
    #    set (RTOS_NW_MODEM_APP_DIR      "${RTOS_NW_DIR}/modem/apps")

    file (GLOB IOT_APP_SOURCE      "${IOT_APP_DIR}/main.c")
    file (GLOB IOT_APP_DEP          "${IOT_APP_DIR}/cert/*" "${IOT_APP_DIR}/sample_device_identity.c" "${IOT_APP_DIR}/sample_azure_iot_embedded_sdk.c")


    get_filename_component (rm_dep "${IOT_APP_SOURCE}" ABSOLUTE)
    list (REMOVE_ITEM IOT_APP_DEP "${rm_dep}")

    include_directories (${IOT_APP_DIR})
endif ()



# Test Application source file and Header file inclusions w.r.t Threadx
file (GLOB_RECURSE TEST_APP_SRCS "${SRC_DIRECTORY}/bolt_apps/Threadx/**/*.c" ${RTOS_NW_APP} ${IOT_APP_SOURCE})
include_directories (${SRC_DIRECTORY}/bolt_apps/Threadx/Camera/bayer2rgb)

#Including Threadx modem testapp cmake
#include (${OS_CMAKE_DIR}/threadx_modem.cmake)

# Collecting all the dependency files of Test Applications
file (GLOB CAMERA_APP_DEP       "${SRC_DIRECTORY}/bolt_apps/Threadx/Camera/bayer2rgb/*.c")
file (GLOB IMAGE_PROCESS_DEP    "${SRC_DIRECTORY}/bolt_apps/Threadx/MIPI/image_processing/image_processing.c")

# Putting the dependency files under one variable name
file (GLOB APP_DEP_FILES ${IMAGE_PROCESS_DEP} ${CAMERA_APP_DEP} ${RTOS_NW_DEP} ${IOT_APP_DEP})

# Removing Camera Dependency files from test application list
foreach (temp_app ${APP_DEP_FILES})
    get_filename_component (temp_path "${temp_app}" ABSOLUTE)
    list (REMOVE_ITEM TEST_APP_SRCS "${temp_path}")
endforeach (temp_app ${APP_DEP_FILES})

if (DEV_NAME STREQUAL M55_HP)
    RM_ENTRY(TEST_APP_SRCS      RM_TEST_APPS_LIST   "${SRC_DIRECTORY}/bolt_apps/Threadx/SPI/LPSPI_SPI_testapp.c")
    RM_ENTRY(TEST_APP_SRCS      RM_TEST_APPS_LIST   "${SRC_DIRECTORY}/bolt_apps/Threadx/PDM/LPPDM_testApp.c")
    RM_ENTRY(TEST_APP_SRCS      RM_TEST_APPS_LIST   "${SRC_DIRECTORY}/bolt_apps/Threadx/UART/LPUART_testApp.c")
    RM_ENTRY(TEST_APP_SRCS      RM_TEST_APPS_LIST   "${SRC_DIRECTORY}/bolt_apps/Threadx/Analog_Comparator/CMP_testapp.c")
endif ()

RM_ENTRY(TEST_APP_SRCS      RM_TEST_APPS_LIST   "${SRC_DIRECTORY}/bolt_apps/Threadx/MIPI/MIPI_Interface_Video_testApp.c")
file (GLOB THREADX_SRC ${OS_C_SRC} ${OS_S_SRC} ${APP_DEP_FILES})

# Creating Library file for OS files
set (OS_LIB_FILE "THREADX")
add_library (${OS_LIB_FILE} STATIC ${THREADX_SRC})
