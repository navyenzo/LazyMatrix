#ifndef INCLUDE_BORDER_FUNCTOR_VIEW_HPP_
#define INCLUDE_BORDER_FUNCTOR_VIEW_HPP_



//-------------------------------------------------------------------
#include <algorithm>

#include "base_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Given a matrix, when trying to access the matrix outside of its
// bounds, it just returns the value at the matrix's border
//-------------------------------------------------------------------
template<typename MatrixType>

struct RepeatedBorderView : public BaseMatrix< RepeatedBorderView<MatrixType> >
{
    RepeatedBorderView<MatrixType>(MatrixType& expression)
    : expression_(expression)
    {
    }



    int64_t rows()const
    {
        return expression_.rows();
    }

    int64_t columns()const
    {
        return expression_.columns();
    }



    decltype(auto) at(int64_t row, int64_t column)const
    {
        row = std::max(int64_t(0), row);
        row = std::min(this->rows() - 1, row);

        column = std::max(int64_t(0), column);
        column = std::min(this->columns() - 1, column);

        return expression_.at(row, column);
    }

    decltype(auto) at(int64_t row, int64_t column)
    {
        row = std::max(int64_t(0), row);
        row = std::min(this->rows() - 1, row);

        column = std::max(int64_t(0), column);
        column = std::min(this->columns() - 1, column);

        return expression_.at(row, column);
    }



    const MatrixType& get_expression_()const
    {
        return expression_;
    }



private:

    MatrixType& expression_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< RepeatedBorderView<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Given a matrix, when trying to access the matrix outside of its
// bounds, it just returns a user specified constant (defaulted to 0)
//-------------------------------------------------------------------
template<typename MatrixType>

struct ConstantBorderView : public BaseMatrix< ConstantBorderView<MatrixType> >
{
    // Type of value that is stored in the expression
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;



    ConstantBorderView<MatrixType>(MatrixType& expression, const value_type& constant_value = static_cast<value_type>(0))
    : expression_(expression),
      constant_value_(constant_value)
    {
    }



    int64_t rows()const
    {
        return expression_.rows();
    }

    int64_t columns()const
    {
        return expression_.columns();
    }



    const value_type& at(int64_t row, int64_t column)const
    {
        if(row < 0 || row >= this->rows() || column < 0 || column >= this->columns())
            return constant_value_;

        return expression_.at(row, column);
    }

    value_type& at(int64_t row, int64_t column)
    {
        if(row < 0 || row >= this->rows() || column < 0 || column >= this->columns())
            return constant_value_;

        return expression_.at(row, column);
    }



    const MatrixType& get_expression_()const
    {
        return expression_;
    }

    const value_type& get_constant_value()const
    {
        return constant_value_;
    }

    void set_constant_value(const value_type& constant_value)
    {
        constant_value_ = constant_value;
    }



private:

    MatrixType& expression_;

    value_type constant_value_ = static_cast<value_type>(0);
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< ConstantBorderView<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Get the matrix with repeated border values
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

repeated_border_view(MatrixType& m)
{
    return RepeatedBorderView<MatrixType>(m);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Get the matrix with constant border values for outside the range
// access
//-------------------------------------------------------------------
template<typename MatrixType,
         typename DataType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

constant_border_view(MatrixType& m, const DataType& constant_value)
{
    return ConstantBorderView<MatrixType>(m, constant_value);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_BORDER_FUNCTOR_VIEW_HPP_
