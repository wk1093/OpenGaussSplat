# ShaderCompilation.cmake
# Provides the ogs_compile_shader() function to compile GLSL to SPIR-V.
# Prefers glslc (Vulkan SDK) and falls back to glslangValidator.

find_program(GLSLC NAMES glslc HINTS $ENV{VULKAN_SDK}/bin)
find_program(GLSLANG_VALIDATOR NAMES glslangValidator HINTS $ENV{VULKAN_SDK}/bin)

if(GLSLC)
    message(STATUS "OGS: Shader compiler found (glslc): ${GLSLC}")
elseif(GLSLANG_VALIDATOR)
    message(STATUS "OGS: Shader compiler found (glslangValidator): ${GLSLANG_VALIDATOR}")
else()
    message(WARNING "OGS: No shader compiler found. Shaders will not be compiled.")
endif()

# Usage: ogs_compile_shader(<target> <path/to/shader.comp>)
# Compiles the given shader and adds the .spv output as a source dependency on <target>.
function(ogs_compile_shader target shader_src)
    get_filename_component(shader_name ${shader_src} NAME)
    set(output_file "${CMAKE_CURRENT_BINARY_DIR}/shaders/${shader_name}.spv")

    if(GLSLC)
        add_custom_command(
            OUTPUT ${output_file}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/shaders"
            COMMAND ${GLSLC} -o ${output_file} ${shader_src}
            DEPENDS ${shader_src}
            COMMENT "Compiling shader: ${shader_name}"
            VERBATIM
        )
    elseif(GLSLANG_VALIDATOR)
        add_custom_command(
            OUTPUT ${output_file}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/shaders"
            COMMAND ${GLSLANG_VALIDATOR} -V -o ${output_file} ${shader_src}
            DEPENDS ${shader_src}
            COMMENT "Compiling shader: ${shader_name}"
            VERBATIM
        )
    else()
        message(WARNING "OGS: Skipping shader compilation for ${shader_name}: no compiler found.")
        return()
    endif()

    target_sources(${target} PRIVATE ${output_file})
endfunction()
