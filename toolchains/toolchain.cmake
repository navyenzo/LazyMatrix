# Ensure VCPKG_ROOT is set as a fallback
if(NOT DEFINED ENV{VCPKG_ROOT})
  message(WARNING "VCPKG_ROOT environment variable is not set. Falling back to default paths if available.")
endif()

# Set VCPKG_ROOT from the environment variable
set(VCPKG_ROOT $ENV{VCPKG_ROOT} CACHE PATH "Path to vcpkg installation")

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
    set(VCPKG_TARGET_TRIPLET ${VCPKG_DEFAULT_TRIPLET_WINDOWS_MINGW} CACHE STRING "Vcpkg target triplet for MinGW or another Windows compiler" FORCE)
  endif()
elseif(UNIX AND NOT APPLE)
  set(VCPKG_TARGET_TRIPLET ${VCPKG_DEFAULT_TRIPLET_LINUX} CACHE STRING "Vcpkg target triplet for Linux" FORCE)
elseif(APPLE)
  set(VCPKG_TARGET_TRIPLET ${VCPKG_DEFAULT_TRIPLET_MAC} CACHE STRING "Vcpkg target triplet for macOS" FORCE)
else()
  message(FATAL_ERROR "Unsupported platform.")
endif()

# Set the CMAKE_TOOLCHAIN_FILE to use vcpkg
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Debugging messages to verify configurations
message(STATUS "Using VCPKG_ROOT: ${VCPKG_ROOT}")
message(STATUS "Vcpkg target triplet: ${VCPKG_TARGET_TRIPLET}")
