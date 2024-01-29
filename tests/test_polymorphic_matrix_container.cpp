//-------------------------------------------------------------------
/**
 * @file test_polymorphic_matrix_container.cpp
 * @brief Test file for the MatrixContainer class using Catch2.
 *
 * This file tests the functionality of the MatrixContainer class.
 * It creates simple matrices, adds them, and verifies the values in the container.
 * 
 * @author Vincenzo Barbato
 */
//-------------------------------------------------------------------



//-------------------------------------------------------------------
#include <catch2/catch_all.hpp>
#include <lazy_matrix.hpp>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
TEST_CASE("MatrixContainer Functionality", "[MatrixContainer]")
{
    // Create a couple of matrices using the factory function
    auto m1 = LazyMatrix::MatrixFactory::create_simple_matrix<int>(2, 2, 1); // 2x2 matrix with all elements as 1
    auto m2 = LazyMatrix::MatrixFactory::create_simple_matrix<int>(2, 2, 2); // 2x2 matrix with all elements as 2

    // Create other matrices by adding and subtracting m1 and m2
    auto m3 = m1 + m2;
    auto m4 = m1 - m2;

    auto m3_wrapped = LazyMatrix::wrap_matrix(m3);
    auto m4_wrapped = LazyMatrix::wrap_matrix(m4);

    // Create a MatrixContainer and add matrices
    LazyMatrix::MatrixContainer<int> container;
    container.add_matrix(m1);
    container.add_matrix(m2);
    container.add_matrix(m3);
    container.add_matrix(m4);

    // Test the dimensions
    REQUIRE(container.pages() == 4);
    REQUIRE(container.rows() == 2);
    REQUIRE(container.columns() == 2);

    // Test values in each matrix (page)
    // Page 0 (m1)
    REQUIRE(container(0, 0, 0) == 1);
    REQUIRE(container(0, 0, 1) == 1);
    REQUIRE(container(0, 1, 0) == 1);
    REQUIRE(container(0, 1, 1) == 1);

    // Page 1 (m2)
    REQUIRE(container(1, 0, 0) == 2);
    REQUIRE(container(1, 0, 1) == 2);
    REQUIRE(container(1, 1, 0) == 2);
    REQUIRE(container(1, 1, 1) == 2);

    // Page 2 (m3 = m1 + m2)
    REQUIRE(container(2, 0, 0) == 3);
    REQUIRE(container(2, 0, 1) == 3);
    REQUIRE(container(2, 1, 0) == 3);
    REQUIRE(container(2, 1, 1) == 3);

    // Page 3 (m3 = m1 - m2)
    REQUIRE(container(3, 0, 0) == -1);
    REQUIRE(container(3, 0, 1) == -1);
    REQUIRE(container(3, 1, 0) == -1);
    REQUIRE(container(3, 1, 1) == -1);
}
//-------------------------------------------------------------------