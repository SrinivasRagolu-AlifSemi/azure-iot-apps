# C-Compiler configurations
set (C_OPT_LEVEL          "-O0")
set (C_LANGUAGE_MODE      "-std=gnu11")
set (C_DEBUG_LEVEL        "-g")
set (C_WARNINGS_ERRORS    "-Wall -Wextra -fmax-errors=2048")

# Checking if user has provided C-Compiler arguments or not
if (C_ARG_FLAG)
    set (C_COMPILER_ARGUMENTS   "${C_COMPILER_ARG}")
else ()
    set (C_COMPILER_ARGUMENTS   "${C_LANGUAGE_MODE} ${C_OPT_LEVEL} ${C_DEBUG_LEVEL} ${C_WARNINGS_ERRORS}")
endif (C_ARG_FLAG)

# Assembler Configurations
set (S_OPT_LEVEL          "-O0")
set (S_DEBUG_LEVEL        "-g")
set (S_WARNINGS_ERRORS    "-Wall")

# Checking if user has provided ASM-Compiler arguments or not
if (S_ARG_FLAG)
    set (ASM_COMPILER_ARGUMENTS   "${ASM_COMPILER_ARG}")
else ()
    set (ASM_COMPILER_ARGUMENTS   "${S_DEBUG_LEVEL} ${S_WARNINGS_ERRORS} ${S_OPT_LEVEL}")
endif (S_ARG_FLAG)