# specify the cross compiler
set (CMAKE_C_COMPILER               arm-none-eabi-gcc)
set (CMAKE_CXX_COMPILER             arm-none-eabi-g++)
set (CMAKE_C_LINKER_PREFERENCE      arm-none-eabi-ld)
set (CMAKE_ASM_LINKER_PREFERENCE    arm-none-eabi-ld)
set (CMAKE_ASM_COMPILER             ${CMAKE_C_COMPILER})
set (CMAKE_ASM_COMPILER_AR          arm-none-eabi-ar)

set (CMAKE_CROSSCOMPILING            true)
set (CMAKE_SYSTEM_NAME               Generic)

# Skip compiler test execution
set (CMAKE_C_COMPILER_WORKS          1)

# Cmake standards
set (CMAKE_C_STANDARD           11)
set (CMAKE_C_STANDARD_REQUIRED  ON)
set (CMAKE_C_EXTENSIONS         ON)

# Set System Processor
set (CMAKE_SYSTEM_PROCESSOR      cortex-m55)

# Flags for cortex-m55
set (CPU_COMPILE_DEF             CPU_CORTEX_M55)
set (CPU_NAME                    ${CMAKE_SYSTEM_PROCESSOR})
set (FLOAT_ABI                   hard)


# Macro Definitions
add_definitions (-D_RTE_)
add_definitions (-D${DEV_NAME})

# Defines needed for threadX
if (OS STREQUAL THREADX)
    add_definitions (-DTX_SINGLE_MODE_SECURE)
endif ()

# Flags for C source files
set (CMAKE_C_FLAGS  "-mcpu=cortex-m55 -mthumb -mfloat-abi=hard -fdata-sections ${C_COMPILER_ARGUMENTS}")

# Tell linker that reset interrupt handler is our entry point
add_link_options(-Wl,--gc-sections -Xlinker -print-memory-usage)

# Link Options
add_link_options(
    -T${LINKER_SCRIPT_PATH} -mthumb -march=armv8.1-m.main --specs=rdimon.specs
    -lc -lrdimon -mfpu=vfpv3-d16 -mfloat-abi=hard)

# Flags for S source files
set (ASM_FLAGS          "-mcpu=cortex-m55 -mthumb -mfloat-abi=hard -fdata-sections ${ASM_COMPILER_ARGUMENTS}")
set (CMAKE_ASM_FLAGS    "${ASM_FLAGS}")

# Path related to CMSIS Pack
set (CMSIS_PACK_PATH    ${CMSIS_PACK_PATH})

# Include directories wrt CMSIS pack
include_directories (${CMSIS_PACK_PATH}/CMSIS/Core/Include)
include_directories (${CMSIS_PACK_PATH}/Device/ARM/ARMCM55/Include)
include_directories (${CMSIS_PACK_PATH}/CMSIS/Driver/Include)

# Files and directories needed for Free-RTOS from CMSIS-PACK
if (OS STREQUAL FREERTOS)
    include_directories (${CMSIS_PACK_PATH}/CMSIS/RTOS2/Include)
endif ()