#ifndef INCLUDE_STRASSEN_MATRIX_MULTIPLICATION_HPP_
#define INCLUDE_STRASSEN_MATRIX_MULTIPLICATION_HPP_



//-------------------------------------------------------------------
#include "base_matrix.hpp"
#include "matrix.hpp"
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
 * @brief Pads a matrix to the next power of two dimensions.
 *
 * @tparam MatrixType Type of the matrix.
 * @param matrix The matrix to be padded.
 * @return Padded matrix.
 */
//-------------------------------------------------------------------
template<typename MatrixType>
MatrixType pad_matrix(const MatrixType& matrix)
{
    int max_dim = std::max(matrix.rows(), matrix.columns());
    int new_size = std::pow(2, std::ceil(std::log2(max_dim)));
    MatrixType padded(new_size, new_size);
    // Copy original matrix into the top-left corner
    for (int i = 0; i < matrix.rows(); ++i)
    {
        for (int j = 0; j < matrix.columns(); ++j)
        {
            padded(i, j) = matrix(i, j);
        }
    }
    return padded;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Trims a matrix to the specified size.
 *
 * @tparam MatrixType Type of the matrix.
 * @param matrix The matrix to be trimmed.
 * @param rows Number of rows in the trimmed matrix.
 * @param cols Number of columns in the trimmed matrix.
 * @return Trimmed matrix.
 */
//-------------------------------------------------------------------
template<typename MatrixType>
MatrixType trim_matrix(const MatrixType& matrix, int rows, int cols)
{
    MatrixType trimmed(rows, cols);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
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
 * @tparam MatrixType Type of the matrix.
 * @param matrix The matrix to be split.
 * @param a11 Upper left submatrix.
 * @param a12 Upper right submatrix.
 * @param a21 Lower left submatrix.
 * @param a22 Lower right submatrix.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

void strassen_split(const MatrixType& matrix, MatrixType& a11, MatrixType& a12, MatrixType& a21, MatrixType& a22)
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
            else
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
template<typename MatrixType>

MatrixType strassen_combine(const MatrixType& a11, const MatrixType& a12, const MatrixType& a21, const MatrixType& a22)
{
    MatrixType result(a11.rows() * 2, a11.columns() * 2);
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
template<typename MatrixType>

MatrixType strassen_add(const MatrixType& a, const MatrixType& b)
{
    MatrixType result(a.rows(), a.columns());

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
template<typename MatrixType>

MatrixType strassen_subtract(const MatrixType& a, const MatrixType& b)
{
    MatrixType result(a.rows(), a.columns());

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
 * @tparam MatrixType Type of the matrices.
 * @param a First matrix.
 * @param b Second matrix.
 * @return The result of multiplying matrices a and b.
 */
//-------------------------------------------------------------------
template<typename MatrixType>
MatrixType strassen_multiply_recursive(const MatrixType& a, const MatrixType& b)
{
    // Base case for recursion
    if (a.rows() <= 2 || a.columns() <= 2 || b.rows() <= 2 || b.columns() <= 2)
    {
        MatrixType result(a.rows(), b.columns());
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
    
    MatrixType a11(mid_row, mid_col), a12(mid_row, mid_col), 
               a21(mid_row, mid_col), a22(mid_row, mid_col);
    MatrixType b11(mid_row, mid_col), b12(mid_row, mid_col), 
               b21(mid_row, mid_col), b22(mid_row, mid_col);

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
 * It pads matrices to the next power of two, performs the Strassen multiplication, and then
 * trims the result back to the original size.
 *
 * @tparam MatrixType1 Type of the first matrix operand.
 * @tparam MatrixType2 Type of the second matrix operand.
 * @param a The first matrix operand.
 * @param b The second matrix operand.
 * @return Matrix containing the result of the multiplication.
 *
 * @note For matrices smaller or equal to 2x2, this function falls back to
 *       conventional matrix multiplication.
 */
//-------------------------------------------------------------------
template<typename MatrixType1, typename MatrixType2,
            std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
            std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>
auto strassen_matrix_multiply(const MatrixType1& a, const MatrixType2& b)
{
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType1>()(0,0))>::type;
    using result_type = Matrix<value_type>;

    // Pad matrices to the next power of two
    auto padded_a = pad_matrix(a);
    auto padded_b = pad_matrix(b);

    // Perform Strassen multiplication on the padded matrices
    result_type result = strassen_multiply_recursive(padded_a, padded_b);

    // Trim the result to the original size
    return trim_matrix(result, a.rows(), b.columns());
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif // INCLUDE_STRASSEN_MATRIX_MULTIPLICATION_HPP_
