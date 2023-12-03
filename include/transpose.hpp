//-------------------------------------------------------------------
/**
 * @file transpose.hpp
 * @brief Provides the Transpose class for creating a transposed view of matrix expressions in LazyMatrix.
 *
 * This file defines the Transpose class, which creates a read-only view of a matrix expression
 * where the rows and columns are swapped (transposed). Unlike TransposeView, Transpose does not allow
 * modifications to the original matrix through the view, ensuring that the original data remains unchanged.
 * This class is useful for operations that require a transpose view without the need for data modification.
 *
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @namespace LazyMatrix
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_TRANSPOSE_HPP_
#define INCLUDE_TRANSPOSE_HPP_



//-------------------------------------------------------------------
#include "base_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class Transpose
 * @brief A class that provides a read-only transposed view of a matrix.
 *
 * Transpose presents a transposed interface to the underlying matrix expression. It swaps
 * rows and columns for read-only access, allowing for operations as if the matrix were transposed.
 * This class does not create a copy of the data, but rather provides a different view of the same data
 * without allowing modifications to the original matrix.
 *
 * @tparam MatrixType The type of the underlying matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

class Transpose : public BaseMatrix< Transpose<MatrixType> >
{
public:

    Transpose<MatrixType>(const MatrixType& expression)
    : expression_(expression)
    {
    }
    


    uintptr_t rows()const
    {
        return this->expression_.columns();
    }

    uintptr_t columns()const
    {
        return this->expression_.rows();
    }



    const MatrixType& get_expression()
    {
        return expression_;
    }



    decltype(auto) at_(const int64_t& row,const int64_t& column)const
    {
        return expression_(column, row);
    }



private:

    const MatrixType& expression_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< Transpose<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Helper function to create a Transpose from a matrix expression.
 *
 * This function provides an easy way to create a Transpose of a given matrix.
 * It checks at compile time if the provided object is a matrix expression and returns
 * a Transpose object that offers read-only access.
 *
 * @tparam MatrixType The type of the matrix expression.
 * @param m Reference to the matrix expression.
 * @return Transpose<MatrixType> A read-only transposed view of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

transpose(const MatrixType& m)
{
    return Transpose<MatrixType>(m);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_TRANSPOSE_HPP_
