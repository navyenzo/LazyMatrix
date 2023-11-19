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
// Given two matrices, augment columns
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2>

struct AugmentColumns : public BaseMatrix< AugmentColumns<MatrixType1, MatrixType2> >
{
    // Type of value that is stored in left side expression
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType1>()(0,0))>::type;



    AugmentColumns<MatrixType1, MatrixType2>(const MatrixType1& left_side_expression,
                                             const MatrixType2& right_side_expression)
    : left_side_expression_(left_side_expression),
      right_side_expression_(right_side_expression)
    {
    }



    int64_t rows()const
    {
        return std::max(this->left_side_expression_.rows(), this->right_side_expression_.rows());
    }

    int64_t columns()const
    {
        return this->left_side_expression_.columns() + this->right_side_expression_.columns();
    }



    const value_type& at(int64_t row, int64_t column)const
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



    const MatrixType1& get_left_side_expression()const
    {
        return left_side_expression_;
    }

    const MatrixType2& get_right_side_expression()const
    {
        return right_side_expression_;
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
// Augment two matrices by rows
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
