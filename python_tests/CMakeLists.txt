# This CMakeLists.txt is for the [tests/python_tests] directory of the LazyMatrix project.
# It sets up the build configuration for the test suite, including specifying test files,
# linking required libraries, and defining any necessary compiler options or flags.



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

# Create the test executable
add_executable(lazymatrix_python_tests ${PYTHON_TEST_SOURCES})

# Link the LazyMatrix library, Catch2, and Pybind11 libraries
target_link_libraries(lazymatrix_python_tests PRIVATE ${PROJECT_NAME} Catch2::Catch2WithMain pybind11::embed)

# Add the include directories
target_include_directories(lazymatrix_python_tests PUBLIC
    ${CMAKE_BINARY_DIR}/_deps/catch-src/src
)
