# Platform.cmake
# Detects the target platform and sets OGS_PLATFORM_* compile definitions.

if(WIN32)
    message(STATUS "OGS: Target platform: Windows")
    add_compile_definitions(OGS_PLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    message(STATUS "OGS: Target platform: Linux")
    add_compile_definitions(OGS_PLATFORM_LINUX)
elseif(APPLE)
    message(STATUS "OGS: Target platform: macOS")
    add_compile_definitions(OGS_PLATFORM_MACOS)
else()
    message(WARNING "OGS: Unknown target platform.")
endif()
