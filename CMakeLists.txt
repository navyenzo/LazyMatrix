cmake_minimum_required(VERSION 3.16)

project(LazyMatrix VERSION 1.0)

# Print the current build type/target
message(STATUS "Current Built Type for project LazyMatrix: ${CMAKE_BUILD_TYPE}")

message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "Using VCPKG_TARGET_TRIPLET: ${VCPKG_TARGET_TRIPLET}")

set(CMAKE_PREFIX_PATH "${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}" ${CMAKE_PREFIX_PATH})
message("CMAKE PREFIX PATH: ${CMAKE_PREFIX_PATH}")



###############################################################
# Check if the MSVC compiler is used
if(MSVC)
  message("adding /bigobj compile option")
  add_compile_options(/bigobj)
endif()

# Compiler options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Adjust or remove flags for Clang
    # Here, you don't add /bigobj, so it's effectively "removed" for Clang builds
    # You can add Clang-specific flags here if necessary
else()
    # For MSVC or other compilers, /bigobj is added
    add_compile_options(/bigobj)
endif()

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



# ###############################################################
# # SQLParser library (needed for Poco library)
# ###############################################################
# FetchContent_Declare(
#   sqlparser
#   GIT_REPOSITORY https://github.com/hyrise/sql-parser.git
#   GIT_TAG master
# )
# FetchContent_MakeAvailable(sqlparser)

# include_directories(${CMAKE_BINARY_DIR}/_deps)

# # Copy the "sql-parser-src" to the "sql-parser" directory as
# # this is needed for the Poco library
# file(COPY ${CMAKE_BINARY_DIR}/_deps/sqlparser-src/src 
#      DESTINATION ${CMAKE_BINARY_DIR}/_deps/sql-parser)
# ###############################################################



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
    ${CMAKE_BINARY_DIR}/_deps/poco/include
    include_directories(${Python3_INCLUDE_DIRS})
)
###############################################################



###############################################################
# Linking
###############################################################
# Linking dlib
find_package(dlib CONFIG REQUIRED)
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

# Link to Eigen library
find_package(Eigen3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE Eigen3::Eigen)

# Find the Poco libraries
find_package(Poco CONFIG REQUIRED COMPONENTS
    Foundation
    Util
    Net
    NetSSL
    Crypto
    DataSQLite
    Data
    JSON
    XML
    MongoDB
    Redis
)

target_link_libraries(${PROJECT_NAME} INTERFACE
    Poco::Foundation
    Poco::Util
    Poco::Net
    Poco::NetSSL
    Poco::Crypto
    Poco::DataSQLite
    Poco::Data
    Poco::JSON
    Poco::XML
    Poco::MongoDB
    Poco::Redis
)

if(UNIX AND NOT APPLE)
    # Link against libdl only on UNIX-like systems (excluding macOS)
    target_link_libraries(${PROJECT_NAME} INTERFACE dl)
endif()

# Find the required libraries for non-vcpkg (particularly on Linux)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG)
target_link_libraries(${PROJECT_NAME} INTERFACE pybind11::pybind11 Python3::Python Python3::Module)
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