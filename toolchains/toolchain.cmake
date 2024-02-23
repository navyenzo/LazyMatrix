# toolchain.cmake

# If the VCPKG_ROOT environment variable is not set, display an error message and exit
if(NOT DEFINED ENV{VCPKG_ROOT})
    message(FATAL_ERROR "VCPKG_ROOT environment variable is not set. Please set it to the path of your vcpkg installation.")
endif()

# Set a flag to check if the toolchain file has already been included
if(NOT TOOLCHAIN_FILE_INCLUDED)
    set(TOOLCHAIN_FILE_INCLUDED TRUE)

    # Get the VCPKG_ROOT environment variable
    set(VCPKG_ROOT $ENV{VCPKG_ROOT} CACHE PATH "Path to vcpkg installation")

    # Form the path to the vcpkg toolchain file
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
    message(STATUS "USING VCPKG_ROOT: ${VCPKG_ROOT}")

    # Print the C and C++ compiler being used
    message(STATUS "C compiler: ${CMAKE_C_COMPILER}")
    message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
    message("CXX Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
    message("CXX Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")

    # Set the default target triplet
    set(DEFAULT_TARGET_TRIPLET "x64-windows-static-release" CACHE STRING "Default target triplet")

    # Determine the compiler and set the target triplet accordingly
    if(WIN32)
        message("Configuring project for Windows")
        if(MSVC)
            message("Configuring project for MSVC compiler")
            set(VCPKG_TARGET_TRIPLET ${DEFAULT_TARGET_TRIPLET} CACHE STRING "Vcpkg target triplet for MSVC" FORCE)
        elseif(CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            message("Configuring project for Clang")
            set(VCPKG_TARGET_TRIPLET ${DEFAULT_TARGET_TRIPLET} CACHE STRING "Vcpkg target triplet for Clang with MSVC" FORCE)
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
            # MinGW compiler
            set(VCPKG_TARGET_TRIPLET ${DEFAULT_TARGET_TRIPLET} CACHE STRING "Vcpkg target triplet for MinGW" FORCE)
        else()
            message(WARNING "Unknown compiler on Windows. Using default target triplet.")
            set(VCPKG_TARGET_TRIPLET ${DEFAULT_TARGET_TRIPLET} CACHE STRING "Vcpkg target triplet" FORCE)
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "Use static runtime libraries for all configurations")
        endif()
    elseif(UNIX AND NOT APPLE)
        message("Configuring project for Unix-like system")
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
            set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "Vcpkg target triplet for Linux with GCC" FORCE)
        else()
            message(FATAL_ERROR "Unsupported compiler on Unix-like system: ${CMAKE_CXX_COMPILER_ID}")
        endif()
    elseif(APPLE)
        message("Configuring project for macOS")
        set(VCPKG_TARGET_TRIPLET "x64-osx" CACHE STRING "Vcpkg target triplet for macOS" FORCE)
    else()
        message(FATAL_ERROR "Unsupported platform.")
    endif()

    # Optional: Specify MSVC runtime library settings for Windows
    if(MSVC)
        # Use MultiThreaded (static) runtime library for all configurations
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "Use static runtime libraries for all configurations")
    endif()

endif()
