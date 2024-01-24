//-------------------------------------------------------------------
/**
 * @file strassen_matrix_multiplication.hpp
 * @brief Strassen's algorithm for efficient matrix multiplication.
 *
 * This file contains the implementation of Strassen's algorithm for matrix multiplication.
 * It includes various utility functions for matrix manipulation, such as padding, trimming,
 * splitting, and combining matrices, as well as the core Strassen multiplication logic.
 * This implementation optimizes matrix multiplication for large matrices by reducing the
 * number of recursive multiplications.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_STRASSEN_MATRIX_MULTIPLICATION_HPP_
#define INCLUDE_STRASSEN_MATRIX_MULTIPLICATION_HPP_



//-------------------------------------------------------------------
#include "base_matrix.hpp"
#include "simple_matrix.hpp"
#include "shared_references.hpp"
#include "padding_view.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Helper functions defined specifically for the Strassen algorithm
// to perform immediate calculations instead of creating lazy expressions.
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Trims a matrix to the specified size.
 *
 * @tparam MatrixType Type of the matrix.
 * @param matrix The matrix to be trimmed.
 * @param rows Number of rows in the trimmed matrix.
 * @param columns Number of columns in the trimmed matrix.
 * @return Trimmed matrix.
 */
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

trim_matrix(const MatrixType& matrix, int rows, int columns)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type>::type;

    auto trimmed = MatrixFactory::create_simple_matrix<value_type>(rows, columns);
    
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            trimmed(i, j) = matrix(i, j);
        }
    }

    return trimmed;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Splits a matrix into four equal submatrices.
 *
 * This function divides a given matrix into four submatrices. The split
 * occurs at the midpoint of the rows and columns of the matrix. It's used
 * as a part of the Strassen algorithm for matrix multiplication.
 *
 * @tparam MatrixType1 Type of the source matrix.
 * @tparam MatrixType2 Type of the submatrices.
 * @param matrix The matrix to be split.
 * @param a11 Upper left submatrix.
 * @param a12 Upper right submatrix.
 * @param a21 Lower left submatrix.
 * @param a22 Lower right submatrix.
 */
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         typename MatrixType3,
         typename MatrixType4,
         typename MatrixType5,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType3>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType4>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType5>{}>* = nullptr>

inline void

strassen_split(const MatrixType1& matrix, MatrixType2& a11, MatrixType3& a12, MatrixType4& a21, MatrixType5& a22)
{
    int mid_row = matrix.rows() / 2;
    int mid_col = matrix.columns() / 2;

    for (int i = 0; i < matrix.rows(); ++i)
    {
        for (int j = 0; j < matrix.columns(); ++j)
        {
            if (i < mid_row && j < mid_col)
            {
                a11(i, j) = matrix(i, j);
            }
            else if (i < mid_row && j >= mid_col)
            {
                a12(i, j - mid_col) = matrix(i, j);
            }
            else if (i >= mid_row && j < mid_col)
            {
                a21(i - mid_row, j) = matrix(i, j);
            }
            else // i >= mid_row && j >= mid_col
            {
                a22(i - mid_row, j - mid_col) = matrix(i, j);
            }
        }
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Combines four submatrices into a single matrix.
 *
 * @tparam MatrixType Type of the matrix.
 * @param a11 Upper left submatrix.
 * @param a12 Upper right submatrix.
 * @param a21 Lower left submatrix.
 * @param a22 Lower right submatrix.
 * @return Combined matrix.
 */
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         typename MatrixType3,
         typename MatrixType4,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType3>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType4>{}>* = nullptr>

inline auto

strassen_combine(const MatrixType1& a11, const MatrixType2& a12, const MatrixType3& a21, const MatrixType4& a22)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<MatrixType1>()(0,0))>::type>::type;

    auto result = SimpleMatrix<value_type>(a11.rows() * 2, a11.columns() * 2);
    int mid_row = result.rows() / 2;
    int mid_col = result.columns() / 2;

    for (int i = 0; i < result.rows(); ++i)
    {
        for (int j = 0; j < result.columns(); ++j)
        {
            if (i < mid_row && j < mid_col)
            {
                result(i, j) = a11(i, j);
            }
            else if (i < mid_row && j >= mid_col)
            {
                result(i, j) = a12(i, j - mid_col);
            }
            else if (i >= mid_row && j < mid_col)
            {
                result(i, j) = a21(i - mid_row, j);
            }
            else
            {
                result(i, j) = a22(i - mid_row, j - mid_col);
            }
        }
    }

    return result;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Adds two matrices.
 *
 * @tparam MatrixType Type of the matrices.
 * @param a First matrix.
 * @param b Second matrix.
 * @return Sum of the two matrices.
 */
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>

inline auto

strassen_add(const MatrixType1& a, const MatrixType2& b)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<MatrixType1>()(0,0))>::type>::type;

    auto result = SimpleMatrix<value_type>(a.rows(), a.columns());

    for (int i = 0; i < a.rows(); ++i)
    {
        for (int j = 0; j < a.columns(); ++j)
        {
            result(i, j) = a(i, j) + b(i, j);
        }
    }

    return result;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Subtracts one matrix from another.
 *
 * @tparam MatrixType Type of the matrices.
 * @param a First matrix.
 * @param b Second matrix to subtract from the first.
 * @return Result of the subtraction.
 */
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>

inline auto

strassen_subtract(const MatrixType1& a, const MatrixType2& b)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<MatrixType1>()(0,0))>::type>::type;

    auto result = SimpleMatrix<value_type>(a.rows(), a.columns());

    for (int i = 0; i < a.rows(); ++i)
    {
        for (int j = 0; j < a.columns(); ++j)
        {
            result(i, j) = a(i, j) - b(i, j);
        }
    }

    return result;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Recursively multiplies two matrices using the Strassen algorithm.
 *
 * Implements the Strassen algorithm to efficiently multiply two matrices.
 * The algorithm recursively divides the matrices into smaller submatrices,
 * computes seven intermediate products, and combines these products to
 * form the final result. This implementation optimizes for generic matrix
 * expressions, using a concrete matrix type for internal computations.
 *
 * @tparam MatrixType Type of the matrices.
 * @param a First matrix operand.
 * @param b Second matrix operand.
 * @return The result of multiplying matrices a and b.
 *
 * @note For matrices smaller than or equal to 2x2, the function falls
 *       back to conventional matrix multiplication.
 */
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>

inline auto

strassen_multiply_recursive(const MatrixType1& a, const MatrixType2& b)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<MatrixType1>()(0,0))>::type>::type;

    // Base case for recursion
    if (a.rows() <= 2 || a.columns() <= 2 || b.rows() <= 2 || b.columns() <= 2)
    {
        auto result = SimpleMatrix<value_type>(a.rows(), b.columns());

        for (int i = 0; i < result.rows(); ++i)
        {
            for (int j = 0; j < result.columns(); ++j)
            {
                for (int k = 0; k < a.columns(); ++k)
                {
                    result(i, j) += a(i, k) * b(k, j);
                }
            }
        }

        return result;
    }

    // Splitting matrices into submatrices
    int mid_row = a.rows() / 2;
    int mid_col = a.columns() / 2;

    auto a11 = SimpleMatrix<value_type>(mid_row, mid_col);
    auto a12 = SimpleMatrix<value_type>(mid_row, mid_col);
    auto a21 = SimpleMatrix<value_type>(mid_row, mid_col);
    auto a22 = SimpleMatrix<value_type>(mid_row, mid_col);

    auto b11 = SimpleMatrix<value_type>(mid_row, mid_col);
    auto b12 = SimpleMatrix<value_type>(mid_row, mid_col);
    auto b21 = SimpleMatrix<value_type>(mid_row, mid_col);
    auto b22 = SimpleMatrix<value_type>(mid_row, mid_col);

    strassen_split(a, a11, a12, a21, a22);
    strassen_split(b, b11, b12, b21, b22);

    // Compute the 7 products using the Strassen algorithm
    auto p1 = strassen_multiply_recursive(a11, strassen_subtract(b12, b22));
    auto p2 = strassen_multiply_recursive(strassen_add(a11, a12), b22);
    auto p3 = strassen_multiply_recursive(strassen_add(a21, a22), b11);
    auto p4 = strassen_multiply_recursive(a22, strassen_subtract(b21, b11));
    auto p5 = strassen_multiply_recursive(strassen_add(a11, a22), strassen_add(b11, b22));
    auto p6 = strassen_multiply_recursive(strassen_subtract(a12, a22), strassen_add(b21, b22));
    auto p7 = strassen_multiply_recursive(strassen_subtract(a11, a21), strassen_add(b11, b12));

    // Combine the products to form the final result
    auto c11 = strassen_add(strassen_subtract(strassen_add(p5, p4), p2), p6);
    auto c12 = strassen_add(p1, p2);
    auto c21 = strassen_add(p3, p4);
    auto c22 = strassen_subtract(strassen_subtract(strassen_add(p1, p5), p3), p7);

    return strassen_combine(c11, c12, c21, c22);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Performs matrix multiplication using the Strassen algorithm.
 *
 * This function implements the Strassen algorithm for efficient matrix multiplication.
 * It first pads the input matrices to dimensions that are powers of two, 
 * which optimizes the recursive division process in the Strassen algorithm.
 * After the multiplication, it trims the result back to the original size.
 * This approach is particularly efficient for large matrices where the
 * conventional multiplication becomes computationally expensive.
 *
 * @tparam MatrixType1 Type of the first matrix operand.
 * @tparam MatrixType2 Type of the second matrix operand.
 * @param a The first matrix operand, not modified by padding.
 * @param b The second matrix operand, not modified by padding.
 * @return Matrix containing the result of the multiplication.
 * 
 * @note For matrices smaller or equal to 2x2, this function falls back to
 *       conventional matrix multiplication. Padding is applied internally
 *       and does not modify the input matrices.
 */
//-------------------------------------------------------------------
template<typename ReferenceType1,
         typename ReferenceType2,
         std::enable_if_t<is_matrix_reference<ReferenceType1>{}>* = nullptr,
         std::enable_if_t<is_matrix_reference<ReferenceType2>{}>* = nullptr>

inline auto

strassen_matrix_multiply(ReferenceType1 a, ReferenceType2 b)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<ReferenceType1>()(0,0))>::type>::type;

    // Calculate the size for padding
    int max_dim = std::max({a.rows(), a.columns(), b.rows(), b.columns()});
    int new_size = std::pow(2, std::ceil(std::log2(max_dim)));

    // Apply padding
    auto padded_a = create_padded_matrix_view(a, new_size, new_size, static_cast<value_type>(0));
    auto padded_b = create_padded_matrix_view(b, new_size, new_size, static_cast<value_type>(0));

    // Perform Strassen multiplication on the padded matrices
    auto result = strassen_multiply_recursive(*padded_a, *padded_b);

    // Trim the result back to the size of the original matrix
    return trim_matrix(result, a.rows(), b.columns());
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif // INCLUDE_STRASSEN_MATRIX_MULTIPLICATION_HPP_
