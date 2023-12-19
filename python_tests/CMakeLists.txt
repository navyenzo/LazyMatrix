cmake_minimum_required(VERSION 3.16)
project(lazymatrix_python_tests)



# Catch2 library used for testing
include(FetchContent)
FetchContent_Declare(
  catch
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG devel
)
FetchContent_MakeAvailable(catch)



# Collect all source files defined in this directory
file(GLOB PYTHON_TEST_SOURCES "*.cpp")

# Create the Python module
pybind11_add_module(lazymatrix_python_module MODULE ${PYTHON_TEST_SOURCES})

# Create the executable for testing the Python bindings
add_executable(lazymatrix_python_tests test_python_bindings.cpp)

# Include directories for the Python tests
target_include_directories(lazymatrix_python_module PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_BINARY_DIR}/_deps/mio-src
    ${CMAKE_BINARY_DIR}/_deps/eigen-src
    ${CMAKE_BINARY_DIR}/_deps/xoshiro-src
    ${CMAKE_BINARY_DIR}/_deps/dlib-src
    ${pybind11_INCLUDE_DIR}
    ${Python3_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/_deps/catch-src/src
)

target_include_directories(lazymatrix_python_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_BINARY_DIR}/_deps/mio-src
    ${CMAKE_BINARY_DIR}/_deps/eigen-src
    ${CMAKE_BINARY_DIR}/_deps/xoshiro-src
    ${CMAKE_BINARY_DIR}/_deps/dlib-src
    ${pybind11_INCLUDE_DIR}
    ${Python3_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/_deps/catch-src/src
)

# Link the dependencies required by the Python module
target_link_libraries(lazymatrix_python_module PRIVATE 
    dlib::dlib 
    Eigen3::Eigen
    Python3::Python Python3::Module
    Catch2::Catch2WithMain
)

# Link the dependencies required by the executable
target_link_libraries(lazymatrix_python_tests PRIVATE 
    dlib::dlib 
    Eigen3::Eigen
    Python3::Python Python3::Module
    Catch2::Catch2WithMain
)

# Set the output directory for binaries
set_target_properties(lazymatrix_python_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
set_target_properties(lazymatrix_python_module PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
