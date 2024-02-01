//-------------------------------------------------------------------
/**
 * @file test_2d_matrix_arithmetic_operations.cpp
 * @brief Tests for 2D matrix arithmetic operations in the LazyMatrix library.
 *
 * This file contains tests for various 2D matrix arithmetic operations
 * including addition, subtraction, multiplication, etc., to validate
 * the correctness and efficiency of these operations.
 * 
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#define CATCH_CONFIG_MAIN // This define is needed once only when using Catch2
#define CATCH_CONFIG_ENABLE_BENCHMARKING



//-------------------------------------------------------------------
#include <catch2/catch_all.hpp>
#include <chrono>
#include "lazy_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
TEST_CASE("2D Matrix Addition", "[Matrix2D]")
{
    // Create matrices
    auto mat1 = LazyMatrix::MatrixFactory::create_simple_matrix<int>(2,2,0);
    auto mat2 = LazyMatrix::MatrixFactory::create_simple_matrix<int>(2,2,0);

    // Populate matrices
    mat1(0,0) = 1; mat1(0,1) = 2;
    mat1(1,0) = 3; mat1(1,1) = 4;

    mat2(0,0) = 5; mat2(0,1) = 6;
    mat2(1,0) = 7; mat2(1,1) = 8;

    // Perform addition
    auto result = mat1 + mat2;

    // Check dimensions
    REQUIRE(result.rows() == 2);
    REQUIRE(result.columns() == 2);

    // Check each element
    REQUIRE(result(0, 0) == 6); // 1 + 5
    REQUIRE(result(0, 1) == 8); // 2 + 6
    REQUIRE(result(1, 0) == 10); // 3 + 7
    REQUIRE(result(1, 1) == 12); // 4 + 8
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
TEST_CASE("2D Matrix Subtraction", "[Matrix2D]")
{
    // Create matrices
    auto mat1 = LazyMatrix::MatrixFactory::create_simple_matrix<int>(2,2,0);
    auto mat2 = LazyMatrix::MatrixFactory::create_simple_matrix<int>(2,2,0);

    // Populate matrices
    mat1(0,0) = 1; mat1(0,1) = 2;
    mat1(1,0) = 3; mat1(1,1) = 4;

    mat2(0,0) = 5; mat2(0,1) = 6;
    mat2(1,0) = 7; mat2(1,1) = 8;

    // Perform addition
    auto result = mat1 - mat2;

    // Check dimensions
    REQUIRE(result.rows() == 2);
    REQUIRE(result.columns() == 2);

    // Check each element
    REQUIRE(result(0, 0) == -4); // 1 - 5
    REQUIRE(result(0, 1) == -4); // 2 - 6
    REQUIRE(result(1, 0) == -4); // 3 - 7
    REQUIRE(result(1, 1) == -4); // 4 - 8
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
TEST_CASE("2D Matrix Multiplication", "[Matrix2D]")
{
    // Create matrices
    auto mat1 = LazyMatrix::MatrixFactory::create_simple_matrix<int>(2,2,0);
    auto mat2 = LazyMatrix::MatrixFactory::create_simple_matrix<int>(2,2,0);

    // Populate matrices
    mat1(0,0) = 1; mat1(0,1) = 2;
    mat1(1,0) = 3; mat1(1,1) = 4;

    mat2(0,0) = 5; mat2(0,1) = 6;
    mat2(1,0) = 7; mat2(1,1) = 8;

    // Perform addition
    auto result = mat1 * mat2;

    // Check dimensions
    REQUIRE(result.rows() == 2);
    REQUIRE(result.columns() == 2);

    // Check each element
    REQUIRE(result(0, 0) == 19);
    REQUIRE(result(0, 1) == 22);
    REQUIRE(result(1, 0) == 43);
    REQUIRE(result(1, 1) == 50);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
TEST_CASE("Strassen vs Naive Matrix Multiplication", "[Matrix2D]")
{
    int64_t dimension_length = 16;

    // Create two random 10x10 matrices
    auto random_mat1 = LazyMatrix::generate_random_matrix<int>(dimension_length, dimension_length, -10, 10);
    auto random_mat2 = LazyMatrix::generate_random_matrix<int>(dimension_length, dimension_length, -10, 10);

    // Convert random generators to actual matrices for multiplication
    auto mat1 = LazyMatrix::MatrixFactory::create_simple_matrix<int>(random_mat1);
    auto mat2 = LazyMatrix::MatrixFactory::create_simple_matrix<int>(random_mat2);

     using clock = std::chrono::high_resolution_clock;

    // Time naive multiplication
    auto start_naive = clock::now();
    auto result_naive = mat1 * mat2;
    auto end_naive = clock::now();
    std::chrono::duration<double> elapsed_naive = end_naive - start_naive;
    std::cout << "Time taken for naive multiplication: " << elapsed_naive.count() << " seconds\n";

    // Time Strassen multiplication
    auto start_strassen = clock::now();
    auto result_strassen = LazyMatrix::strassen_matrix_multiply(mat1, mat2);
    auto end_strassen = clock::now();
    std::chrono::duration<double> elapsed_strassen = end_strassen - start_strassen;
    std::cout << "Time taken for Strassen multiplication: " << elapsed_strassen.count() << " seconds\n\n\n";

    // Check dimensions
    REQUIRE(result_naive.rows() == dimension_length);
    REQUIRE(result_naive.columns() == dimension_length);
    REQUIRE(result_strassen.rows() == dimension_length);
    REQUIRE(result_strassen.columns() == dimension_length);

    // Check each element
    for (int64_t i = 0; i < 10; ++i)
    {
        for (int64_t j = 0; j < 10; ++j)
        {
            REQUIRE(result_naive(i, j) == result_strassen(i, j));
        }
    }
}
//-------------------------------------------------------------------