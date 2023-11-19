# Catch2 library used for testing
include(FetchContent)
FetchContent_Declare(
  catch
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG devel
)
FetchContent_MakeAvailable(catch)



# Collect all source files defined in this directory
file(GLOB TEST_SOURCES "*.cpp")

# Add the source files to the tests executable
add_executable(tests ${TEST_SOURCES})


# link the Catch2 library
target_link_libraries(tests PRIVATE ${PROJECT_NAME} Catch2::Catch2WithMain)



# Add the include directories
target_include_directories(tests PUBLIC
    ${CMAKE_BINARY_DIR}/_deps/catch-src/src
)