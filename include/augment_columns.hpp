//-------------------------------------------------------------------
/**
 * @file augment_columns.hpp
 * @brief Provides functionality to augment two matrices column-wise.
 *
 * This file contains the AugmentColumns template class, which is used to 
 * create a new matrix combining two matrices by augmenting their columns. It essentially
 * places the columns of the second matrix to the right of the first matrix, creating
 * a new matrix object.
 *
 * @author Vincenzo Barbato
 * @contact GitHub Project: https://github.com/navyenzo/LazyMatrix.git
 *          LinkedIn: https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_AUGMENT_COLUMNS_HPP_
#define INCLUDE_AUGMENT_COLUMNS_HPP_



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
 * @class AugmentColumns
 * @brief Augments two matrices by columns to create a new matrix.
 *
 * @tparam MatrixType1 Type of the left side matrix in the augmentation.
 * @tparam MatrixType2 Type of the right side matrix in the augmentation.
 */
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2>

struct AugmentColumns : public BaseMatrix< AugmentColumns<MatrixType1, MatrixType2> >
{
    // Type of value that is stored in left side expression
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType1>()(0,0))>::type;

    /**
     * @brief Constructs a new matrix by augmenting the columns of two matrices.
     *
     * @param left_side_expression Reference to the left side matrix.
     * @param right_side_expression Reference to the right side matrix.
     */
    AugmentColumns<MatrixType1, MatrixType2>(const MatrixType1& left_side_expression,
                                             const MatrixType2& right_side_expression)
    : left_side_expression_(left_side_expression),
      right_side_expression_(right_side_expression)
    {
    }



    const MatrixType1& get_left_side_expression()const
    {
        return left_side_expression_;
    }

    const MatrixType2& get_right_side_expression()const
    {
        return right_side_expression_;
    }

    

    uintptr_t rows()const
    {
        return std::max(this->left_side_expression_.rows(), this->right_side_expression_.rows());
    }

    uintptr_t columns()const
    {
        return this->left_side_expression_.columns() + this->right_side_expression_.columns();
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



private:

    const MatrixType1& left_side_expression_;
    const MatrixType2& right_side_expression_;

    value_type zero_ = static_cast<value_type>(0);
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2>

struct is_type_a_matrix< AugmentColumns<MatrixType1, MatrixType2> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Function to augment two matrices by columns.
 *
 * This function takes two matrix expressions and creates a new matrix that represents
 * the augmentation of these matrices by columns.
 *
 * @tparam MatrixType1 Type of the left side matrix.
 * @tparam MatrixType2 Type of the right side matrix.
 * @param m1 Reference to the left side matrix.
 * @param m2 Reference to the right side matrix.
 * @return An AugmentColumns object representing the augmented matrix.
 */
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>

inline auto

augment_by_columns(const MatrixType1& m1,
                   const MatrixType2& m2)
{
    return AugmentColumns<MatrixType1,MatrixType2>(m1, m2);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_AUGMENT_COLUMNS_HPP_
