//-------------------------------------------------------------------
/**
 * @file border_functor.hpp
 * @brief Provides functionality to handle matrix border cases.
 *
 * This file contains the RepeatedBorder and ConstantBorder template classes,
 * which are used to handle border cases when accessing elements outside the
 * bounds of a matrix. RepeatedBorder returns the value at the matrix's border,
 * while ConstantBorder returns a user-specified constant value.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



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
/**
 * @class RepeatedBorder
 * @brief Handles border cases by repeating border values.
 *
 * @tparam MatrixType Type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct RepeatedBorder : public BaseMatrix< RepeatedBorder<MatrixType> >
{
    RepeatedBorder<MatrixType>(const MatrixType& expression)
    : expression_(expression)
    {
    }



    uintptr_t rows()const
    {
        return expression_.rows();
    }

    uintptr_t columns()const
    {
        return expression_.columns();
    }



    const MatrixType& get_expression_()const
    {
        return expression_;
    }


    
    decltype(auto) at_(int64_t row, int64_t column)const
    {
        row = std::max(int64_t(0), row);
        row = std::min(this->rows() - 1, row);

        column = std::max(int64_t(0), column);
        column = std::min(this->columns() - 1, column);

        return expression_(row, column);
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
/**
 * @class ConstantBorder
 * @brief Handles border cases by returning a constant value.
 *
 * @tparam MatrixType Type of the matrix expression.
 */
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



    uintptr_t rows()const
    {
        return expression_.rows();
    }

    uintptr_t columns()const
    {
        return expression_.columns();
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
    


    const value_type& at_(int64_t row, int64_t column)const
    {
        if(row < 0 || row >= this->rows() || column < 0 || column >= this->columns())
            return constant_value_;

        return expression_(row, column);
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
