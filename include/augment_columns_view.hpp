//-------------------------------------------------------------------
/**
 * @file augment_columns_view.hpp
 * @brief Provides functionality to augment two matrices column-wise.
 *
 * This file contains the AugmentColumnsView template class, which is used to 
 * create a view combining two matrices by augmenting their columns. It essentially
 * places the columns of the second matrix to the right of the first matrix.
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
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class AugmentColumnsView
 * @brief Augments two matrices by columns to create a new matrix view.
 *
 * @tparam MatrixType1 Type of the left side matrix in the augmentation.
 * @tparam MatrixType2 Type of the right side matrix in the augmentation.
 */
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2>

struct AugmentColumnsView : public BaseMatrix< AugmentColumnsView<MatrixType1, MatrixType2> >
{
    // Type of value that is stored in the expression
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<MatrixType1>()(0,0))>::type>::type;



    /**
     * @brief Constructs a view which augments the columns of two matrices.
     *
     * @param left_side_expression Reference to the left side matrix.
     * @param right_side_expression Reference to the right side matrix.
     */
    AugmentColumnsView<MatrixType1, MatrixType2>(MatrixType1& left_side_expression,
                                                 MatrixType2& right_side_expression)
    : left_side_expression_(left_side_expression),
      right_side_expression_(right_side_expression)
    {
    }



    uintptr_t rows()const
    {
        return std::max(this->left_side_expression_.rows(), this->right_side_expression_.rows());
    }

    uintptr_t columns()const
    {
        return this->left_side_expression_.columns() + this->right_side_expression_.columns();
    }



    const MatrixType1& get_left_side_expression()const
    {
        return left_side_expression_;
    }
    
    const MatrixType2& get_right_side_expression()const
    {
        return right_side_expression_;
    }

    

    const value_type& at_(int64_t row, int64_t column)const
    {
        if(column < this->left_side_expression_.columns())
        {
            if(row < this->left_side_expression_.rows())
                return this->left_side_expression_.at(row, column);
            else
                return zero_;
        }
        else
        {
            if(row < this->right_side_expression_.rows())
                return this->right_side_expression_.at(row, column - this->left_side_expression_.columns());
            else
                return zero_;
        }
    }

    value_type& at_(int64_t row, int64_t column)
    {
        if(column < this->left_side_expression_.columns())
        {
            if(row < this->left_side_expression_.rows())
                return this->left_side_expression_.at(row, column);
            else
                return zero_;
        }
        else
        {
            if(row < this->right_side_expression_.rows())
                return this->right_side_expression_.at(row, column - this->left_side_expression_.columns());
            else
                return zero_;
        }
    }



private:

    MatrixType1& left_side_expression_;
    MatrixType2& right_side_expression_;

    value_type zero_ = static_cast<value_type>(0);
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2>

struct is_type_a_matrix< AugmentColumnsView<MatrixType1, MatrixType2> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Function to augment two matrices by columns.
 *
 * This function takes two matrix expressions and creates a view that represents
 * the augmentation of these matrices by columns.
 *
 * @tparam MatrixType1 Type of the left side matrix.
 * @tparam MatrixType2 Type of the right side matrix.
 * @param m1 Reference to the left side matrix.
 * @param m2 Reference to the right side matrix.
 * @return An AugmentColumnsView object representing the augmented matrix.
 */
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>

inline auto

augment_by_columns_view(MatrixType1& m1,
                        MatrixType2& m2)
{
    return AugmentColumnsView<MatrixType1,MatrixType2>(m1, m2);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_AUGMENT_COLUMNS_VIEW_HPP_
