# Ensure VCPKG_ROOT is set as a fallback
if(NOT DEFINED ENV{VCPKG_ROOT})
  message(WARNING "VCPKG_ROOT environment variable is not set. Falling back to other options if available.")
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
  # Prioritize MSYS2 if it's available
  elseif(DEFINED ENV{MSYS2_ROOT} AND EXISTS "$ENV{MSYS2_ROOT}")
    message(STATUS "MSYS2 detected. Configuring to use MSYS2 for package management.")
    # Set CMAKE_PREFIX_PATH to use MSYS2 paths
    set(CMAKE_PREFIX_PATH "$ENV{MSYS2_ROOT}/mingw64" ${CMAKE_PREFIX_PATH})
    # Note: Additional configuration specific to MSYS2 can be added here
  else()
    message(STATUS "Configuring for MinGW or another Windows compiler")
    set(VCPKG_TARGET_TRIPLET ${VCPKG_DEFAULT_TRIPLET_WINDOWS_MINGW} CACHE STRING "Vcpkg target triplet for MinGW or another Windows compiler" FORCE)
  endif()
elseif(UNIX AND NOT APPLE)
  set(VCPKG_TARGET_TRIPLET ${VCPKG_DEFAULT_TRIPLET_LINUX} CACHE STRING "Vcpkg target triplet for Linux" FORCE)
elseif(APPLE)
  set(VCPKG_TARGET_TRIPLET ${VCPKG_DEFAULT_TRIPLET_MAC} CACHE STRING "Vcpkg target triplet for macOS" FORCE)
else()
  message(FATAL_ERROR "Unsupported platform.")
endif()

# Set the CMAKE_TOOLCHAIN_FILE to use vcpkg if MSYS2 is not being explicitly used
if(NOT DEFINED ENV{MSYS2_ROOT} OR NOT EXISTS "$ENV{MSYS2_ROOT}")
  set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

# Debugging messages to verify configurations
message(STATUS "Using VCPKG_ROOT: ${VCPKG_ROOT}")
message(STATUS "Using MSYS2_ROOT: ${MSYS2_ROOT}")
message(STATUS "Vcpkg target triplet: ${VCPKG_TARGET_TRIPLET}")
