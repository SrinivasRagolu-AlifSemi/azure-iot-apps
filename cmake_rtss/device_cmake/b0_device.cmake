# Setting paths for Bin directory wrt to OS defined
if (OS STREQUAL THREADX)
    # Creating Directory for Threadx Binary files
    set (OS_BIN_DIR     ${THREADX_BIN_DIR})
elseif (OS STREQUAL FREERTOS)
    # Setting Bin Directory wrt to Free-RTOS
    set (OS_BIN_DIR     ${FREE_RTOS_BIN_DIR})
elseif (OS STREQUAL CMSISRTOS)
    # Setting Bin Directory wrt to CMSIS-RTOS
    set (OS_BIN_DIR     ${CMSIS_RTOS_BIN_DIR})
elseif (OS STREQUAL NONE)
    # Creating Binary directory for Bare-Metal
    set (OS_BIN_DIR     ${BAREMETAL_BIN_DIR})
else()
    # OS is not proper
    message (FATAL_ERROR "${Red}GIVEN OS IS NOT SUPPORTED ${ColourReset}")
endif ()

# Header inclusion based on Device type
set (DEF_M55_HP     "#define CMSIS_device_header \"M55_HP.h\"")
set (DEF_M55_HE     "#define CMSIS_device_header \"M55_HE.h\"")

# If pack name is Ensemble
if ((PACK STREQUAL ENSEMBLE) OR (PACK STREQUAL ENSEMBLE_GNSS))
    set (RTE_COMP_DIR   "${CMAKE_CURRENT_SOURCE_DIR}/../Include_RTE_Comp/ensemble")
else ()
    message (FATAL_ERROR "${Red}DEFINE PROPER PACK NAME (Please run ./run.sh to see options)${ColourReset}")
endif ()

# Assigning RTE components header file to a variable
include_directories (${RTE_COMP_DIR})
file (GLOB RTE_COMPONENTS_FILE  "${RTE_COMP_DIR}/RTE_Components.h")
file (STRINGS ${RTE_COMPONENTS_FILE}  RTEcomponentFile)

# Based on the Device name, paths and device files
set (DEVICE_PATH    "${SRC_DIRECTORY}/rtss_device")
if (DEV_NAME STREQUAL M55_HP)
    message (STATUS "SELECTED DEVICE   --> ${Cyan}B-Series (HP) ${ColourReset}")
    set (DEVICE_CORE_PATH           "${DEVICE_PATH}/core/M55_HP")

    # checking if the header name is M55_HP or not. If not, changing the name to M55_HP
    CHECK_DEF ("${DEF_M55_HP}" "${RTE_COMPONENTS_FILE}" ret)
    if (NOT ret)
        CHANGE_MACRO_VAL ("${DEF_M55_HE}" "${RTE_COMPONENTS_FILE}" "${DEF_M55_HP}")
    endif ()

    #Choosing the proper Scatter file to boot
    if (BOOT STREQUAL TCM)
        set (SCRIPT_NAME   "M55_HP_TCM")
        set (CMAKE_RUNTIME_OUTPUT_DIRECTORY   ${OS_BIN_DIR}/HP/TCM)
    elseif (BOOT STREQUAL MRAM)
        set (SCRIPT_NAME   "M55_HP")
        set (CMAKE_RUNTIME_OUTPUT_DIRECTORY   ${OS_BIN_DIR}/HP/MRAM)
    else ()
        message(FATAL_ERROR  "${Red}INVALID BOOT MODE ... !!!${ColourReset}")
    endif ()
# If Device name is M55_HE
elseif (DEV_NAME STREQUAL M55_HE)
    message (STATUS "SELECTED DEVICE   --> ${Cyan}B-Series (HE) ${ColourReset}")
    set (DEVICE_CORE_PATH           "${DEVICE_PATH}/core/M55_HE")

    # checking if the header name is M55_HE or not. If not, changing the name to M55_HE
    CHECK_DEF ("${DEF_M55_HE}" "${RTE_COMPONENTS_FILE}" ret)
    if (NOT ret)
        CHANGE_MACRO_VAL ("${DEF_M55_HP}" "${RTE_COMPONENTS_FILE}" "${DEF_M55_HE}")
    endif ()

    #Choosing the proper Scatter file to boot
    if (BOOT STREQUAL TCM)
        set (SCRIPT_NAME   "M55_HE_TCM")
        set (CMAKE_RUNTIME_OUTPUT_DIRECTORY   ${OS_BIN_DIR}/HE/TCM)
    elseif (BOOT STREQUAL MRAM)
        set (SCRIPT_NAME   "M55_HE")
        set (CMAKE_RUNTIME_OUTPUT_DIRECTORY   ${OS_BIN_DIR}/HE/MRAM)
    else ()
        message(FATAL_ERROR  "${Red}INVALID BOOT MODE ... !!!${ColourReset}")
    endif ()
else ()
    message (FATAL_ERROR "${Red}DEFINE PROPER DEVICE NAME (Please run ./run.sh to see options)${ColourReset}")
endif ()

# Path for RTEDevice.h
set (E7_PATH     "${DEVICE_PATH}/E7/AE722F80F55D5XX")

# Setting paths for Device directories
set (DEVICE_COMMON_SRC_DIR      "${DEVICE_PATH}/common/source")
set (DEVICE_COMMON_INC          "${DEVICE_PATH}/common/include")
set (DEVICE_CORE_SRC_DIR        "${DEVICE_CORE_PATH}/source")
set (DEVICE_CORE_CONFIG_DIR     "${DEVICE_CORE_PATH}/config")
set (DEVICE_CORE_INC_DIR        "${DEVICE_CORE_PATH}/include")
set (OUTPUT_DIR                 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CACHE INTERNAL "")

# Collecting Source files
file (GLOB DEVICE_SRC   "${DEVICE_COMMON_SRC_DIR}/*.c" "${DEVICE_CORE_SRC_DIR}/*.c")
include_directories (${E7_PATH})
include_directories (${DEVICE_COMMON_INC})
include_directories (${DEVICE_CORE_CONFIG_DIR})
include_directories (${DEVICE_CORE_INC_DIR})

# Linker File Details
if (COMPILER STREQUAL ARMCLANG)
    set (SCRIPT_DIR            "${E7_PATH}/linker_script/ARM")
    set (LINKER_SCRIPT_FILE    ${SCRIPT_DIR}/${SCRIPT_NAME}.sct)
    set (LINKER_SCRIPT_PATH    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${SCRIPT_NAME}.sct)
    set (LINKER_INC_PATH       "-I${DEVICE_CORE_INC_DIR} -I${DEVICE_COMMON_INC}")
    set (LINKER_CMD            "#! armclang -E --target=arm-arm-none-eabi -mcpu=cortex-m55 -xc")
    file(READ ${LINKER_SCRIPT_FILE} LINKER_CONTENT)
    string(REPLACE "${LINKER_CMD}" "${LINKER_CMD} ${LINKER_INC_PATH}" MODIFIED_LINKER_CONTENT "${LINKER_CONTENT}")
    file(WRITE ${LINKER_SCRIPT_PATH} "${MODIFIED_LINKER_CONTENT}")
elseif (COMPILER STREQUAL GCC)
    set (SCRIPT_DIR                 "${E7_PATH}/linker_script/GCC")

    if (DEV_NAME STREQUAL M55_HP)
        set (SCRIPT_NAME   "gcc_M55_HP")
    elseif (DEV_NAME STREQUAL M55_HE)
        set (SCRIPT_NAME   "gcc_M55_HE")
    endif ()
    
    set (LINKER_SCRIPT_PATH         ${SCRIPT_DIR}/${SCRIPT_NAME}.ld)
endif ()

set (DEVICE_LIB     "DEVICE")
add_library (${DEVICE_LIB} STATIC ${DEVICE_SRC})
