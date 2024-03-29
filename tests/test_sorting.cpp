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
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
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
    auto matrix = LazyMatrix::MatrixFactory::create_simple_matrix<double>(3, 3, 0.0);

    // Fill matrix with test data
    matrix(0, 0) = 3.0; matrix(0, 1) = 1.0; matrix(0, 2) = 2.0;
    matrix(1, 0) = 2.0; matrix(1, 1) = 3.0; matrix(1, 2) = 1.0;
    matrix(2, 0) = 1.0; matrix(2, 1) = 2.0; matrix(2, 2) = 3.0;

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
    auto matrix = LazyMatrix::MatrixFactory::create_simple_matrix<double>(3, 3, 0.0);

    // Fill matrix with test data
    matrix(0, 0) = 3.0; matrix(0, 1) = 1.0; matrix(0, 2) = 2.0;
    matrix(1, 0) = 2.0; matrix(1, 1) = 3.0; matrix(1, 2) = 1.0;
    matrix(2, 0) = 1.0; matrix(2, 1) = 2.0; matrix(2, 2) = 3.0;

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
    auto matrix = LazyMatrix::MatrixFactory::create_simple_matrix<double>(3, 3, 0.0);

    // Fill matrix with test data
    matrix(0, 0) = 3.0; matrix(0, 1) = 1.0; matrix(0, 2) = 2.0;
    matrix(1, 0) = 2.0; matrix(1, 1) = 3.0; matrix(1, 2) = 1.0;
    matrix(2, 0) = 1.0; matrix(2, 1) = 2.0; matrix(2, 2) = 3.0;

    std::vector<int64_t> sorted_indices;
    LazyMatrix::get_sorted_indices(matrix, -3, true, sorted_indices); // Sort with circular indexing (first row)

    // Expected order after sorting the last row with circular indexing: 2, 0, 1
    REQUIRE(sorted_indices[0] == 1);
    REQUIRE(sorted_indices[1] == 2);
    REQUIRE(sorted_indices[2] == 0);
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
    auto matrix = LazyMatrix::MatrixFactory::create_simple_matrix<double>(3, 3, 0.0);

    // Fill matrix with test data
    matrix(0, 0) = 3.0; matrix(0, 1) = 1.0; matrix(0, 2) = 2.0;
    matrix(1, 0) = 2.0; matrix(1, 1) = 3.0; matrix(1, 2) = 1.0;
    matrix(2, 0) = 1.0; matrix(2, 1) = 2.0; matrix(2, 2) = 3.0;

    int64_t index_of_row_to_sort = 1;

    auto sorted_view = LazyMatrix::create_sorted_matrix_view(matrix, index_of_row_to_sort, true); // Create a mutable sorted view

    // Verify sorting correctness
    REQUIRE(sorted_view(index_of_row_to_sort, 0) <= sorted_view(index_of_row_to_sort, 1));
    REQUIRE(sorted_view(index_of_row_to_sort, 1) <= sorted_view(index_of_row_to_sort, 2));

    // Modify the sorted view
    sorted_view(index_of_row_to_sort, 0) = 10.0;

    // Check if the modification is reflected in the original matrix
    REQUIRE(matrix(index_of_row_to_sort, 2) == 10.0);
}
//-------------------------------------------------------------------
