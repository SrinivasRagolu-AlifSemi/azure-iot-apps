# specify the cross compiler
set (CMAKE_C_COMPILER                armclang)
set (CMAKE_CXX_COMPILER              armclang)
set (CMAKE_C_LINKER_PREFERENCE       armlink)
set (CMAKE_ASM_LINKER_PREFERENCE     armlink)
set (CMAKE_ASM_COMPILER              armclang)
set (CMAKE_ASM_COMPILER_AR           armar)

set (CMAKE_CROSSCOMPILING            true)
set (CMAKE_SYSTEM_NAME               Generic)

set (MIN_ARM_CLANG_VERSION           6.15)

# Skip compiler test execution
set (CMAKE_C_COMPILER_WORKS          1)

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
endif()

# Flags for C source files
set (CMAKE_C_FLAGS  "--target=arm-arm-none-eabi -mcpu=cortex-m55 -mfloat-abi=hard -mthumb -mlittle-endian -xc ${C_COMPILER_ARGUMENTS} -MD -MP")

# Tell linker that reset interrupt handler is our entry point
add_link_options(--map --entry=Reset_Handler --diag_suppress 6312,6314)

# Link Options
add_link_options(
    --info=sizes
    --scatter=${LINKER_SCRIPT_PATH})

# Flags for Assembly files 
set(CPU_LINK_OPT    "--target=arm-arm-none-eabi -mcpu=cortex-m55 -mfloat-abi=hard -mthumb -mlittle-endian -x assembler-with-cpp ${ASM_COMPILER_ARGUMENTS}")

set(CMAKE_ASM_FLAGS "${CPU_LINK_OPT}")

# Path related to CMSIS Pack
set (CMSIS_PACK_PATH    ${CMSIS_PACK_PATH})

# Include directories wrt CMSIS pack
include_directories (${CMSIS_PACK_PATH}/CMSIS/Core/Include)
include_directories (${CMSIS_PACK_PATH}/Device/ARM/ARMCM55/Include)
include_directories (${CMSIS_PACK_PATH}/CMSIS/Driver/Include)

# Files and directories needed for Free-RTOS from CMSIS-PACK
include_directories (${CMSIS_PACK_PATH}/CMSIS/RTOS2/Include)