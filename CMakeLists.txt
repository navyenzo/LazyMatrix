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
FetchContent_Declare(
  dlib
  GIT_REPOSITORY https://github.com/davisking/dlib.git
  GIT_TAG master
)
FetchContent_MakeAvailable(dlib)

# Define Dlib image support
add_definitions(-DDLIB_PNG_SUPPORT)
add_definitions(-DDLIB_JPEG_SUPPORT)
add_definitions(-DDLIB_GIF_SUPPORT)
###############################################################



###############################################################
# Build unit tests or not
###############################################################
# Option to build or not build the provided tests
option(LazyMatrix_BUILD_TESTS "Build the tests." ON)

message("LazyMatrix_BUILD_TESTS option: " ${BUILD_TESTS})

if(${LazyMatrix_BUILD_TESTS})
    enable_testing()
    add_subdirectory(tests)
endif()
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
)
###############################################################



###############################################################
# dlib related linking
###############################################################
# Link dlib to LazyMatrix
target_link_libraries(${PROJECT_NAME} INTERFACE dlib::dlib)

# Find PNG library
find_package(PNG REQUIRED)
if(PNG_FOUND)
  target_link_libraries(${PROJECT_NAME} INTERFACE ${PNG_LIBRARIES})
  target_include_directories(${PROJECT_NAME} INTERFACE ${PNG_INCLUDE_DIRS})
endif()

# Find JPEG library
find_package(JPEG REQUIRED)
if(JPEG_FOUND)
  target_link_libraries(${PROJECT_NAME} INTERFACE ${JPEG_LIBRARIES})
  target_include_directories(${PROJECT_NAME} INTERFACE ${JPEG_INCLUDE_DIR})
endif()

# Find GIF library
find_package(GIF REQUIRED)
if(GIF_FOUND)
  target_link_libraries(${PROJECT_NAME} INTERFACE ${GIF_LIBRARIES})
  target_include_directories(${PROJECT_NAME} INTERFACE ${GIF_INCLUDE_DIR})
endif()
###############################################################



###############################################################
# Linker options for Windows or other platforms
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