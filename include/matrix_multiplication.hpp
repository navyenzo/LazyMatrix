#ifndef INCLUDE_MATRIX_MULTIPLICATION_HPP_
#define INCLUDE_MATRIX_MULTIPLICATION_HPP_



//-------------------------------------------------------------------
#include "matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------------------
/**
 * @brief Performs matrix multiplication between two matrices.
 *
 * This function overloads the multiplication operator to perform 
 * standard matrix multiplication between two matrices of potentially 
 * different types. The function employs the classical matrix multiplication 
 * algorithm with a time complexity of O(n^3).
 *
 * Template Parameters:
 * @tparam MatrixType1 Type of the first matrix (left operand).
 * @tparam MatrixType2 Type of the second matrix (right operand).
 * 
 * Function Parameters:
 * @param m1 The first matrix (left operand).
 * @param m2 The second matrix (right operand).
 *
 * @return A new matrix of type Matrix<value_type> containing the result 
 *         of the multiplication. If the matrices are empty or their dimensions 
 *         do not conform to the requirements of matrix multiplication 
 *         (i.e., the number of columns in m1 is different from the number of rows in m2), 
 *         an empty matrix with dimensions (0, 0) is returned.
 */
//-------------------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>

inline auto

operator*(const MatrixType1& m1,
          const MatrixType2& m2)
{
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType1>()(0,0))>::type;
    
    if(m1.size() > 0 && m2.size() > 0 && (m1.columns() == m2.rows()))
    {
        auto result = Matrix<value_type>(m1.rows(), m2.columns());

        for(int i = 0; i < result.rows(); ++i)
        {
            for(int j = 0; j < result.columns(); ++j)
            {
                for(int k = 0; k < m1.columns(); ++k)
                {
                    result(i,j) += m1(i,k) * m2(k,j);
                }
            }
        }

        return result;
    }
    else
    {
        return Matrix<value_type>(0, 0);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_MATRIX_MULTIPLICATION_HPP_
