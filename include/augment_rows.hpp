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
// Given two matrices, augment rows
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2>

struct AugmentRows : public BaseMatrix< AugmentRows<MatrixType1, MatrixType2> >
{
    // Type of value that is stored in left side expression
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType1>()(0,0))>::type;



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
// Augment two matrices by rows
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
