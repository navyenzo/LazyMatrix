//-------------------------------------------------------------------
/**
 * @file reverse_view.hpp
 * @brief Provides the ReverseView class for creating a reversed view of matrix expressions in LazyMatrix.
 *
 * This file defines the ReverseView class, which creates a view of a matrix expression
 * with reversed rows and/or columns. It allows modification of the original data through this reversed view.
 * This class is useful for operations that require a reversed perspective of the matrix, while still
 * allowing changes to be reflected in the original data.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_REVERSE_VIEW_HPP_
#define INCLUDE_REVERSE_VIEW_HPP_



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
 * @class ReverseView
 * @brief A class that provides a reversed view of a matrix.
 *
 * ReverseView presents a reversed interface to the underlying matrix expression. It allows
 * for reversing the order of rows and/or columns, providing a flexible view for accessing
 * and modifying the data. This class does not create a copy of the data but rather provides
 * a different view of the same data with the ability to modify the original matrix.
 *
 * @tparam MatrixType The type of the underlying matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct ReverseView : public BaseMatrix< ReverseView<MatrixType> >
{
    ReverseView<MatrixType>(MatrixType& expression, bool should_rows_be_reversed, bool should_columns_be_reversed)
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



    decltype(auto) at_(int64_t row, int64_t column)const
    {
        int64_t actual_row = row;
        int64_t actual_column = column;

        if(should_rows_be_reversed_)
            actual_row = this->rows() - row - 1;
        
        if(should_columns_be_reversed_)
            actual_column = this->columns() - column - 1;

        return expression_(actual_row, actual_column);
    }

    decltype(auto) at_(int64_t row,int64_t column)
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

    MatrixType& expression_;
    bool should_rows_be_reversed_ = true;
    bool should_columns_be_reversed_ = true;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< ReverseView<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Helper function to create a ReverseView from a matrix expression.
 *
 * This function provides an easy way to create a ReverseView of a given matrix.
 * It checks at compile time if the provided object is a matrix expression and returns
 * a ReverseView object that offers access with reversed rows and/or columns.
 *
 * @tparam MatrixType The type of the matrix expression.
 * @param m Reference to the matrix expression.
 * @param should_rows_be_reversed Flag indicating if rows should be reversed.
 * @param should_columns_be_reversed Flag indicating if columns should be reversed.
 * @return ReverseView<MatrixType> A view of the matrix with reversed rows and/or columns.
 */
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

reverse_view(MatrixType& m, bool should_rows_be_reversed, bool should_columns_be_reversed)
{
    return ReverseView<MatrixType>(m, should_rows_be_reversed, should_columns_be_reversed);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_REVERSE_VIEW_HPP_
