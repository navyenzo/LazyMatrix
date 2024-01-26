//-------------------------------------------------------------------
/**
 * @file matrix_multiplication.hpp
 * @brief Provides functionality for matrix multiplication in the LazyMatrix library.
 *
 * This file defines a function for performing matrix multiplication between two matrices,
 * supporting matrices of different types. The multiplication follows the standard matrix
 * multiplication algorithm with a time complexity of O(n^3). If dimensions are not compatible
 * for multiplication, an empty matrix is returned.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_MATRIX_MULTIPLICATION_HPP_
#define INCLUDE_MATRIX_MULTIPLICATION_HPP_



//-------------------------------------------------------------------
#include "simple_matrix.hpp"
#include "matrix_factory.hpp"
#include "shared_references.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Performs matrix multiplication between two matrices.
 * 
 * This function overloads the multiplication operator to perform 
 * standard matrix multiplication between two matrices of potentially 
 * different types. The function employs the classical matrix multiplication 
 * algorithm with a time complexity of O(n^3).
 * 
 * @tparam ReferenceType1 Type of the left side matrix.
 * @tparam ReferenceType2 Type of the right side matrix.
 * 
 * @param m1 Shared reference to the left side matrix.
 * @param m2 Shared reference to the right side matrix.
 * 
 * @return A new matrix of type SimpleMatrix<value_type> containing the result 
 *         of the multiplication. If the matrices are empty or their dimensions 
 *         do not conform to the requirements of matrix multiplication 
 *         (i.e., the number of columns in m1 is different from the number of rows in m2), 
 *         an empty matrix with dimensions (0, 0) is returned.
 */
//-------------------------------------------------------------------
template<typename ReferenceType1,
         typename ReferenceType2,
         std::enable_if_t<is_matrix_reference<ReferenceType1>{}>* = nullptr,
         std::enable_if_t<is_matrix_reference<ReferenceType2>{}>* = nullptr>

inline auto

operator*(const ReferenceType1& m1,
          const ReferenceType2& m2)
{
    using value_type = typename ReferenceType1::value_type;
    
    if(m1.size() > 0 && m2.size() > 0 && (m1.columns() == m2.rows()))
    {
        auto result = MatrixFactory::create_simple_matrix<value_type>(m1.rows(), m2.columns());

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
        return MatrixFactory::create_simple_matrix<value_type>(0,0);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_MATRIX_MULTIPLICATION_HPP_
