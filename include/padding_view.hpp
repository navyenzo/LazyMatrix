//-------------------------------------------------------------------
/**
 * @file padding_view.hpp
 * @brief Provides functionality for padding matrix expressions.
 *
 * This file contains the PaddedMatrix template class, which is used to pad a matrix expression 
 * with a constant value. This is useful for algorithms that require matrices with specific 
 * dimensions or for handling boundary conditions. The file also includes utility functions to 
 * facilitate the creation of padded matrices.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_PADDING_VIEW_HPP_
#define INCLUDE_PADDING_VIEW_HPP_



//-------------------------------------------------------------------
#include <algorithm>
#include <cstdint>

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
 * @class PaddedMatrix
 * @brief Pads matrix expression with a constant value
 *
 * @tparam ReferenceType Type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

class PaddedMatrixView : public BaseMatrix<PaddedMatrixView<ReferenceType>,
                                           has_non_const_access<ReferenceType>::value>
{
public:

    // Type of value that is stored in the expression
    using value_type = typename ReferenceType::value_type;

    friend class BaseMatrix<PaddedMatrixView<ReferenceType>,
                            has_non_const_access<ReferenceType>::value>;

    /**
     * @brief Construct a new Padded Matrix View< Matrix Type> object.
     * 
     * @param expression The input matrix expression.
     * @param padded_rows Number of rows after padding.
     * @param padded_columns Number of columns after padding.
     * @param constant_value_for_padding value used for the padding (defaulted to zero).
     */
    PaddedMatrixView(ReferenceType expression,
                     uintptr_t padded_rows,
                     uintptr_t padded_columns,
                     value_type constant_value_for_padding = static_cast<value_type>(0))
    {
        set_expression(expression);
        set_padded_rows(padded_rows);
        set_padded_columns(padded_columns);
        set_constant_value_for_padding(constant_value_for_padding);
    }

    /**
     * @brief Sets the reference to the matrix expression
     * @param expression Reference to the matrix.
     */
    void set_expression(ReferenceType expression)
    {
        expression_ = expression;
    }

    /**
     * @brief Set the number of rows after padding
     * 
     * @param padded_rows 
     */
    void set_padded_rows(uintptr_t padded_rows)
    {
        padded_rows_ = padded_rows;
    }

    /**
     * @brief Set the number of columns after padding
     * 
     * @param padded_columns
     */
    void set_padded_columns(uintptr_t padded_columns)
    {
        padded_columns_ = padded_columns;
    }

    /**
     * @brief Set the constant value used for the padding
     * 
     * @param constant_value_for_padding 
     */
    void set_constant_value_for_padding(const value_type& constant_value_for_padding)
    {
        constant_value_for_padding_ = constant_value_for_padding;
    }

    /**
     * @brief Returns the number of rows Of the resulting matrix.
     */
    uintptr_t rows()const
    {
        return padded_rows_;
    }

    /**
     * @brief Returns the total number of columns of the resulting matrix.
     */
    uintptr_t columns()const
    {
        return padded_columns_;
    }

    // Functions used to handle row and column header names
    std::string get_row_header(int64_t row_index) const
    {
        if(row_index < expression_.rows())
            return expression_.get_row_header(row_index);
        else
            return this->headers_.get_row_header(row_index);
    }
    
    std::string get_column_header(int64_t column_index) const
    {
        if(column_index < expression_.columns())
            return expression_.get_column_header(column_index);
        else
            return this->headers_.get_column_header(column_index - expression_.columns());
    }

    void set_row_header(int64_t row_index, const std::string& row_header) const
    {
        if(row_index < expression_.rows())
            expression_.set_row_header(row_index, row_header);
        else
            this->headers_.set_row_header(row_index, row_header);
    }

    void set_column_header(int64_t column_index, const std::string& column_header) const
    {
        if(column_index < expression_.columns())
            expression_.set_column_header(column_index, column_header);
        else
            this->headers_.set_column_header(column_index - expression_.columns(), column_header);
    }



private: // Private functions

    /**
     * @brief Dummy "resize" function needed for the matrix interface, but
     *        here it doesn't do anything
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
        if(row < 0 || row >= expression_.rows() || column < 0 || column >= expression_.columns())
            return constant_value_for_padding_;

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
        if(row < 0 || row >= expression_.rows() || column < 0 || column >= expression_.columns())
            return constant_value_for_padding_;

        return expression_(row, column);
    }



private: // Private variables

    ReferenceType expression_;
    uintptr_t padded_rows_;
    uintptr_t padded_columns_;
    value_type constant_value_for_padding_ = static_cast<value_type>(0);
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename ReferenceType>

struct is_type_a_matrix< PaddedMatrixView<ReferenceType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Creates a padded view of a matrix.
 * @tparam ReferenceType Type of the input matrix expression.
 * @param m The input matrix expression.
 * @param padded_rows Number of rows after padding.
 * @param padded_columns Number of columns after padding.
 * @param constant_value_for_padding value used for the padding (defaulted to zero).
 * @return A SharedMatrixRef to the PaddedMatrixView matrix object.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline auto

create_padded_matrix_view(ReferenceType m,
                          uintptr_t padded_rows,
                          uintptr_t padded_columns,
                          typename std::remove_const<typename std::remove_reference<decltype(std::declval<ReferenceType>()(0,0))>::type>::type constant_value_for_padding = 0)
{
    auto view = std::make_shared<PaddedMatrixView<ReferenceType>>(m, padded_rows, padded_columns);

    if constexpr (has_non_const_access<ReferenceType>::value)
    {
        return SharedMatrixRef<PaddedMatrixView<ReferenceType>>(view);
    }
    else
    {
        return ConstSharedMatrixRef<PaddedMatrixView<ReferenceType>>(view);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_PADDING_VIEW_HPP_
