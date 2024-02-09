# the name of the target operating system
set(CMAKE_SYSTEM_NAME Windows)

# the path to the target environment
# (You might need to adjust this path based on your MinGW installation)
set(CMAKE_SYSROOT /usr/x86_64-w64-mingw32)

# specify the cross compiler
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

# where to look for the target binaries (this tells CMake where to find the target environment)
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

# adjust the default behaviour of the find commands: search headers and libraries in the target environment, programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
