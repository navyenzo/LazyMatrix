cmake_minimum_required(VERSION 3.16)



project(LazyMatrix VERSION 1.0)



# Compiler options
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")



# Set the output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)



# mio library used for cross-platform memory mapping
include(FetchContent)
FetchContent_Declare(
  mio
  GIT_REPOSITORY https://github.com/vimpunk/mio.git
  GIT_TAG master
)
FetchContent_MakeAvailable(mio)



# eigen library used for matrix computations
include(FetchContent)
FetchContent_Declare(
  eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG master
)
FetchContent_MakeAvailable(eigen)



# Xoshiro-cpp library used random number generation
include(FetchContent)
FetchContent_Declare(
  xoshiro
  GIT_REPOSITORY https://github.com/Reputeless/Xoshiro-cpp.git
  GIT_TAG master
)
FetchContent_MakeAvailable(xoshiro)



# stb library used for loading/handling images
include(FetchContent)
FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master
)
FetchContent_MakeAvailable(stb)



# dlib library used machine learning algorithms
include(FetchContent)
FetchContent_Declare(
  dlib
  GIT_REPOSITORY https://github.com/davisking/dlib.git
  GIT_TAG master
)
FetchContent_MakeAvailable(dlib)



# Option to build or not build the provided tests
option(BUILD_TESTS "Build the tests." OFF)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()



# Add project source files
# Make sure to include the stb_image_wrapper.cpp in your source directory
file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)



# Add LazyMatrix as a library
add_library(${PROJECT_NAME} ${SOURCE_FILES})



# Add the include directories
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_BINARY_DIR}/_deps/mio-src
    ${CMAKE_BINARY_DIR}/_deps/eigen-src
    ${CMAKE_BINARY_DIR}/_deps/xoshiro-src
    ${CMAKE_BINARY_DIR}/_deps/stb-src
    ${CMAKE_BINARY_DIR}/_deps/dlib-src
    ${CMAKE_SOURCE_DIR}/include
)



# Add the include directories
target_include_directories(${PROJECT_NAME} PUBLIC include)



# Linker options
# Linking libraries (if needed)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} stdc++fs)
else()
    target_link_libraries(${PROJECT_NAME} stdc++fs pthread)
endif()



# Add threading support
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
