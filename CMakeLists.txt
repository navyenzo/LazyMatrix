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
    add_compile_options(-fexceptions) # Enable exceptions
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

# We need these compile definitions when compiling in windows
if(WIN32)
  add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX)
endif()
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
# Conditionally add the unixODBC include directory to the compiler flags
# if MSYS2 is being used to compile library
# if(DEFINED ENV{MSYS2_ROOT} AND NOT "$ENV{MSYS2_ROOT}" STREQUAL "")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I$ENV{MSYS2_ROOT}/mingw64/include/unixodbc")
# endif()

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
set(ENABLE_DATA_ODBC OFF CACHE INTERNAL "") # Disable ODBC for now as it's causing problems in windows
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
# Find all required packages
###############################################################
# find_package(ODBC)

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

# find_package(Poco CONFIG REQUIRED COMPONENTS Prometheus Encodings JWT Zip ActiveRecord DataMySQL DataSQLite Data Redis JSON XML Util NetSSL Crypto Net Foundation)
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
    include_directories(${Python3_INCLUDE_DIRS})
    # "${PocoFoundationIncludeDirs}"
    # "${PocoFoundationIncludeDirs}/Poco/Data"
    # ${ODBC_INCLUDE_DIRS}
    ${poco_SOURCE_DIR}/Foundation/include
    ${poco_SOURCE_DIR}/Util/include
    ${poco_SOURCE_DIR}/Net/include
    ${poco_SOURCE_DIR}/JSON/include
    ${poco_SOURCE_DIR}/XML/include
    ${poco_SOURCE_DIR}/Data/include
    ${poco_SOURCE_DIR}/Data/SQLite/include
    ${poco_SOURCE_DIR}/Data/MySQL/include
    # ${poco_SOURCE_DIR}/Data/ODBC/include
    ${poco_SOURCE_DIR}/Data/PostgreSQL/include
)
###############################################################



###############################################################
# In case we're using MSYS2 in windows, we need to set the path
# to the MariaDB because otherwise Poco::DataMySQL won't find
# MySQL::client target
###############################################################
if(WIN32)
    if(DEFINED ENV{MSYS2_ROOT} AND EXISTS "$ENV{MSYS2_ROOT}")
        # MariaDB Client Library
        set(MARIADB_LIBRARY_PATH "${MSYS2_ROOT}/mingw64/lib/libmariadb.a") # For static linking
        set(MARIADB_INCLUDE_DIR "${MSYS2_ROOT}/mingw64/include/mariadb")
        add_library(MySQL::client STATIC IMPORTED)
        set_target_properties(MySQL::client PROPERTIES
            IMPORTED_LOCATION "${MARIADB_LIBRARY_PATH}"
            INTERFACE_INCLUDE_DIRECTORIES "${MARIADB_INCLUDE_DIR}"
        )

        set(PG_LIBRARY_PATH "${MSYS2_ROOT}/mingw64/lib/libpq.dll.a") # Dynamic linking import library
        set(PG_IMPORT_LIB_PATH "${MSYS2_ROOT}/mingw64/lib/libpq.lib") # Adjust this path to the actual import library if different
        set(PG_INCLUDE_DIR "${MSYS2_ROOT}/mingw64/include")
        add_library(PostgreSQL::client SHARED IMPORTED)
        set_target_properties(PostgreSQL::client PROPERTIES
            IMPORTED_LOCATION "${PG_LIBRARY_PATH}"
            IMPORTED_IMPLIB "${PG_IMPORT_LIB_PATH}" # Set the IMPORTED_IMPLIB property
            INTERFACE_INCLUDE_DIRECTORIES "${PG_INCLUDE_DIR}"
        )
    endif()
endif()
###############################################################



###############################################################
# Linking
###############################################################
# target_link_libraries(${PROJECT_NAME} INTERFACE ${ODBC_LIBRARIES})

target_link_libraries(${PROJECT_NAME} INTERFACE dlib::dlib)

target_link_libraries(${PROJECT_NAME} INTERFACE LazyMatrix_ZLIB LazyMatrix_PNG LazyMatrix_JPEG LazyMatrix_GIF)

target_link_libraries(${PROJECT_NAME} INTERFACE Eigen3::Eigen)

# Link to Poco libraries
target_link_libraries(${PROJECT_NAME} INTERFACE Poco::Prometheus Poco::Encodings Poco::JWT Poco::Zip Poco::ActiveRecord Poco::DataMySQL Poco::DataSQLite Poco::Data Poco::Redis Poco::JSON Poco::XML Poco::Util Poco::NetSSL Poco::Crypto Poco::Net Poco::Foundation)

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