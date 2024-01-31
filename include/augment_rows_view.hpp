//-------------------------------------------------------------------
/**
 * @file augment_rows_view.hpp
 * @brief Provides functionality to augment two matrices row-wise as a view.
 *
 * This file contains the AugmentRowsView template class, which is used to 
 * create a view combining two matrices by augmenting their rows. This view 
 * essentially places the rows of the second matrix below the first matrix, 
 * creating a new matrix view without copying the data.
 * 
 * This file contains the AugmentRowsView template class, which is used to 
 * create a view of two input matrix expressions combined together by
 * augmenting their rows.  It essentially places the rows of the second
 * matrix expression below the first matrix expression.
 * This class only creates a view and doesn't actually copy any data.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_AUGMENT_ROWS_VIEW_HPP_
#define INCLUDE_AUGMENT_ROWS_VIEW_HPP_



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
 * @class AugmentRowsView
 * @brief Augments two matrices by rows to create a new matrix view.
 *
 * @tparam ReferenceType1 Type of the upper matrix in the augmentation.
 * @tparam ReferenceType2 Type of the lower matrix in the augmentation.
 */
//-------------------------------------------------------------------
template<typename ReferenceType1,
         typename ReferenceType2,
         std::enable_if_t<is_matrix_reference<ReferenceType1>{}>* = nullptr,
         std::enable_if_t<is_matrix_reference<ReferenceType2>{}>* = nullptr>

class AugmentRowsView : public BaseMatrix<AugmentRowsView<ReferenceType1, ReferenceType2>,
                                          has_non_const_access<ReferenceType1>::value && has_non_const_access<ReferenceType2>::value>
{
public:

    // Type of value that is stored in the expression
    using value_type = typename ReferenceType1::value_type;

    friend class BaseMatrix<AugmentRowsView<ReferenceType1, ReferenceType2>,
                            has_non_const_access<ReferenceType1>::value && has_non_const_access<ReferenceType2>::value>;

    /**
     * @brief Constructs a new matrix view by augmenting the rows of two matrices.
     *
     * @param top_side_expression Reference to the upper matrix.
     * @param bottom_side_expression Reference to the lower matrix.
     */
    AugmentRowsView(ReferenceType1 top_side_expression,
                    ReferenceType2 bottom_side_expression)
    {
        set_top_side_expression(top_side_expression);
        set_bottom_side_expression(bottom_side_expression);
    }
    
    /**
     * @brief Sets the reference to the top side matrix expression
     * @param top_side_expression Reference to the left side matrix.
     */
    void set_top_side_expression(ReferenceType1 top_side_expression)
    {
        top_side_expression_ = top_side_expression;
    }

    /**
     * @brief Sets the reference to the right side matrix expression
     * @param bottom_side_expression Reference to the right side matrix.
     */
    void set_bottom_side_expression(ReferenceType2 bottom_side_expression)
    {
        bottom_side_expression_ = bottom_side_expression;
    }

    /**
     * @brief Returns the number of rows in the augmented matrix.
     * @return The maximum number of rows between both matrices.
     */
    uintptr_t rows()const
    {
        return this->top_side_expression_.rows() + this->bottom_side_expression_.rows();
    }

    /**
     * @brief Returns the total number of columns in the augmented matrix.
     * @return Sum of the columns of both matrices.
     */
    uintptr_t columns()const
    {
        return std::max(this->top_side_expression_.columns(), this->bottom_side_expression_.columns());
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
    value_type const_at_(int64_t row, int64_t column)const
    {
        if(row < this->top_side_expression_.rows())
        {
            if(column < this->top_side_expression_.columns())
                return this->top_side_expression_.at(row, column);
            else
                return DummyValueHolder<value_type>::zero;
        }
        else
        {
            if(column < this->bottom_side_expression_.columns())
                return this->bottom_side_expression_.at(row - this->top_side_expression_.rows(), column);
            else
                return DummyValueHolder<value_type>::zero;
        }
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A reference to the element at the specified position.
     */
    template<typename T1 = ReferenceType1, typename T2 = ReferenceType2>
    std::enable_if_t<has_non_const_access<T1>::value && has_non_const_access<T2>::value, value_type&>
    non_const_at_(int64_t row, int64_t column)
    {
        if(row < this->top_side_expression_.rows())
        {
            if(column < this->top_side_expression_.columns())
                return this->top_side_expression_.at(row, column);
            else
                return DummyValueHolder<value_type>::zero;
        }
        else
        {
            if(column < this->bottom_side_expression_.columns())
                return this->bottom_side_expression_.at(row - this->top_side_expression_.rows(), column);
            else
                return DummyValueHolder<value_type>::zero;
        }
    }



private: // Private variables

    ReferenceType1 top_side_expression_;
    ReferenceType2 bottom_side_expression_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename ReferenceType1,
         typename ReferenceType2>

struct is_type_a_matrix< AugmentRowsView<ReferenceType1, ReferenceType2> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Creates an augmented view of two matrices by rows.
 * @tparam ReferenceType1 Type of the top side matrix.
 * @tparam ReferenceType2 Type of the bottom side matrix.
 * @param m1 Shared reference to the top side matrix.
 * @param m2 Shared reference to the bottom side matrix.
 * @return A SharedMatrixRef to the AugmentRowsView combining both matrices.
 */
//-------------------------------------------------------------------
template<typename ReferenceType1,
         typename ReferenceType2,
         std::enable_if_t<is_matrix_reference<ReferenceType1>{}>* = nullptr,
         std::enable_if_t<is_matrix_reference<ReferenceType2>{}>* = nullptr>

inline auto

augment_by_rows_view(ReferenceType1 m1,
                     ReferenceType2 m2)
{
    auto view = std::make_shared<AugmentRowsView<ReferenceType1, ReferenceType2>>(m1, m2);

    if constexpr (has_non_const_access<ReferenceType1>::value && has_non_const_access<ReferenceType2>::value)
    {
        return SharedMatrixRef<AugmentRowsView<ReferenceType1, ReferenceType2>>(view);
    }
    else
    {
        return ConstSharedMatrixRef<AugmentRowsView<ReferenceType1, ReferenceType2>>(view);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_AUGMENT_ROWS_VIEW_HPP_
