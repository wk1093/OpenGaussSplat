function(add_format_target TARGET_NAME)
    # Get all source files for the target
    get_target_property(TARGET_SOURCES ${TARGET_NAME} SOURCES)

    # Convert paths to a list of files we want to format
    set(FORMAT_FILES "")
    foreach(SRC ${TARGET_SOURCES})
        if(SRC MATCHES "\\.(c|h|cpp|hpp|cc|hh)$")
            list(APPEND FORMAT_FILES ${SRC})
        endif()
    endforeach()

    # Create the custom target
    add_custom_target(${TARGET_NAME}_format
        COMMAND clang-format -i -style=file ${FORMAT_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Formatting ${TARGET_NAME} sources..."
        VERBATIM
    )
endfunction()