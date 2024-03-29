//-------------------------------------------------------------------
/**
 * @file border_functor_view.hpp
 * @brief Provides views for matrix border handling.
 *
 * This file contains the RepeatedBorderView and ConstantBorderView template classes,
 * offering views to handle border cases when accessing matrix elements outside the
 * bounds. RepeatedBorderView returns the value at the matrix's border, while
 * ConstantBorderView returns a user-specified constant value.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_BORDER_FUNCTOR_VIEW_HPP_
#define INCLUDE_BORDER_FUNCTOR_VIEW_HPP_



//-------------------------------------------------------------------
#include <algorithm>

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
 * @class RepeatedBorderView
 * @brief View that handles border cases by repeating border values.
 *
 * @tparam ReferenceType Type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

struct RepeatedBorderView : public BaseMatrix<RepeatedBorderView<ReferenceType>,
                                              has_non_const_access<ReferenceType>::value>
{
    // Type of value that is stored in the expression
    using value_type = typename ReferenceType::value_type;

    friend class BaseMatrix<RepeatedBorderView<ReferenceType>,
                            has_non_const_access<ReferenceType>::value>;

    /**
     * @brief Constructs a new matrix view by repeating border values.
     *
     * @param expression Reference to the input matrix expression.
     */
    RepeatedBorderView(ReferenceType expression)
    {
        set_expression(expression);
    }

    /**
     * @brief Sets the reference to the input matrix expression
     * @param expression Reference to the input matrix expression.
     */
    void set_expression(ReferenceType expression)
    {
        expression_ = expression;
    }

    /**
     * @brief Returns the number of rows in the augmented matrix.
     * @return The maximum number of rows between both matrices.
     */
    uintptr_t rows()const
    {
        return expression_.rows();
    }

    /**
     * @brief Returns the total number of columns in the augmented matrix.
     * @return Sum of the columns of both matrices.
     */
    uintptr_t columns()const
    {
        return expression_.columns();
    }

    // Functions used to handle row and column header names
    std::string get_row_header(int64_t row_index) const { return expression_.get_row_header(row_index); }
    std::string get_column_header(int64_t column_index) const { return expression_.get_column_header(column_index); }
    void set_row_header(int64_t row_index, const std::string& row_header) const { expression_.set_row_header(row_index, row_header); }
    void set_column_header(int64_t column_index, const std::string& column_header) const { expression_.set_column_header(column_index, column_header); }



private: // Private functions

    /**
     * @brief Dummy "resize" function needed for the matrix interface, but
     *        here it doesn't do anything
     * 
     * @param rows 
     * @param columns 
     * @return std::error_code 
     */
    std::error_code resize_(uintptr_t rows, uintptr_t columns)
    {
        return std::error_code();
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A copy of the value of the element at the specified position.
     */
    value_type const_at_(int64_t row, int64_t column)const
    {
        row = std::max(row, int64_t(0));
        row = std::min(this->rows() - 1, row);

        column = std::max(column, int64_t(0));
        column = std::min(this->columns() - 1, column);

        return expression_(row, column);
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A reference to the element at the specified position.
     */
    template<typename T = ReferenceType>
    std::enable_if_t<has_non_const_access<T>::value, value_type&>
    non_const_at_(int64_t row, int64_t column)
    {
        row = std::max(row, int64_t(0));
        row = std::min(this->rows() - 1, row);
        
        column = std::max(column, int64_t(0));
        column = std::min(this->columns() - 1, column);

        return expression_(row, column);
    }



private: // Private variables

    ReferenceType expression_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename ReferenceType>

struct is_type_a_matrix< RepeatedBorderView<ReferenceType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class ConstantBorderView
 * @brief View that handles border cases by returning a constant value.
 *
 * @tparam ReferenceType Type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

class ConstantBorderView : public BaseMatrix<ConstantBorderView<ReferenceType>,
                                             has_non_const_access<ReferenceType>::value>
{
public:

    // Type of value that is stored in the expression
    using value_type = typename ReferenceType::value_type;

    friend class BaseMatrix<ConstantBorderView<ReferenceType>,
                            has_non_const_access<ReferenceType>::value>;
    
    /**
     * @brief Constructs a new matrix view that gives a constant border
     *        value when asking for values outside of the input matrix bounds.
     *
     * @param expression Reference to the input matrix expression.
     * @param constant_value The constant value returned when asking for values
     *                       outside of the input matrix bounds.
     */
    ConstantBorderView(ReferenceType expression, const value_type& constant_value = static_cast<value_type>(0))
    {
        set_constant_value(constant_value);
        set_expression(expression);
    }

    /**
     * @brief Sets the reference to the input matrix expression
     * @param expression Reference to the input matrix expression.
     */
    void set_expression(ReferenceType expression)
    {
        expression_ = expression;
    }

    /**
     * @brief Sets the constant value returned when asking for a value
     *        outside the bounds of the input matrix.
     * @param constant_value The value to be returned.
     */
    void set_constant_value(const value_type& constant_value)
    {
        constant_value_ = constant_value;
    }

    /**
     * @brief Returns the number of rows in the augmented matrix.
     * @return The maximum number of rows between both matrices.
     */
    uintptr_t rows()const
    {
        return expression_.rows();
    }

    /**
     * @brief Returns the total number of columns in the augmented matrix.
     * @return Sum of the columns of both matrices.
     */
    uintptr_t columns()const
    {
        return expression_.columns();
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
        if(row < 0 || row >= this->rows() || column < 0 || column >= this->columns())
            return constant_value_;

        return expression_(row, column);
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A reference to the element at the specified position.
     */
    template<typename T = ReferenceType>
    std::enable_if_t<has_non_const_access<T>::value, value_type&>
    non_const_at_(int64_t row, int64_t column)
    {
        if(row < 0 || row >= this->rows() || column < 0 || column >= this->columns())
            return constant_value_;

        return expression_(row, column);
    }



private: // Private variables

    ReferenceType expression_;
    mutable value_type constant_value_ = static_cast<value_type>(0);
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename ReferenceType>

struct is_type_a_matrix< ConstantBorderView<ReferenceType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Creates a view of an input matrix such that if the user
 *        tries to access values outside the bounds of the input
 *        matrix, they will get the border value closest to it.
 * @tparam ReferenceType Type of the input matrix expression.
 * @param m Shared reference to the input matrix expression.
 * @return A SharedMatrixRef to the RepeatedBorderView matrix object.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline auto

repeated_border_view(ReferenceType m)
{
    auto view = std::make_shared<RepeatedBorderView<ReferenceType>>(m);

    if constexpr (has_non_const_access<ReferenceType>::value)
    {
        return SharedMatrixRef<RepeatedBorderView<ReferenceType>>(view);
    }
    else
    {
        return ConstSharedMatrixRef<RepeatedBorderView<ReferenceType>>(view);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Creates a view of an input matrix such that if the user
 *        tries to access values outside the bounds of the input
 *        matrix, they will get a user specified constant value defaulted
 *        to Zero.
 * @tparam ReferenceType Type of the input matrix expression.
 * @param m Shared reference to the input matrix expression.
 * @return A SharedMatrixRef to the ConstantBorderView matrix object.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline auto

constant_border_view(ReferenceType m)
{
    auto view = std::make_shared<ConstantBorderView<ReferenceType>>(m);

    if constexpr (has_non_const_access<ReferenceType>::value)
    {
        return SharedMatrixRef<ConstantBorderView<ReferenceType>>(view);
    }
    else
    {
        return ConstSharedMatrixRef<ConstantBorderView<ReferenceType>>(view);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_BORDER_FUNCTOR_VIEW_HPP_
