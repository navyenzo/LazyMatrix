//-------------------------------------------------------------------
/**
 * @file augment_columns_view.hpp
 * @brief Provides functionality to augment two matrices column-wise.
 *
 * This file contains the AugmentColumnsView template class, which is used to 
 * create a view of two input matrix expressions combined together by
 * augmenting their columns.  It essentially places the columns of the second
 * matrix expression to the right of the first matrix expression.
 * This class only creates a view and doesn't actually copy any data.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_AUGMENT_COLUMNS_VIEW_HPP_
#define INCLUDE_AUGMENT_COLUMNS_VIEW_HPP_



//-------------------------------------------------------------------
#include "base_matrix.hpp"
#include "shared_references.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------
/**
 * @class AugmentColumnsView
 * @brief Augments two matrices by columns to create a new matrix view.
 *
 * @tparam ReferenceType1 Type of the left side matrix in the augmentation.
 * @tparam ReferenceType2 Type of the right side matrix in the augmentation.
 *
 * This class represents a view that augments the columns of two matrices.
 * It places the columns of the second matrix to the right of the first matrix,
 * creating a combined matrix view without modifying the original matrices.
 */
//-------------------------------------------------------------------
template<typename ReferenceType1,
         typename ReferenceType2,
         std::enable_if_t<is_matrix_reference<ReferenceType1>{}>* = nullptr,
         std::enable_if_t<is_matrix_reference<ReferenceType2>{}>* = nullptr>

class AugmentColumnsView : public BaseMatrix<AugmentColumnsView<ReferenceType1, ReferenceType2>,
                                             typename ReferenceType1::value_type,
                                             has_non_const_access<ReferenceType1>::value && has_non_const_access<ReferenceType2>::value>
{
public:

    // Type of value that is stored in the matrix
    using value_type = typename ReferenceType1::value_type;

    friend class BaseMatrix<AugmentColumnsView<ReferenceType1, ReferenceType2>,
                            typename ReferenceType1::value_type,
                            has_non_const_access<ReferenceType1>::value && has_non_const_access<ReferenceType2>::value>;

    /**
     * @brief Constructs a new matrix by augmenting the columns of two matrices.
     * @param left_side_expression Reference to the left side matrix.
     * @param right_side_expression Reference to the right side matrix.
     */
    AugmentColumnsView(ReferenceType1 left_side_expression,
                       ReferenceType2 right_side_expression)
    {
        set_left_side_expression(left_side_expression);
        set_right_side_expression(right_side_expression);
    }
    
    /**
     * @brief Sets the reference to the left side matrix expression
     * @param left_side_expression Reference to the left side matrix.
     */
    void set_left_side_expression(ReferenceType1 left_side_expression)
    {
        left_side_expression_ = left_side_expression;
    }

    /**
     * @brief Sets the reference to the right side matrix expression
     * @param right_side_expression Reference to the right side matrix.
     */
    void set_right_side_expression(ReferenceType2 right_side_expression)
    {
        right_side_expression_ = right_side_expression;
    }

    /**
     * @brief Returns the number of rows in the augmented matrix.
     * @return The maximum number of rows between both matrices.
     */
    uintptr_t rows() const
    {
        return std::max(left_side_expression_.rows(), right_side_expression_.rows());
    }

    /**
     * @brief Returns the total number of columns in the augmented matrix.
     * @return Sum of the columns of both matrices.
     */
    uintptr_t columns() const
    {
        return left_side_expression_.columns() + right_side_expression_.columns();
    }



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
    value_type const_at_(int64_t row, int64_t column) const
    {
        if(column < left_side_expression_.columns())
            return row < left_side_expression_.rows() ? left_side_expression_.at(row, column) : DummyValueHolder<value_type>::zero;
        else
            return row < right_side_expression_.rows() ? right_side_expression_.at(row, column - left_side_expression_.columns()) : DummyValueHolder<value_type>::zero;
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A reference to the element at the specified position.
     */
    std::enable_if_t<has_non_const_access<ReferenceType1>::value && has_non_const_access<ReferenceType2>::value, value_type&>
    non_const_at_(int64_t row, int64_t column)
    {
        if(column < left_side_expression_.columns())
            return row < left_side_expression_.rows() ? left_side_expression_.at(row, column) : DummyValueHolder<value_type>::zero;
        else
            return row < right_side_expression_.rows() ? right_side_expression_.at(row, column - left_side_expression_.columns()) : DummyValueHolder<value_type>::zero;
    }



private: // Private variables

    ReferenceType1 left_side_expression_;
    ReferenceType2 right_side_expression_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Creates an augmented view of two matrices by columns.
 * @tparam ReferenceType1 Type of the left side matrix.
 * @tparam ReferenceType2 Type of the right side matrix.
 * @param m1 Shared reference to the left side matrix.
 * @param m2 Shared reference to the right side matrix.
 * @return A SharedMatrixRef to the AugmentColumnsView combining both matrices.
 */
//-------------------------------------------------------------------
template<typename ReferenceType1,
         typename ReferenceType2,
         std::enable_if_t<is_matrix_reference<ReferenceType1>{}>* = nullptr,
         std::enable_if_t<is_matrix_reference<ReferenceType2>{}>* = nullptr>

inline auto

augment_by_columns_view(ReferenceType1 m1,
                        ReferenceType2 m2)
{
    auto view = std::make_shared<AugmentColumnsView<ReferenceType1, ReferenceType2>>(m1, m2);

    if constexpr (has_non_const_access<ReferenceType1>::value && has_non_const_access<ReferenceType2>::value)
    {
        return SharedMatrixRef<AugmentColumnsView<ReferenceType1, ReferenceType2>>(view);
    }
    else
    {
        return ConstSharedMatrixRef<AugmentColumnsView<ReferenceType1, ReferenceType2>>(view);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_AUGMENT_COLUMNS_VIEW_HPP_
