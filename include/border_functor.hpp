#ifndef INCLUDE_BORDER_FUNCTOR_HPP_
#define INCLUDE_BORDER_FUNCTOR_HPP_



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

struct RepeatedBorder : public BaseMatrix< RepeatedBorder<MatrixType> >
{
    RepeatedBorder<MatrixType>(const MatrixType& expression)
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



    const MatrixType& get_expression_()const
    {
        return expression_;
    }



private:

    const MatrixType& expression_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< RepeatedBorder<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Given a matrix, when trying to access the matrix outside of its
// bounds, it just returns a user specified constant (defaulted to 0)
//-------------------------------------------------------------------
template<typename MatrixType>

struct ConstantBorder : public BaseMatrix< ConstantBorder<MatrixType> >
{
    // Type of value that is stored in the expression
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;



    ConstantBorder<MatrixType>(const MatrixType& expression, const value_type& constant_value = static_cast<value_type>(0))
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

    const MatrixType& expression_;

    value_type constant_value_ = static_cast<value_type>(0);
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< ConstantBorder<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Get the matrix with repeated border values
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

repeated_border(const MatrixType& m)
{
    return RepeatedBorder<MatrixType>(m);
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

constant_border(const MatrixType& m, const DataType& constant_value)
{
    return ConstantBorder<MatrixType>(m, constant_value);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_BORDER_FUNCTOR_HPP_
