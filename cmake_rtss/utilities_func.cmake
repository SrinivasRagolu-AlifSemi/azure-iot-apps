# MACRO to add all directories in result
# argv[0] - result variable, argv[1] - add relative paths, set "" if you don't want it
# argv[2] - path to folder with folders
macro (SUBDIRLIST firstdir curdir)
    file(GLOB ENDF6_SRC_TOP RELATIVE ${curdir} ${curdir}/*)
    file(GLOB ENDF6_SRC_SUBS RELATIVE ${curdir}/ ${curdir}/**/**)

    set(children ${ENDF6_SRC_TOP} ${ENDF6_SRC_SUBS})

    set(dirlist "${firstdir}")

    foreach (child ${children})
        if (IS_DIRECTORY ${curdir}/${child})
            list (APPEND dirlist ${curdir}/${child})
        endif ()
    endforeach ()

    set(result ${dirlist})

    foreach (subdir ${result})
        include_directories(${subdir})
    endforeach ()
endmacro ()

# Checking for Macro Definition in given header file
macro (CHECK_DEF check_def header_file return_val)
    file (READ "${header_file}" header)
    string (REGEX MATCH "${check_def}" MACRODEF "${header}")    # The string should be exactly as present in the file, including the spaces/tabs
    if (MACRODEF)
        set (${return_val} "1")
    else ()
        set (${return_val} "0") 
    endif (MACRODEF)
endmacro ()

# Get Macro value, (Note: It works only for real number of Macros)
macro (GET_MACRO_VALUE header macro_name macro_val)
    FOREACH(arg ${header})
        string(REGEX MATCHALL "^[ \t]*#(define|DEFINE)[ \t]+${macro_name}[ \t]+[0-9]+[ \t]*.*" foundDefines "${arg}")

        if (foundDefines)
            string(REGEX MATCH "[ \t]+[0-9]+" tmp "${foundDefines}")
            string(REGEX MATCH "[0-9]+" tmp "${tmp}")
            set(${macro_val} ${tmp})
        endif (foundDefines)
    endforeach()
endmacro ()

# Change Macro value
macro (CHANGE_MACRO_VAL check_def header_file change_val)
    file (READ "${header_file}" header)
    string(REGEX MATCH "${check_def}" macrodef "${header}")     # The string should be exactly as present in the file, including the spaces/tabs
    string (REPLACE "${macrodef}" "${change_val}" header "${header}")
    file(WRITE "${header_file}" "${header}")
endmacro ()

# Macro definition to Compile w.r.t OS types
macro (BUILD_PROJECT)

    set(EXECUTABLE ${testname})

    # Adding executable file name
    add_executable (${EXECUTABLE} ${DEVICE_SRC} ${testsourcefile})

    if (OS STREQUAL FREERTOS)

        # Linking all the library files to the test application
        target_link_libraries(${EXECUTABLE} ${COMMON_LIB} ${DRIVER_LIB} ${PACK_LIB} ${SE_HOST_SERVICES_LIB} ${OS_LIB_FILE})

    elseif (OS STREQUAL THREADX)

        # Linking all the library files to the test application
        target_link_libraries (${EXECUTABLE} ${COMMON_LIB} ${DRIVER_LIB} ${FILEX_LIB} ${PACK_LIB} ${NETXDUO_LIB} ${USBX_LIB} ${SE_HOST_SERVICES_LIB} ${OS_LIB_FILE})

    elseif (OS STREQUAL NONE)

        # Linking all the library files to the test application
        target_link_libraries(${EXECUTABLE} ${COMMON_LIB} ${DRIVER_LIB} ${SE_HOST_SERVICES_LIB})

    endif ()

    # Improve clean target
    set_target_properties(${EXECUTABLE} PROPERTIES ADDITIONAL_CLEAN_FILES 
        "${OUTPUT_DIR}/${EXECUTABLE}.bin;${OUTPUT_DIR}/${EXECUTABLE}.hex;${OUTPUT_DIR}/${EXECUTABLE}.map")

    if (COMPILER STREQUAL GCC)

        target_link_options(${EXECUTABLE} PRIVATE  -Wl,-Map=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXECUTABLE}.map)
        set_target_properties(${EXECUTABLE} PROPERTIES OUTPUT_NAME ${EXECUTABLE}.elf)
        add_custom_command(TARGET  ${EXECUTABLE}
           POST_BUILD
           COMMAND echo "${EXECUTABLE}" >> "${CMAKE_SOURCE_DIR}/${TMP_FILE1}"
            && echo  "${Cyan} ${EXECUTABLE}.elf  BUILD SUCCESSFUL *******************************${ColourReset}"
            && arm-none-eabi-objcopy -O binary     ${OUTPUT_DIR}/${EXECUTABLE}.elf  ${OUTPUT_DIR}/${EXECUTABLE}.bin
            && echo  "${Green}Generated Bin files for ${Yellow}${EXECUTABLE}   ${ColourReset}"
           COMMAND  arm-none-eabi-objcopy -O ihex       ${OUTPUT_DIR}/${EXECUTABLE}.elf  ${OUTPUT_DIR}/${EXECUTABLE}.hex
            &&  echo  "${Green}Generated Hex files for ${Yellow}${EXECUTABLE}   ${ColourReset}"
        )

    elseif (COMPILER STREQUAL ARMCLANG)

        add_custom_command(TARGET  ${EXECUTABLE}
           POST_BUILD
           COMMAND  echo "${EXECUTABLE}" >> "${CMAKE_SOURCE_DIR}/${TMP_FILE1}"
           && echo  "${Cyan} ${EXECUTABLE}.elf  BUILD SUCCESSFUL *******************************${ColourReset}"
           && fromelf --bin --output=${OUTPUT_DIR}/${EXECUTABLE}.bin  ${OUTPUT_DIR}/${EXECUTABLE}.elf
           && echo  "${Green}Generated Bin files for ${Yellow}${EXECUTABLE}.elf   ${ColourReset}"
           COMMAND  fromelf --i32 --output=${OUTPUT_DIR}/${EXECUTABLE}.hex  ${OUTPUT_DIR}/${EXECUTABLE}.elf
           && echo  "${Green}Generated Hex files for ${Yellow}${EXECUTABLE}.elf   ${ColourReset}"
        )

    endif ()

endmacro (BUILD_PROJECT)

# Get Git Parameters
macro (GET_GIT_PARAMS repoPath branchName messageType )

    string(REGEX REPLACE "[ \t]+$" "" my_repo_withpath ${repoPath})
    execute_process(
        COMMAND git -C ${my_repo_withpath} rev-parse --abbrev-ref HEAD
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

   execute_process(
        COMMAND git -C ${my_repo_withpath} rev-parse --short HEAD
        OUTPUT_VARIABLE GIT_SHA_ID
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    string(LENGTH ${my_repo_withpath} repoPath_length)
    math(EXPR repoPath_length "${repoPath_length} - 1")
    string(SUBSTRING ${my_repo_withpath} ${repoPath_length} 1 repoPath_last_char)

    if( repoPath_last_char STREQUAL "/")
        string(SUBSTRING ${my_repo_withpath} 0 ${repoPath_length} my_repo_withpath)
    endif()
    
    string(FIND ${my_repo_withpath} "/" last_slash REVERSE)
    math(EXPR last_slash "+ ${last_slash} + 1")
    string(SUBSTRING ${my_repo_withpath} ${last_slash} -1 repoName)

    if (${branchName} STREQUAL ${GIT_BRANCH})
        message(VERBOSE "Branch (${repoName}) Check : ${Cyan} PASS ${ColourReset}")
    else ()
        if (${branchName} STREQUAL ${GIT_SHA_ID})
            message(VERBOSE "Branch (${repoName}) Check : ${Cyan} PASS ${ColourReset}")
        else()
            message(${messageType} "Branch (${repoName}) Check : ${Red} FAIL \n(checkout proper branch as per A/B series device)${ColourReset}")
        endif()
    endif ()

endmacro ()

# Define a macro to move an item from one list to another
macro(RM_ENTRY  src_list dst_list   elementName_to_remove)
    get_filename_component(F_NAME   ${elementName_to_remove}    NAME_WE)
    list(REMOVE_ITEM    ${src_list}     ${elementName_to_remove})
    list(APPEND         ${dst_list}     ${F_NAME})
endmacro()
