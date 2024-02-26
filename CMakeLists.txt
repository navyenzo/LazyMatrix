cmake_minimum_required(VERSION 3.16)

project(LazyMatrix VERSION 1.0)



###############################################################
# Check if the MSVC compiler is used
if(MSVC)
  # Apply MSVC-specific flags
  add_compile_options(/bigobj)
else()
  # Apply flags for other compilers if necessary
  # GCC or Clang specific flags can go here
endif()

# Compiler options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

# Set the output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Define common arguments for external projects
# These are used to force the external projects
# to use the same compiler that is used to build
# the main project
set(EXTERNAL_PROJECT_COMPILER_ARGS
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
)

# These modules are needed for external libraries
include(FetchContent)
include(ExternalProject)
###############################################################



###############################################################
# Include toolchain file
include(toolchains/toolchain.cmake)

# Print the current build type/target
message(STATUS "Current Built Type for project LazyMatrix: ${CMAKE_BUILD_TYPE}")

message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "Using VCPKG_TARGET_TRIPLET: ${VCPKG_TARGET_TRIPLET}")

set(CMAKE_PREFIX_PATH "${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}" ${CMAKE_PREFIX_PATH})
message("CMAKE PREFIX PATH: ${CMAKE_PREFIX_PATH}")
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
# ExprTk library for evaluating expression writtent in text
###############################################################
FetchContent_Declare(
  exprtk
  GIT_REPOSITORY https://github.com/ArashPartow/exprtk.git
  GIT_TAG master
)

FetchContent_MakeAvailable(exprtk)
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
# Poco library
###############################################################
# Include the FetchContent module
include(FetchContent)

# Declare the FetchContent for Poco
FetchContent_Declare(
  Poco
  GIT_REPOSITORY https://github.com/pocoproject/poco.git
  GIT_TAG devel
)

# Set the Poco specific arguments before calling FetchContent_MakeAvailable
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE INTERNAL "")
set(ENABLE_FOUNDATION ON CACHE INTERNAL "")
set(ENABLE_DATA ON CACHE INTERNAL "")
set(ENABLE_DATA_ODBC ON CACHE INTERNAL "")
set(ENABLE_DATA_SQLITE ON CACHE INTERNAL "")
set(ENABLE_DATA_MYSQL ON CACHE INTERNAL "")
set(ENABLE_DATA_POSTGRESQL ON CACHE INTERNAL "")
set(ENABLE_ZIP ON CACHE INTERNAL "")
set(ENABLE_PAGECOMPILER ON CACHE INTERNAL "")
set(ENABLE_PAGECOMPILER_FILE2PAGE ON CACHE INTERNAL "")

# Make Poco available for the current project
FetchContent_MakeAvailable(Poco)
###############################################################



###############################################################
# Define LazyMatrix as an interface library
###############################################################
add_library(${PROJECT_NAME} INTERFACE)
###############################################################



###############################################################
# Add external project dependencies
###############################################################
add_dependencies(${PROJECT_NAME} Poco)
###############################################################



###############################################################
# Find all required packages
###############################################################
find_package(dlib CONFIG REQUIRED)

find_package(ZLIB REQUIRED MODULE)
find_package(PNG REQUIRED MODULE)
find_package(JPEG REQUIRED MODULE)
find_package(GIF REQUIRED MODULE)

# Create alias targets for each dependency
add_library(LazyMatrix_ZLIB ALIAS ZLIB::ZLIB)
add_library(LazyMatrix_PNG ALIAS PNG::PNG)
add_library(LazyMatrix_JPEG ALIAS JPEG::JPEG)
add_library(LazyMatrix_GIF ALIAS GIF::GIF)

find_package(Eigen3 CONFIG REQUIRED)


find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# find_package(Poco CONFIG REQUIRED COMPONENTS Foundation Util Net NetSSL Crypto DataSQLite Data JSON XML MongoDB Redis)
# get_target_property(PocoFoundationIncludeDirs Poco::Foundation INTERFACE_INCLUDE_DIRECTORIES)
###############################################################



###############################################################
# Specify the include directories for the interface library
###############################################################
target_include_directories(${PROJECT_NAME} INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${CMAKE_BINARY_DIR}/_deps
    ${CMAKE_BINARY_DIR}/_deps/mio-src
    ${CMAKE_BINARY_DIR}/_deps/exprtk-src
    ${CMAKE_BINARY_DIR}/_deps/eigen-src
    ${CMAKE_BINARY_DIR}/_deps/xoshiro-src
    ${CMAKE_BINARY_DIR}/_deps/dlib-src
    ${pybind11_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/_deps/poco-src/Foundation/include
    ${CMAKE_BINARY_DIR}/_deps/poco-src/Data/include
    ${CMAKE_BINARY_DIR}/_deps/poco-src/Encodings/include
    ${CMAKE_BINARY_DIR}/_deps/poco-src/JSON/include
    ${CMAKE_BINARY_DIR}/_deps/poco-src/JWT/include
    ${CMAKE_BINARY_DIR}/_deps/poco-src/MongoDB/include
    ${CMAKE_BINARY_DIR}/_deps/poco-src/Net/include
    ${CMAKE_BINARY_DIR}/_deps/poco-src/NetSSL_OpenSSL/include
    ${CMAKE_BINARY_DIR}/_deps/poco-src/NetSSL_Win/include
    ${CMAKE_BINARY_DIR}/_deps/poco-src/PDF/include
    ${CMAKE_BINARY_DIR}/_deps/poco-src/Prometheus/include
    ${CMAKE_BINARY_DIR}/_deps/poco-src/Redis/include
    ${CMAKE_BINARY_DIR}/_deps/poco-src/SevenZip/include
    ${CMAKE_BINARY_DIR}/_deps/poco-src/Util/include
    ${CMAKE_BINARY_DIR}/_deps/poco-src/XML/include
    ${CMAKE_BINARY_DIR}/_deps/poco-src/Zip/include
    ${CMAKE_BINARY_DIR}/_deps/poco-src/XML/include
    ${CMAKE_BINARY_DIR}/_deps/poco-src/XML/include
    ${CMAKE_BINARY_DIR}/_deps/poco-src/XML/include
    include_directories(${Python3_INCLUDE_DIRS})
)
###############################################################



###############################################################
# Linking
###############################################################
target_link_libraries(${PROJECT_NAME} INTERFACE dlib::dlib)

target_link_libraries(${PROJECT_NAME} INTERFACE LazyMatrix_ZLIB LazyMatrix_PNG LazyMatrix_JPEG LazyMatrix_GIF)

target_link_libraries(${PROJECT_NAME} INTERFACE Eigen3::Eigen)

# target_link_libraries(${PROJECT_NAME} INTERFACE Poco::Foundation Poco::Util Poco::Net Poco::NetSSL Poco::Crypto Poco::DataSQLite Poco::Data Poco::JSON Poco::XML Poco::MongoDB Poco::Redis)

# Link to Poco libraries
target_link_libraries(${PROJECT_NAME} INTERFACE
    Poco::Prometheus
    Poco::Encodings
    Poco::JWT
    Poco::Zip
    Poco::ActiveRecord
    Poco::MongoDB
    Poco::DataPostgreSQL
    Poco::DataMySQL
    Poco::DataODBC
    Poco::DataSQLite
    Poco::Data
    Poco::Redis
    Poco::JSON
    Poco::XML
    Poco::Util
    Poco::NetSSL
    Poco::Crypto
    Poco::Net
    Poco::Foundation
)

if(UNIX AND NOT APPLE)
    # Link against libdl only on UNIX-like systems (excluding macOS)
    target_link_libraries(${PROJECT_NAME} INTERFACE dl)
endif()

target_link_libraries(${PROJECT_NAME} INTERFACE pybind11::pybind11 Python3::Python Python3::Module)

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