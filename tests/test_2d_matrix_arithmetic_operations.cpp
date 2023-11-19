#define CATCH_CONFIG_MAIN // This define is needed once only when using Catch2



#include <catch2/catch_all.hpp>
#include "lazy_matrix.hpp"



TEST_CASE("2D Matrix Addition", "[Matrix2D]")
{
    // Create matrices
    LazyMatrix::Matrix<int> mat1(2, 2);
    LazyMatrix::Matrix<int> mat2(2, 2);

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



TEST_CASE("2D Matrix Subtraction", "[Matrix2D]")
{
    // Create matrices
    LazyMatrix::Matrix<int> mat1(2, 2);
    LazyMatrix::Matrix<int> mat2(2, 2);

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



TEST_CASE("2D Matrix Multiplication", "[Matrix2D]")
{
    // Create matrices
    LazyMatrix::Matrix<int> mat1(2, 2);
    LazyMatrix::Matrix<int> mat2(2, 2);

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
