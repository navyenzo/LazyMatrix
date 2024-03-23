//-------------------------------------------------------------------
/**
 * @file test_simple_data_wrappers.cpp
 * @brief Test wrappring individual single data value in 2d or 3d matrix like interface.
 * 
 * This file contains various test cases for when a single value is wrapped
 * in a 2d-matrix-like or 3d-matrix-like interface.
 * 
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



//-------------------------------------------------------------------
#include <catch2/catch_all.hpp>
#include "lazy_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test case for wrapping a value in a 2d-matrix-like interface
 */
//-------------------------------------------------------------------
TEST_CASE("Wrap single value in 2d-matrix-like interface", "[SimpleData2DMatrixWrapper]")
{
    // Sample value
    double value = 5.5;

    // Wrap the value with 2d-matrix-like interface
    auto matrix = LazyMatrix::wrap_scalar_in_matrix(value);

    // Test that it worked
    REQUIRE(matrix.rows() == 1);
    REQUIRE(matrix.columns() == 1);
    REQUIRE(matrix(0,0) == value);

    // Test that no matter the indeces, the
    // returned value is always the same
    REQUIRE(matrix(1000,1000) == value);
    
    // Let's test changing the value
    matrix(0,0) = -1.0;

    // Let's test that the changes occurred
    // but that the original value is intact
    REQUIRE(value == 5.5);
    REQUIRE(matrix(0,0) == -1.0);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test case for wrapping a value in a 3d-matrix-like interface
 */
//-------------------------------------------------------------------
TEST_CASE("Wrap single value in 3d-matrix-like interface", "[SimpleData3DMatrixWrapper]")
{
    // Sample value
    double value = 5.5;

    // Wrap the value with 2d-matrix-like interface
    auto matrix = LazyMatrix::wrap_scalar_in_matrix3d(value);

    // Test that it worked
    REQUIRE(matrix.pages() == 1);
    REQUIRE(matrix.rows() == 1);
    REQUIRE(matrix.columns() == 1);
    REQUIRE(matrix(0,0,0) == value);

    // Test that no matter the indeces, the
    // returned value is always the same
    REQUIRE(matrix(1000,1000,1000) == value);
    
    // Let's test changing the value
    matrix(0,0,0) = -1.0;

    // Let's test that the changes occurred
    // but that the original value is intact
    REQUIRE(value == 5.5);
    REQUIRE(matrix(0,0,0) == -1.0);
}
//-------------------------------------------------------------------