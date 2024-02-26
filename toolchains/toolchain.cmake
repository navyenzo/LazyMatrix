# Ensure VCPKG_ROOT is set as a fallback
if(NOT DEFINED ENV{VCPKG_ROOT})
    message(WARNING "VCPKG_ROOT environment variable is not set. vcpkg will be used as a fallback if MSYS2_ROOT is not set.")
endif()

# Set VCPKG_ROOT from the environment variable
set(VCPKG_ROOT $ENV{VCPKG_ROOT} CACHE PATH "Path to vcpkg installation")

# New: Check for MSYS2_ROOT environment variable
if(NOT DEFINED ENV{MSYS2_ROOT})
    message(WARNING "MSYS2_ROOT environment variable is not set. Falling back to vcpkg if necessary.")
else()
    set(MSYS2_ROOT $ENV{MSYS2_ROOT} CACHE PATH "Path to MSYS2 installation")
endif()

# Default triplet values for vcpkg
set(VCPKG_DEFAULT_TRIPLET_LINUX "x64-linux")
set(VCPKG_DEFAULT_TRIPLET_WINDOWS "x64-windows-static-release")
set(VCPKG_DEFAULT_TRIPLET_WINDOWS_MINGW "x64-mingw-static")
set(VCPKG_DEFAULT_TRIPLET_MAC "x64-osx")

# Detect operating system and compiler
if(WIN32)
    if(MSVC)
        message(STATUS "Configuring for MSVC")
        set(VCPKG_TARGET_TRIPLET ${VCPKG_DEFAULT_TRIPLET_WINDOWS} CACHE STRING "Vcpkg target triplet for MSVC" FORCE)
    else()
        message(STATUS "Configuring for MinGW or another Windows compiler")
        # Check if MSYS2_ROOT is defined and prefer it over vcpkg for MinGW
        if(DEFINED MSYS2_ROOT AND EXISTS "${MSYS2_ROOT}")
            message(STATUS "Configuring to use MSYS2 libraries at ${MSYS2_ROOT}")
            # Set paths to prefer MSYS2 installations
            set(CMAKE_PREFIX_PATH "${MSYS2_ROOT}/mingw64" ${CMAKE_PREFIX_PATH})
            # Additional MSYS2-specific configuration can go here
        else()
            set(VCPKG_TARGET_TRIPLET ${VCPKG_DEFAULT_TRIPLET_WINDOWS_MINGW} CACHE STRING "Vcpkg target triplet for MinGW or another Windows compiler" FORCE)
        endif()
    endif()
elseif(UNIX AND NOT APPLE)
    set(VCPKG_TARGET_TRIPLET ${VCPKG_DEFAULT_TRIPLET_LINUX} CACHE STRING "Vcpkg target triplet for Linux" FORCE)
elseif(APPLE)
    set(VCPKG_TARGET_TRIPLET ${VCPKG_DEFAULT_TRIPLET_MAC} CACHE STRING "Vcpkg target triplet for macOS" FORCE)
else()
    message(FATAL_ERROR "Unsupported platform.")
endif()

# Set the CMAKE_TOOLCHAIN_FILE to use vcpkg if MSYS2 is not being used
if(NOT DEFINED MSYS2_ROOT OR NOT EXISTS "${MSYS2_ROOT}")
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

# Debugging messages to verify configurations
message(STATUS "Using VCPKG_ROOT: ${VCPKG_ROOT}")
message(STATUS "Using MSYS2_ROOT: ${MSYS2_ROOT}")
message(STATUS "Vcpkg target triplet: ${VCPKG_TARGET_TRIPLET}")
