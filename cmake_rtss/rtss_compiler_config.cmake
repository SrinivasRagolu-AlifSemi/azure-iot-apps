# C-Compiler configurations
set (C_OPT_LEVEL          "-O0")
set (C_LANGUAGE_MODE      "-std=c11")
set (C_DEBUG_LEVEL        "-g")
#set (C_WARNINGS_ERRORS    "-Wall -Wextra")
#set (C_WARNINGS_ERRORS    "-Wall -Wextra -Weverything -Wno-packed -Wno-reserved-id-macro -Wno-unused-macros -Wno-documentation-unknown-command -Wno-documentation -Wno-license-management -Wno-parentheses-equality -Wno-reserved-identifier -Wc11-extensions -Wpointer-arith")
set (C_SHORT_FLAGS        "-fshort-enums -fshort-wchar -ferror-limit=2048")
set (C_SECURE_FLAGS       "-mcmse")

# Checking if user has provided C-Compiler arguments or not
if (C_ARG_FLAG)
    set (C_COMPILER_ARGUMENTS   "${C_COMPILER_ARG}")
else ()
    set (C_COMPILER_ARGUMENTS   "${C_LANGUAGE_MODE} ${C_OPT_LEVEL} ${C_DEBUG_LEVEL} ${C_WARNINGS_ERRORS} ${C_SHORT_FLAGS} ${C_SECURE_FLAGS}")
endif (C_ARG_FLAG)

# Assembler Configurations
set (S_DEBUG_LEVEL        "-g3")
set (S_WARNINGS_ERRORS    "-Wall")
set (S_ASSEMB_SYNTAX      "-masm=auto")

# Checking if user has provided ASM-Compiler arguments or not
if (S_ARG_FLAG)
    set (ASM_COMPILER_ARGUMENTS   "${ASM_COMPILER_ARG}")
else ()
    set (ASM_COMPILER_ARGUMENTS   "${S_DEBUG_LEVEL} ${S_WARNINGS_ERRORS} ${S_ASSEMB_SYNTAX}")
endif (S_ARG_FLAG)
