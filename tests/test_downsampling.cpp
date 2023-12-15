//-------------------------------------------------------------------
/**
 * @file test_downsampling.cpp
 * @brief Tests for downsampling functionality in LazyMatrix.
 *
 * This file contains test cases to verify the correct implementation of downsampling
 * methods in the LazyMatrix library. It includes tests for various downsampling scenarios,
 * such as forward, reverse, and circular downsampling, ensuring robustness and correctness
 * of the functionality.
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
 * @brief Test for forward downsampling of a source matrix.
 *
 * This test verifies that the simple_downsampling function correctly downsamples
 * a source matrix in a forward direction. It uses the LazyMatrix::Iota generator
 * to create a matrix with sequential values and checks if the downsampling
 * result matches the expected pattern.
 */
//-------------------------------------------------------------------
TEST_CASE("Downsampling test: Downsampling a source matrix in forward direction", "[MatrixDownsampling]")
{
    // Create a source matrix with sequential values using Iota
    auto source = LazyMatrix::Iota<int>(10, 1, 0, 1); // 10x1 matrix

    // Destination matrix for forward sampling
    LazyMatrix::Matrix<int> destination(5, 1, 0); // 5x1 matrix

    // Perform forward sampling
    LazyMatrix::simple_downsampling(source, destination, 0, 10, false);

    // Verify sampled data
    for (int i = 0; i < destination.rows(); ++i)
    {
        REQUIRE(destination.circ_at(i, 0) == source.circ_at(i * 2, 0));
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test for reverse downsampling of a source matrix.
 *
 * This test assesses the ability of the simple_downsampling function to correctly
 * perform reverse downsampling. It involves sampling from a higher index to a lower
 * index in the source matrix and verifying that the resulting data in the destination
 * matrix aligns with the expected reverse order.
 */
//-------------------------------------------------------------------
TEST_CASE("Downsampling test: Downsampling a source matrix in reverse direction", "[MatrixDownsampling]")
{
    // Create a source matrix with sequential values using Iota
    auto source = LazyMatrix::Iota<int>(10, 1, 0, 1); // 10x1 matrix

    // Destination matrix for reverse sampling
    LazyMatrix::Matrix<int> destination(5, 1, 0); // 5x1 matrix

    // Perform reverse sampling
    LazyMatrix::simple_downsampling(source, destination, 9, 0, false);

    // Verify sampled data
    for (int i = 0; i < destination.rows(); ++i)
    {
        REQUIRE(destination.circ_at(i, 0) == source.circ_at(9 - i * 2, 0));
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test for circular downsampling of a source matrix.
 *
 * This test checks the circular downsampling capability of the simple_downsampling
 * function. It ensures that the function can wrap around the source matrix when
 * the indices go beyond its bounds, correctly filling the destination matrix
 * with the appropriate values.
 */
//-------------------------------------------------------------------
TEST_CASE("Downsampling test: Circular downsampling of a source matrix", "[MatrixDownsampling]")
{
    // Create a source matrix with sequential values using Iota
    auto source = LazyMatrix::Iota<int>(10, 1, 0, 1); // 10x1 matrix

    // Destination matrix for circular sampling
    LazyMatrix::Matrix<int> destination(5, 1, 0); // 5x1 matrix

    // Perform circular forward sampling
    LazyMatrix::simple_downsampling(source, destination, 8, 13, false);

    // Verify sampled data
    int expectedIndices[5] = {8, 9, 0, 1, 2};
    for (int i = 0; i < destination.rows(); ++i)
    {
        REQUIRE(destination.circ_at(i, 0) == source.circ_at(expectedIndices[i], 0));
    }
}
//-------------------------------------------------------------------