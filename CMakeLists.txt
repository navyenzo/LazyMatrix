cmake_minimum_required(VERSION 3.16)

project(LazyMatrix VERSION 1.0)

###############################################################
# Compiler options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

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

# Get the current build type
# This is used to make sure that
# any externalproject_add project
# uses the same build type as the
# main project
if(CMAKE_BUILD_TYPE)
    set(EXTERNAL_PROJECT_BUILD_TYPE ${CMAKE_BUILD_TYPE})
else()
    set(EXTERNAL_PROJECT_BUILD_TYPE "Release")  # Default to Release if not set
endif()

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
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        master
)

FetchContent_MakeAvailable(pybind11)
###############################################################



###############################################################
# Poco library
###############################################################
ExternalProject_Add(Poco
    GIT_REPOSITORY https://github.com/pocoproject/poco.git
    GIT_TAG devel
    CMAKE_ARGS
        ${EXTERNAL_PROJECT_COMPILER_ARGS}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/_deps/poco
        -DBUILD_SHARED_LIBS=OFF
        -DCMAKE_BUILD_TYPE=${EXTERNAL_PROJECT_BUILD_TYPE}
        -DENABLE_FOUNDATION=ON
        -DENABLE_DATA=ON
        -DENABLE_DATA_ODBC=ON
        -DENABLE_DATA_SQLITE=ON
        -DENABLE_DATA_MYSQL=ON
        -DENABLE_DATA_POSTGRESQL=ON
        -DENABLE_ZIP=ON
        -DENABLE_PAGECOMPILER=ON
        -DENABLE_PAGECOMPILER_FILE2PAGE=ON
)


include_directories(${CMAKE_BINARY_DIR}/_deps/poco/include)

link_directories(${CMAKE_BINARY_DIR}/_deps/poco/lib)
###############################################################



###############################################################
# SQLParser library (needed for Poco library)
###############################################################
FetchContent_Declare(
  sqlparser
  GIT_REPOSITORY https://github.com/hyrise/sql-parser.git
  GIT_TAG master
)
FetchContent_MakeAvailable(sqlparser)

include_directories(${CMAKE_BINARY_DIR}/_deps)

# Copy the "sql-parser-src" to the "sql-parser" directory as
# this is needed for the Poco library
file(COPY ${CMAKE_BINARY_DIR}/_deps/sqlparser-src/src 
     DESTINATION ${CMAKE_BINARY_DIR}/_deps/sql-parser)
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
# Specify the include directories for the interface library
###############################################################
target_include_directories(${PROJECT_NAME} INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${CMAKE_BINARY_DIR}/_deps
    ${CMAKE_BINARY_DIR}/_deps/mio-src
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

# Ensure LazyMatrix is built after Poco is built
add_dependencies(${PROJECT_NAME} Poco)

# Link to Poco libraries
target_link_libraries(${PROJECT_NAME} INTERFACE
    "$<IF:$<CONFIG:Debug>,PocoPrometheusd,PocoPrometheus>"
    "$<IF:$<CONFIG:Debug>,PocoEncodingsd,PocoEncodings>"
    "$<IF:$<CONFIG:Debug>,PocoJWTd,PocoJWT>"
    "$<IF:$<CONFIG:Debug>,PocoZipd,PocoZip>"
    "$<IF:$<CONFIG:Debug>,PocoActiveRecordd,PocoActiveRecord>"
    "$<IF:$<CONFIG:Debug>,PocoMongoDBd,PocoMongoDB>"
    "$<IF:$<CONFIG:Debug>,PocoDataPostgreSQLd,PocoDataPostgreSQL>"
    "$<IF:$<CONFIG:Debug>,PocoDataMySQLd,PocoDataMySQL>"
    "$<IF:$<CONFIG:Debug>,PocoDataODBCd,PocoDataODBC>"
    "$<IF:$<CONFIG:Debug>,PocoDataSQLited,PocoDataSQLite>"
    "$<IF:$<CONFIG:Debug>,PocoDatad,PocoData>"
    "$<IF:$<CONFIG:Debug>,PocoRedisd,PocoRedis>"
    "$<IF:$<CONFIG:Debug>,PocoJSONd,PocoJSON>"
    "$<IF:$<CONFIG:Debug>,PocoXMLd,PocoXML>"
    "$<IF:$<CONFIG:Debug>,PocoUtild,PocoUtil>"
    "$<IF:$<CONFIG:Debug>,PocoNetSSLd,PocoNetSSL>"
    "$<IF:$<CONFIG:Debug>,PocoCryptod,PocoCrypto>"
    "$<IF:$<CONFIG:Debug>,PocoNetd,PocoNet>"
    "$<IF:$<CONFIG:Debug>,PocoFoundationd,PocoFoundation>"
)

# Link against libdl
target_link_libraries(${PROJECT_NAME} INTERFACE dl)

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