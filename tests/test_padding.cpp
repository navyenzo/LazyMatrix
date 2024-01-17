//-------------------------------------------------------------------
/**
 * @file test_padding.cpp
 * @brief Tests for matrix padding functionality in LazyMatrix.
 *
 * This file contains test cases to verify the correct implementation of padding
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
 * @brief Test the padding of a source matrix
 *
 * This test verifies that we can successfully pad a source matrix
 * with a user specified value. It tests both the case when the
 * source matrix is a constant or when it is a non-constant whose
 * values can be changed
 */
//-------------------------------------------------------------------
TEST_CASE("Matrix Padding Test", "[MatrixPadding]")
{
    // Create a small matrix for testing
    LazyMatrix::Matrix<int> small_matrix(2, 2);
    small_matrix(0, 0) = 1;
    small_matrix(0, 1) = 2;
    small_matrix(1, 0) = 3;
    small_matrix(1, 1) = 4;

    // Define padded size
    const uintptr_t padded_rows = 4;
    const uintptr_t padded_columns = 4;

    SECTION("padded_matrix Test")
    {
        // Apply padding
        auto padded_matrix = pad_matrix(small_matrix, padded_rows, padded_columns, 0);

        // Check dimensions
        REQUIRE(padded_matrix.rows() == padded_rows);
        REQUIRE(padded_matrix.columns() == padded_columns);

        // Check original values
        REQUIRE(padded_matrix(0, 0) == 1);
        REQUIRE(padded_matrix(1, 1) == 4);

        // Check padding values
        REQUIRE(padded_matrix(2, 2) == 0);
        REQUIRE(padded_matrix(3, 3) == 0);
    }

    SECTION("padded_matrix_view Test")
    {
        // Apply padding view
        auto padded_matrix_view = pad_matrix(small_matrix, padded_rows, padded_columns, 0);

        // Check dimensions
        REQUIRE(padded_matrix_view.rows() == padded_rows);
        REQUIRE(padded_matrix_view.columns() == padded_columns);

        // Check original values
        REQUIRE(padded_matrix_view(0, 0) == 1);
        REQUIRE(padded_matrix_view(1, 1) == 4);

        // Check padding values
        REQUIRE(padded_matrix_view(2, 2) == 0);
        REQUIRE(padded_matrix_view(3, 3) == 0);
    }
}
//-------------------------------------------------------------------