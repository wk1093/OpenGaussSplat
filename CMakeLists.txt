cmake_minimum_required(VERSION 3.15)
project(OpenGaussSplat VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Pull in project cmake helpers
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerFlags)
include(ShaderCompilation)
include(Platform)
include(Format)

# Dependencies
find_package(Vulkan REQUIRED)

# -------------------------------------------------------------------------
# Core library
# -------------------------------------------------------------------------
add_library(opengs SHARED
    include/ogs.h
    src/ogs_internal.h
    src/ogs_context.c
)
add_format_target(opengs)

target_include_directories(opengs
    PUBLIC  include   # consumers only see the public API
    PRIVATE src       # implementation can see ogs_internal.h
)

target_link_libraries(opengs PUBLIC Vulkan::Vulkan)

# Enable Vulkan validation layers in Debug builds
target_compile_definitions(opengs PRIVATE
    $<$<CONFIG:Debug>:OGS_DEBUG>
)

ogs_set_compiler_flags(opengs)

# -------------------------------------------------------------------------
# Test / sandbox executable
# -------------------------------------------------------------------------
add_executable(ogs_test tests/ogs_test.c)
add_format_target(ogs_test)
target_link_libraries(ogs_test PRIVATE opengs)
target_compile_definitions(ogs_test PRIVATE
    $<$<CONFIG:Debug>:OGS_DEBUG>
)
ogs_set_compiler_flags(ogs_test)