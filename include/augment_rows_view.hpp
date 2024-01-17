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
 * @tparam MatrixType1 Type of the upper matrix in the augmentation.
 * @tparam MatrixType2 Type of the lower matrix in the augmentation.
 */
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2>

struct AugmentRowsView : public BaseMatrix< AugmentRowsView<MatrixType1, MatrixType2> >
{
    // Type of value that is stored in the expression
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<MatrixType1>()(0,0))>::type>::type;



    /**
     * @brief Constructs a new matrix view by augmenting the rows of two matrices.
     *
     * @param left_side_expression Reference to the upper matrix.
     * @param right_side_expression Reference to the lower matrix.
     */
    AugmentRowsView<MatrixType1, MatrixType2>(MatrixType1& left_side_expression,
                                              MatrixType2& right_side_expression)
    : left_side_expression_(left_side_expression),
      right_side_expression_(right_side_expression)
    {
    }
    


    uintptr_t rows()const
    {
        return this->left_side_expression_.rows() + this->right_side_expression_.rows();
    }

    uintptr_t columns()const
    {
        return std::max(this->left_side_expression_.columns(), this->right_side_expression_.columns());
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
        if(row < this->left_side_expression_.rows())
        {
            if(column < this->left_side_expression_.columns())
                return this->left_side_expression_.at(row, column);
            else
                return zero_;
        }
        else
        {
            if(column < this->right_side_expression_.columns())
                return this->right_side_expression_.at(row - this->left_side_expression_.rows(), column);
            else
                return zero_;
        }
    }

    value_type& at_(int64_t row, int64_t column)
    {
        if(row < this->left_side_expression_.rows())
        {
            if(column < this->left_side_expression_.columns())
                return this->left_side_expression_.at(row, column);
            else
                return zero_;
        }
        else
        {
            if(column < this->right_side_expression_.columns())
                return this->right_side_expression_.at(row - this->left_side_expression_.rows(), column);
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

struct is_type_a_matrix< AugmentRowsView<MatrixType1, MatrixType2> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Function to augment two matrices by rows as a view.
 *
 * This function takes two matrix expressions and creates a new matrix view that represents
 * the augmentation of these matrices by rows without copying the data.
 *
 * @tparam MatrixType1 Type of the upper matrix.
 * @tparam MatrixType2 Type of the lower matrix.
 * @param m1 Reference to the upper matrix.
 * @param m2 Reference to the lower matrix.
 * @return An AugmentRowsView object representing the augmented matrix view.
 */
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>

inline auto

augment_by_rows_view(MatrixType1& m1,
                     MatrixType2& m2)
{
    return AugmentRowsView<MatrixType1,MatrixType2>(m1, m2);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_AUGMENT_ROWS_VIEW_HPP_
