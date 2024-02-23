# Ensure VCPKG_ROOT is set
if(NOT DEFINED ENV{VCPKG_ROOT})
    message(FATAL_ERROR "VCPKG_ROOT environment variable is not set. Please set it to the path of your vcpkg installation.")
endif()

# Set VCPKG_ROOT from the environment variable
set(VCPKG_ROOT $ENV{VCPKG_ROOT} CACHE PATH "Path to vcpkg installation")

# Default triplet values
set(VCPKG_DEFAULT_TRIPLET_LINUX "x64-linux")
set(VCPKG_DEFAULT_TRIPLET_WINDOWS "x64-windows-static")
set(VCPKG_DEFAULT_TRIPLET_WINDOWS_MINGW "x64-mingw-static")
set(VCPKG_DEFAULT_TRIPLET_MAC "x64-osx")

# Detect operating system
if(WIN32)
    # Check if using MSVC or another compiler (e.g., MinGW)
    if(MSVC)
        message(STATUS "Configuring for MSVC")
        set(VCPKG_TARGET_TRIPLET ${VCPKG_DEFAULT_TRIPLET_WINDOWS} CACHE STRING "Vcpkg target triplet for MSVC" FORCE)
        
        # Specify MSVC runtime library settings for Windows
        # Use MultiThreaded (static) runtime library for all configurations
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "Use static runtime libraries for all configurations")
    else()
        message(STATUS "Configuring for MinGW or another Windows compiler")
        set(VCPKG_TARGET_TRIPLET ${VCPKG_DEFAULT_TRIPLET_WINDOWS_MINGW} CACHE STRING "Vcpkg target triplet for MinGW or another Windows compiler" FORCE)
    endif()
elseif(UNIX AND NOT APPLE)
    # Linux-specific configuration
    set(VCPKG_TARGET_TRIPLET ${VCPKG_DEFAULT_TRIPLET_LINUX} CACHE STRING "Vcpkg target triplet for Linux" FORCE)
elseif(APPLE)
    # macOS-specific configuration
    set(VCPKG_TARGET_TRIPLET ${VCPKG_DEFAULT_TRIPLET_MAC} CACHE STRING "Vcpkg target triplet for macOS" FORCE)
else()
    message(FATAL_ERROR "Unsupported platform.")
endif()

# Now set the CMAKE_TOOLCHAIN_FILE to use vcpkg
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Debugging messages to verify correct triplet selection
message(STATUS "Using VCPKG_ROOT: ${VCPKG_ROOT}")
message(STATUS "Vcpkg target triplet: ${VCPKG_TARGET_TRIPLET}")
