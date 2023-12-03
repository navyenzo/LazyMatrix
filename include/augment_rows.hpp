//-------------------------------------------------------------------
/**
 * @file augment_rows.hpp
 * @brief Provides functionality to augment two matrices row-wise.
 *
 * This file contains the AugmentRows template class, which is used to 
 * create a new matrix combining two matrices by augmenting their rows. 
 * This class essentially stacks the rows of the second matrix below the 
 * first matrix, creating a new matrix without copying the data.
 *
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_AUGMENT_ROWS_HPP_
#define INCLUDE_AUGMENT_ROWS_HPP_



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
 * @class AugmentRows
 * @brief Augments two matrices by rows to create a new matrix.
 *
 * @tparam MatrixType1 Type of the upper matrix in the augmentation.
 * @tparam MatrixType2 Type of the lower matrix in the augmentation.
 */
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2>

struct AugmentRows : public BaseMatrix< AugmentRows<MatrixType1, MatrixType2> >
{
    // Type of value that is stored in left side expression
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType1>()(0,0))>::type;

    /**
     * @brief Constructs a new matrix by augmenting the rows of two matrices.
     *
     * @param left_side_expression Reference to the upper matrix.
     * @param right_side_expression Reference to the lower matrix.
     */
    AugmentRows<MatrixType1, MatrixType2>(const MatrixType1& left_side_expression,
                                          const MatrixType2& right_side_expression)
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

struct is_type_a_matrix< AugmentRows<MatrixType1, MatrixType2> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Function to augment two matrices by rows.
 *
 * This function takes two matrix expressions and creates a new matrix that represents
 * the augmentation of these matrices by rows.
 *
 * @tparam MatrixType1 Type of the upper matrix.
 * @tparam MatrixType2 Type of the lower matrix.
 * @param m1 Reference to the upper matrix.
 * @param m2 Reference to the lower matrix.
 * @return An AugmentRows object representing the augmented matrix.
 */
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>

inline auto

augment_by_rows(const MatrixType1& m1,
                const MatrixType2& m2)
{
    return AugmentRows<MatrixType1,MatrixType2>(m1, m2);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_AUGMENT_ROWS_HPP_
