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
 * @class ReverseView
 * @brief A class that provides a reversed view of a matrix.
 *
 * ReverseView presents a reversed interface to the underlying matrix expression. It allows
 * for reversing the order of rows and/or columns, providing a flexible view for accessing
 * and modifying the data. This class does not create a copy of the data but rather provides
 * a different view of the same data with the ability to modify the original matrix.
 *
 * @tparam ReferenceType The type of the underlying matrix expression.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

class ReverseView : public BaseMatrix< ReverseView<ReferenceType> >
{
public:

    // Type of value that is stored in the expression
    using value_type = typename ReferenceType::value_type;

    friend class BaseMatrix< ReverseView<ReferenceType> >;

    /**
     * @brief Construct a new Reverse View< Matrix Type> object
     * 
     * @param expression THe input matrix expression
     * @param should_rows_be_reversed Whether rows should be reversed
     * @param should_columns_be_reversed Whether columns should be reversed
     */
    ReverseView<ReferenceType>(ReferenceType expression, bool should_rows_be_reversed, bool should_columns_be_reversed)
    {
        set_expression(expression);
        set_should_rows_be_reversed(should_rows_be_reversed);
        set_should_columns_be_reversed(should_columns_be_reversed);
    }

    /**
     * @brief Sets the reference to the matrix expression
     * @param left_side_expression Reference to the matrix.
     */
    void set_expression(ReferenceType expression)
    {
        expression_ = expression;
    }

    /**
     * @brief Set the should rows be reversed object
     * 
     * @param should_rows_be_reversed 
     */
    void set_should_rows_be_reversed(bool should_rows_be_reversed)
    {
        should_rows_be_reversed_ = should_rows_be_reversed;
    }

    /**
     * @brief Set the should columns be reversed object
     * 
     * @param should_columns_be_reversed 
     */
    void set_should_columns_be_reversed(bool should_columns_be_reversed)
    {
        should_columns_be_reversed_ = should_columns_be_reversed;
    }

    /**
     * @brief Returns the number of rows Of the resulting matrix.
     */
    uintptr_t rows()const
    {
        return this->expression_.rows();
    }

    /**
     * @brief Returns the total number of columns of the resulting matrix.
     */
    uintptr_t columns()const
    {
        return this->expression_.columns();
    }



private: // Private functions

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A copy of the value of the element at the specified position.
     */
    value_type const_at_(int64_t row, int64_t column)const
    {
        int64_t actual_row = row;
        int64_t actual_column = column;

        if(should_rows_be_reversed_)
            actual_row = this->rows() - row - 1;
        
        if(should_columns_be_reversed_)
            actual_column = this->columns() - column - 1;

        return expression_(actual_row, actual_column);
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A reference to the element at the specified position.
     */
    value_type& non_const_at_(int64_t row,int64_t column)
    {
        int64_t actual_row = row;
        int64_t actual_column = column;

        if(should_rows_be_reversed_)
            actual_row = this->rows() - row - 1;
        
        if(should_columns_be_reversed_)
            actual_column = this->columns() - column - 1;

        return expression_(actual_row, actual_column);
    }



private: // Private variables

    ReferenceType expression_;
    bool should_rows_be_reversed_ = true;
    bool should_columns_be_reversed_ = true;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename ReferenceType>

struct is_type_a_matrix< ReverseView<ReferenceType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Creates a reverse view of an input matrix expression
 * @tparam ReferenceType Type of the input matrix expression.
 * @param m Shared reference to the input matrix expression
 * @param should_rows_be_reversed Flag indicating if rows should be reversed.
 * @param should_columns_be_reversed Flag indicating if columns should be reversed.
 * @return A SharedMatrixRef or ConstSharedMatrixRef to the
 *         ReverseView matrix object.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline auto

create_reversed_matrix_view(ReferenceType m,
                            bool should_rows_be_reversed,
                            bool should_columns_be_reversed)
{
    auto view = std::make_shared<ReverseView<ReferenceType>>(m, should_rows_be_reversed, should_columns_be_reversed);
    return SharedMatrixRef<ReverseView<ReferenceType>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_REVERSE_VIEW_HPP_
