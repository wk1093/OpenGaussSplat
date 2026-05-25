file(GLOB OGS_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/ogs_test*.c"
)

list(REMOVE_ITEM OGS_TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/ogs_test_support.c"
)

list(SORT OGS_TEST_SOURCES)

set(OGS_TEST_TARGETS)

foreach(test_source IN LISTS OGS_TEST_SOURCES)
    get_filename_component(test_name "${test_source}" NAME_WE)

    add_executable(${test_name}
        "${CMAKE_CURRENT_SOURCE_DIR}/ogs_test_support.c"
        "${test_source}"
    )
    target_include_directories(${test_name} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    target_link_libraries(${test_name} PRIVATE opengs)
    target_compile_definitions(${test_name} PRIVATE
        $<$<CONFIG:Debug>:OGS_DEBUG>
    )
    ogs_set_compiler_flags(${test_name})

    add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${test_name}>)
    list(APPEND OGS_TEST_TARGETS ${test_name})
endforeach()

add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --test-dir "${CMAKE_CURRENT_BINARY_DIR}"
    DEPENDS ${OGS_TEST_TARGETS}
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)