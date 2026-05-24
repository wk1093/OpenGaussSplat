# CompilerFlags.cmake
# Applies strict, consistent compiler warnings to a target.

function(ogs_set_compiler_flags target)
    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W4
            /WX
            /wd4996  # Suppress deprecation warnings for portable C functions
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wconversion
            -Wsign-conversion
            -Wmissing-prototypes
            -Wstrict-prototypes
        )
    endif()
endfunction()
