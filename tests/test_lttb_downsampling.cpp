//-------------------------------------------------------------------
/**
 * @file test_lttb_downsampling.cpp
 * @brief Tests for triangle bucket downsampling functionality in LazyMatrix.
 *
 * This file contains test cases to verify the correct implementation of the
 * triangle bucket downsampling method in the LazyMatrix library. It includes tests for
 * various downsampling scenarios, such as forward, reverse, and circular downsampling,
 * ensuring robustness and correctness of the functionality.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------

#include <catch2/catch_all.hpp>
#include "lazy_matrix.hpp"

//-------------------------------------------------------------------
/**
 * @brief Test for forward triangle bucket downsampling of a source matrix.
 *
 * This test verifies that the triangle_bucket_downsampling function correctly
 * downsamples a source matrix in a forward direction. It uses the LazyMatrix::Iota generator
 * to create a matrix with sequential values and checks if the downsampling
 * result is as expected with averaged values.
 */
//-------------------------------------------------------------------
TEST_CASE("LTTB Downsampling test: Forward direction", "[Downsampling]")
{
    auto source = LazyMatrix::Iota<double>(10, 1.0); // 10-element vector
    LazyMatrix::SimpleMatrix<double> destination(5, 1, 0.0); // 5-element vector for downsampled data

    downsample_lttb(source, destination, 0, 9);

    double expected_values[5] = {1.0, 3.0, 5.0, 7.0, 9.0};
    for (int i = 0; i < 5; ++i)
    {
        REQUIRE(destination(i) == Catch::Approx(expected_values[i]));
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test for reverse triangle bucket downsampling of a source matrix.
 *
 * This test assesses the ability of the triangle_bucket_downsampling function to correctly
 * perform reverse downsampling. It involves sampling from a higher index to a lower
 * index in the source matrix and verifying that the resulting data in the destination
 * matrix is averaged correctly.
 */
//-------------------------------------------------------------------
TEST_CASE("LTTB Downsampling test: Reverse direction", "[Downsampling]")
{
    auto source = LazyMatrix::Iota<double>(10, 1.0); // 10-element vector
    LazyMatrix::SimpleMatrix<double> destination(5, 1, 0.0); // 5-element vector for downsampled data

    downsample_lttb(source, destination, 9, 0);

    double expected_values[5] = {9.0, 7.0, 5.0, 3.0, 1.0};
    for (int i = 0; i < 5; ++i)
    {
        REQUIRE(destination(i) == Catch::Approx(expected_values[i]));
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test for circular triangle bucket downsampling of a source matrix.
 *
 * This test checks the circular downsampling capability of the triangle_bucket_downsampling
 * function. It ensures that the function can wrap around the source matrix when
 * the indices go beyond its bounds, correctly filling the destination matrix
 * with the averaged values.
 */
//-------------------------------------------------------------------
TEST_CASE("LTTB Downsampling test: Circular sampling", "[Downsampling]")
{
    auto source = LazyMatrix::Iota<double>(10, 1.0); // 10-element vector
    LazyMatrix::SimpleMatrix<double> destination(5, 1, 0.0); // 5-element vector for downsampled data

    downsample_lttb(source, destination, 8, -2);

    double expected_values[5] = {9.0, 2.0, 4.0, 6.0, 8.0};
    for (int i = 0; i < 5; ++i)
    {
        REQUIRE(destination(i) == Catch::Approx(expected_values[i]));
    }
}
//-------------------------------------------------------------------
