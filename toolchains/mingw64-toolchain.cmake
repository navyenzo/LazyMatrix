message(STATUS "Using mingw64-toolchain.cmake")

# Set the system name to ensure CMake uses the correct system compilers
set(CMAKE_SYSTEM_NAME Windows)

# Specify the absolute paths to the C and C++ compilers from MinGW-w64
set(CMAKE_C_COMPILER C:/mingw64/bin/gcc.exe)
set(CMAKE_CXX_COMPILER C:/mingw64/bin/g++.exe)

# Specify the root path where CMake can find the MinGW-w64 libraries and headers
set(CMAKE_FIND_ROOT_PATH C:/mingw64)

# Adjust the default behavior of the FIND commands:
# - NEVER search for programs in the system PATH.
# - ONLY search for libraries and headers in the CMAKE_FIND_ROOT_PATH.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Ensure that CMake uses the MinGW-w64 toolchain for both compiling and linking
set(CMAKE_C_COMPILER_TARGET x86_64-w64-mingw32)
set(CMAKE_CXX_COMPILER_TARGET x86_64-w64-mingw32)
