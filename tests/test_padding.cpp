//-------------------------------------------------------------------
/**
 * @file test_padding.cpp
 * @brief Tests for matrix padding functionality in LazyMatrix.
 *
 * This file contains test cases to verify the correct implementation of padding
 *
 * @author Vincenzo Barbato
 * @contact GitHub Project: https://github.com/navyenzo/LazyMatrix.git
 *          LinkedIn: https://www.linkedin.com/in/vincenzobarbato/
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

    SECTION("PaddedMatrix Test")
    {
        // Apply padding
        auto paddedMatrix = LazyMatrix::pad_matrix(small_matrix, padded_rows, padded_columns, 0);

        // Check dimensions
        REQUIRE(paddedMatrix.rows() == padded_rows);
        REQUIRE(paddedMatrix.columns() == padded_columns);

        // Check original values
        REQUIRE(paddedMatrix(0, 0) == 1);
        REQUIRE(paddedMatrix(1, 1) == 4);

        // Check padding values
        REQUIRE(paddedMatrix(2, 2) == 0);
        REQUIRE(paddedMatrix(3, 3) == 0);
    }

    SECTION("PaddedMatrixView Test")
    {
        // Apply padding view
        auto paddedMatrixView = LazyMatrix::pad_matrix_view(small_matrix, padded_rows, padded_columns, 0);

        // Check dimensions
        REQUIRE(paddedMatrixView.rows() == padded_rows);
        REQUIRE(paddedMatrixView.columns() == padded_columns);

        // Check original values
        REQUIRE(paddedMatrixView(0, 0) == 1);
        REQUIRE(paddedMatrixView(1, 1) == 4);

        // Check padding values
        REQUIRE(paddedMatrixView(2, 2) == 0);
        REQUIRE(paddedMatrixView(3, 3) == 0);
    }
}
//-------------------------------------------------------------------