//-------------------------------------------------------------------
/**
 * @file reverse.hpp
 * @brief Provides the Reverse class for creating a non-modifiable reversed view of matrix expressions in LazyMatrix.
 *
 * This file defines the Reverse class, which creates a non-modifiable reversed view of a matrix expression
 * with reversed rows and/or columns. This class is useful for operations that require a reversed perspective
 * of the matrix without modifying the original data.
 *
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @namespace LazyMatrix
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_REVERSE_HPP_
#define INCLUDE_REVERSE_HPP_



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
 * @class Reverse
 * @brief A class that provides a non-modifiable reversed view of a matrix.
 *
 * Reverse presents a reversed interface to the underlying matrix expression. It allows
 * for reversing the order of rows and/or columns, providing a flexible view for accessing
 * the data. This class does not allow modification of the underlying data but rather provides
 * a different view of the same data.
 *
 * @tparam MatrixType The type of the underlying matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct Reverse : public BaseMatrix< Reverse<MatrixType> >
{
    Reverse<MatrixType>(const MatrixType& expression, bool should_rows_be_reversed, bool should_columns_be_reversed)
    : expression_(expression),
      should_rows_be_reversed_(should_rows_be_reversed),
      should_columns_be_reversed_(should_columns_be_reversed)
    {
    }
    


    uintptr_t rows()const
    {
        return this->expression_.rows();
    }

    uintptr_t columns()const
    {
        return this->expression_.columns();
    }



    decltype(auto) at(int64_t row, int64_t column)const
    {
        int64_t actual_row = row;
        int64_t actual_column = column;

        if(should_rows_be_reversed_)
            actual_row = this->rows() - row - 1;
        
        if(should_columns_be_reversed_)
            actual_column = this->columns() - column - 1;

        return expression_(actual_row, actual_column);
    }



    const MatrixType& get_expression()
    {
        return expression_;
    }



private:

    const MatrixType& expression_;
    bool should_rows_be_reversed_ = true;
    bool should_columns_be_reversed_ = true;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< Reverse<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Helper function to create a Reverse object from a matrix expression.
 *
 * This function provides an easy way to create a Reverse object of a given matrix.
 * It checks at compile time if the provided object is a matrix expression and returns
 * a Reverse object that offers access with reversed rows and/or columns.
 *
 * @tparam MatrixType The type of the matrix expression.
 * @param m Reference to the matrix expression.
 * @param should_rows_be_reversed Flag indicating if rows should be reversed.
 * @param should_columns_be_reversed Flag indicating if columns should be reversed.
 * @return Reverse<MatrixType> A view of the matrix with reversed rows and/or columns.
 */
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

reverse(const MatrixType& m, bool should_rows_be_reversed, bool should_columns_be_reversed)
{
    return Reverse<MatrixType>(m, should_rows_be_reversed, should_columns_be_reversed);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_REVERSE_HPP_
