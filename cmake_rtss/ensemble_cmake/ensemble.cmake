# Including GNSS Middleware Files
if (PACK STREQUAL ENSEMBLE_GNSS)
    include (${MODEM_SS_CMAKE_DIR}/modem_ss.cmake)

    # Creating Library file for Ensemble Pack
    set (PACK_LIB     "ENSEMBLE_PACK")
    add_library (${PACK_LIB} STATIC ${MODEM_SS_LIB_SRC})

    # Including header files for Driver lib
    target_include_directories (${PACK_LIB} PRIVATE ${MODEM_SS_PRIVATE_INC})
endif ()