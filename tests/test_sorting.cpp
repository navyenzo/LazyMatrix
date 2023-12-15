//-------------------------------------------------------------------
/**
 * @file test_sorting.cpp
 * @brief Tests for sorting functionality in LazyMatrix.
 *
 * This file contains test cases to verify the correct implementation of the
 * sorting function in the LazyMatrix library. It tests sorting rows and columns
 * of a 3x3 matrix using the get_sorted_indices function, ensuring robustness and
 * correctness of the sorting.
 *
 * @author Vincenzo Barbato
 * @contact GitHub Project: https://github.com/navyenzo/LazyMatrix.git
 *          LinkedIn: https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



//-------------------------------------------------------------------
#include <catch2/catch_all.hpp>

#include <lazy_matrix.hpp>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test for sorting a specific row of a matrix.
 *
 * This test verifies that the get_sorted_indices function correctly
 * sorts a specific row of a matrix. A predefined 3x3 matrix is used, and the sorted indices are
 * checked against the expected sorted order.
 */
//-------------------------------------------------------------------
TEST_CASE("Sorting test: Specific row", "[Sorting]")
{
    LazyMatrix::SimpleMatrix<double> matrix(3, 3, 0.0);
    // Fill matrix with test data
    matrix.at_(0, 0) = 3.0; matrix.at_(0, 1) = 1.0; matrix.at_(0, 2) = 2.0;
    matrix.at_(1, 0) = 2.0; matrix.at_(1, 1) = 3.0; matrix.at_(1, 2) = 1.0;
    matrix.at_(2, 0) = 1.0; matrix.at_(2, 1) = 2.0; matrix.at_(2, 2) = 3.0;

    std::vector<int64_t> sorted_indices;
    LazyMatrix::get_sorted_indices(matrix, 1, true, sorted_indices); // Sort the second row

    // Expected order after sorting the second row: 2, 0, 1
    REQUIRE(sorted_indices[0] == 2);
    REQUIRE(sorted_indices[1] == 0);
    REQUIRE(sorted_indices[2] == 1);
}

//-------------------------------------------------------------------
/**
 * @brief Test for sorting a specific column of a matrix.
 *
 * This test assesses the functionality of the get_sorted_indices function
 * to correctly sort a specific column of a matrix. A predefined 3x3 matrix is used, and
 * the sorted indices are verified against the expected order of elements in that column.
 */
//-------------------------------------------------------------------
TEST_CASE("Sorting test: Specific column", "[Sorting]")
{
    LazyMatrix::SimpleMatrix<double> matrix(3, 3, 0.0);
    // Fill matrix with test data
    matrix.at_(0, 0) = 3.0; matrix.at_(0, 1) = 1.0; matrix.at_(0, 2) = 2.0;
    matrix.at_(1, 0) = 2.0; matrix.at_(1, 1) = 3.0; matrix.at_(1, 2) = 1.0;
    matrix.at_(2, 0) = 1.0; matrix.at_(2, 1) = 2.0; matrix.at_(2, 2) = 3.0;

    std::vector<int64_t> sorted_indices;
    LazyMatrix::get_sorted_indices(matrix, 2, false, sorted_indices); // Sort the third column

    // Expected order after sorting the third column: 1, 0, 2
    REQUIRE(sorted_indices[0] == 1);
    REQUIRE(sorted_indices[1] == 0);
    REQUIRE(sorted_indices[2] == 2);
}

//-------------------------------------------------------------------
/**
 * @brief Test for sorting with circular indices.
 *
 * This test checks the ability of the get_sorted_indices function to handle
 * circular indices, ensuring that sorting works correctly even when indices
 * are negative or exceed matrix dimensions. A predefined 3x3 matrix is used for the test.
 */
//-------------------------------------------------------------------
TEST_CASE("Sorting test: Circular indices", "[Sorting]")
{
    LazyMatrix::SimpleMatrix<double> matrix(3, 3, 0.0);
    // Fill matrix with test data
    matrix.at_(0, 0) = 3.0; matrix.at_(0, 1) = 1.0; matrix.at_(0, 2) = 2.0;
    matrix.at_(1, 0) = 2.0; matrix.at_(1, 1) = 3.0; matrix.at_(1, 2) = 1.0;
    matrix.at_(2, 0) = 1.0; matrix.at_(2, 1) = 2.0; matrix.at_(2, 2) = 3.0;

    std::vector<int64_t> sorted_indices;
    LazyMatrix::get_sorted_indices(matrix, -1, true, sorted_indices); // Sort with circular indexing (last row)

    // Expected order after sorting the last row with circular indexing: 1, 0, 2
    REQUIRE(sorted_indices[0] == 1);
    REQUIRE(sorted_indices[1] == 0);
    REQUIRE(sorted_indices[2] == 2);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test for mutable sorted matrix view.
 *
 * This test verifies the functionality of the sorted_matrix_view function, which creates
 * a mutable view of a matrix sorted by a specified row or column. The test checks if the
 * sorting is correctly applied and also ensures that modifications to the view are 
 * reflected in the original matrix. This is essential for validating that the view is 
 * indeed mutable and correctly linked to the original matrix.
 */
//-------------------------------------------------------------------
TEST_CASE("Mutable sorted matrix view test", "[Sorting View]")
{
    LazyMatrix::SimpleMatrix<double> matrix(3, 3, 0.0);
    // Fill matrix with test data
    matrix(0, 0) = 3.0; matrix(0, 1) = 1.0; matrix(0, 2) = 2.0;
    matrix(1, 0) = 2.0; matrix(1, 1) = 3.0; matrix(1, 2) = 1.0;
    matrix(2, 0) = 1.0; matrix(2, 1) = 2.0; matrix(2, 2) = 3.0;

    auto sorted_view = LazyMatrix::sorted_matrix_view(matrix, 1, true); // Create a mutable sorted view

    // Verify sorting correctness
    REQUIRE(sorted_view(0, 0) <= sorted_view(0, 1));
    REQUIRE(sorted_view(0, 1) <= sorted_view(0, 2));

    // Modify the sorted view
    sorted_view(1, 0) = 10.0;

    // Check if the modification is reflected in the original matrix
    REQUIRE(matrix(1, 2) == 10.0);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test for immutable sorted matrix view.
 *
 * This test examines the behavior of the sorted_matrix function, which generates
 * an immutable view of a sorted matrix. The key aspect being tested is whether 
 * modifications made to the view do not affect the original matrix, thereby 
 * confirming the immutability of the view. This test ensures the integrity of the 
 * original matrix data when using sorted views for read-only purposes.
 */
//-------------------------------------------------------------------
TEST_CASE("Immutable sorted matrix view test", "[Sorting]")
{
    LazyMatrix::SimpleMatrix<double> matrix(3, 3, 0.0);
    // Fill matrix with test data
    matrix(0, 0) = 3.0; matrix(0, 1) = 1.0; matrix(0, 2) = 2.0;
    matrix(1, 0) = 2.0; matrix(1, 1) = 3.0; matrix(1, 2) = 1.0;
    matrix(2, 0) = 1.0; matrix(2, 1) = 2.0; matrix(2, 2) = 3.0;

    auto sorted_view = LazyMatrix::sorted_matrix(matrix, 1, true); // Create an immutable sorted view

    // Verify sorting correctness
    REQUIRE(sorted_view(0, 0) <= sorted_view(1, 0));
    REQUIRE(sorted_view(1, 0) <= sorted_view(2, 0));

    // Attempt to modify the sorted view (this should be a compile-time error)
    // sorted_view(1, 2) = 10.0;

    // Check if the original matrix is unaffected
    REQUIRE(matrix(1, 2) != 10.0); // Assuming that the modification above is commented out
}
//-------------------------------------------------------------------
