cmake_minimum_required(VERSION 3.16)

project(LazyMatrix VERSION 1.0)

###############################################################
# Compiler options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

# Set the output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# These modules are needed for external libraries
include(FetchContent)
include(ExternalProject)
###############################################################



###############################################################
# mio library
###############################################################
FetchContent_Declare(
  mio
  GIT_REPOSITORY https://github.com/vimpunk/mio.git
  GIT_TAG master
)
FetchContent_MakeAvailable(mio)
###############################################################



###############################################################
# eigen library
###############################################################
FetchContent_Declare(
  eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG master
)
FetchContent_MakeAvailable(eigen)
###############################################################



###############################################################
# Xoshiro-cpp library
###############################################################
FetchContent_Declare(
  xoshiro
  GIT_REPOSITORY https://github.com/Reputeless/Xoshiro-cpp.git
  GIT_TAG master
)
FetchContent_MakeAvailable(xoshiro)
###############################################################



###############################################################
# dlib library
###############################################################
# Before declaring dlib or any other library
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Fetch dlib library
FetchContent_Declare(
  dlib
  GIT_REPOSITORY https://github.com/davisking/dlib.git
  GIT_TAG master
)

# Set required definitions for dlib image support
set(DLIB_PNG_SUPPORT ON CACHE BOOL "Enable PNG support in dlib" FORCE)
set(DLIB_JPEG_SUPPORT ON CACHE BOOL "Enable JPEG support in dlib" FORCE)
set(DLIB_GIF_SUPPORT ON CACHE BOOL "Enable GIF support in dlib" FORCE)

FetchContent_MakeAvailable(dlib)
###############################################################




###############################################################
# pybind11 (For creating python interfaces to LazyMatrix objects)
###############################################################
include(FetchContent)

FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        master
)

FetchContent_MakeAvailable(pybind11)
###############################################################



###############################################################
# Define LazyMatrix as an interface library
###############################################################
add_library(${PROJECT_NAME} INTERFACE)
###############################################################



###############################################################
# Specify the include directories for the interface library
###############################################################
target_include_directories(${PROJECT_NAME} INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${CMAKE_BINARY_DIR}/_deps/mio-src
    ${CMAKE_BINARY_DIR}/_deps/eigen-src
    ${CMAKE_BINARY_DIR}/_deps/xoshiro-src
    ${CMAKE_BINARY_DIR}/_deps/dlib-src
    ${pybind11_SOURCE_DIR}/include
    include_directories(${Python3_INCLUDE_DIRS})
)
###############################################################



###############################################################
# dlib related linking
###############################################################
# Link dlib to LazyMatrix
target_link_libraries(${PROJECT_NAME} INTERFACE dlib::dlib)

# Find the required libraries for non-vcpkg (particularly on Linux)
find_package(ZLIB REQUIRED MODULE)
find_package(PNG REQUIRED MODULE)
find_package(JPEG REQUIRED MODULE)
find_package(GIF REQUIRED MODULE)

# Create alias targets for each dependency
add_library(LazyMatrix_ZLIB ALIAS ZLIB::ZLIB)
add_library(LazyMatrix_PNG ALIAS PNG::PNG)
add_library(LazyMatrix_JPEG ALIAS JPEG::JPEG)
add_library(LazyMatrix_GIF ALIAS GIF::GIF)

# Link the alias libraries to LazyMatrix
target_link_libraries(${PROJECT_NAME} INTERFACE 
    LazyMatrix_ZLIB 
    LazyMatrix_PNG 
    LazyMatrix_JPEG 
    LazyMatrix_GIF
)

# Find the required libraries for non-vcpkg (particularly on Linux)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Link python libraries to LazyMatrix
target_link_libraries(${PROJECT_NAME} INTERFACE Python3::Python Python3::Module)
###############################################################



###############################################################
# Linker options for Windows or other platforms
###############################################################
if(WIN32)
    target_link_libraries(${PROJECT_NAME} INTERFACE stdc++fs)
else()
    target_link_libraries(${PROJECT_NAME} INTERFACE stdc++fs pthread)
endif()
###############################################################



###############################################################
# Add threading support
###############################################################
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE Threads::Threads)
###############################################################



###############################################################
# Build unit tests or not
###############################################################
# Option to build or not build the provided tests
option(LazyMatrix_BUILD_TESTS "Build the tests." ON)

message("LazyMatrix_BUILD_TESTS option: " ${LazyMatrix_BUILD_TESTS})

if(${LazyMatrix_BUILD_TESTS})
    enable_testing()
    add_subdirectory(tests)
    add_subdirectory(python_tests)
endif()
###############################################################