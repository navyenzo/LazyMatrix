//-------------------------------------------------------------------
/**
 * @file element_by_element_unary_expression.hpp
 * @brief Provides element-by-element unary operations for matrices.
 *
 * This file is part of the LazyMatrix library. It includes a template structure and functions to perform 
 * element-by-element unary operations such as negation, sign, absolute value, square root, and exponential functions on matrices. 
 * These operations are essential for various matrix-based calculations and transformations.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_ELEMENT_BY_ELEMENT_UNARY_EXPRESSION_HPP_
#define INCLUDE_ELEMENT_BY_ELEMENT_UNARY_EXPRESSION_HPP_



//-------------------------------------------------------------------
#include <algorithm>
#include <functional>
#include <cmath>
#include <complex>
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
 * @brief A structure to represent element-by-element unary operations on a matrix.
 *
 * This structure allows for operations like negation, absolute value, square root, etc., 
 * to be applied element-wise on a matrix. It uses a functional approach to define 
 * the operation that should be applied to each element of the matrix.
 *
 * @tparam ReferenceType Type of the matrix.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

struct ElementByElementUnaryExpression : public BaseMatrix< ElementByElementUnaryExpression<ReferenceType> >
{
    // Type of value that is stored in the matrix
    using value_type = typename ReferenceType::value_type;

    // The operation function type
    using operation_type = std::function<value_type(value_type)>;

    friend class BaseMatrix< ElementByElementUnaryExpression<ReferenceType> >;

    /**
     * @brief Construct a new Element By Element Unary Expression< Reference Type> object
     * 
     * @param expression The input matrix expression
     * @param operation_function The function used to calculate the output
     */
    ElementByElementUnaryExpression(ReferenceType expression,
                                    const operation_type& operation_function)
    {
        set_expression(expression);
        set_operation_function(operation_function);
    }

    /**
     * @brief Sets the reference to the input matrix expression
     * @param left_side_expression Reference to the input matrix.
     */
    void set_expression(ReferenceType expression)
    {
        expression_ = expression;
    }

    /**
     * @brief Set the operation function object
     * 
     * @param operation_function The function used to calculate the output
     */
    void set_operation_function(const operation_type& operation_function)
    {
        operation_function_ = operation_function;
    }

    /**
     * @brief Returns the number of rows in the resulting matrix.
     */
    uintptr_t rows()const
    {
        return expression_.rows();
    }

    /**
     * @brief Returns the number of columns in the resulting matrix.
     */
    uintptr_t columns()const
    {
        return expression_.columns();
    }



private: // Private functions

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A copy of the value of the element at the specified position.
     */
    value_type const_at_(int row, int column)const
    {
        return static_cast<value_type>(operation_function_(this->expression_.at(row,column)));
    }

    /**
     * @brief Reference access doesn't make sense for this type of matrix operation
     *        expression, so we just return a dummy value.
     * @param row Row index.
     * @param column Column index.
     * @return A reference to a dummy value since reference access doesn't make sense here.
     */
    value_type& non_const_at_(int row, int column)
    {
        return DummyValueHolder<value_type>::zero;
    }



private: // Private variables

    ReferenceType expression_;
    operation_type operation_function_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename ReferenceType>

struct is_type_a_matrix< ElementByElementUnaryExpression<ReferenceType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Negation (-m).
 * @tparam ReferenceType Type of the input matrix expression.
 * @param m Shared reference to the input matrix.
 * @return Returns a SharedMatrixRef to the negation of the
 *         input matrix expression (-m)
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline auto

operator-(ReferenceType m)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<ReferenceType>()(0,0))>::type>::type;

    auto view = std::make_shared<ElementByElementUnaryExpression<ReferenceType>>(m, std::negate<value_type>());

    return SharedMatrixRef<ElementByElementUnaryExpression<ReferenceType>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Sign(m).
 * @tparam ReferenceType Type of the input matrix expression.
 * @param m Shared reference to the input matrix.
 * @return Returns a SharedMatrixRef to the sign of each element
 *         from the input expression sign(m).
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline auto

sign(ReferenceType m)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<ReferenceType>()(0,0))>::type>::type;

    auto view = std::make_shared<ElementByElementUnaryExpression<ReferenceType>>
                                                      (
                                                       m,
                                                       [](value_type number)
                                                       {
                                                            value_type zero = static_cast<value_type>(0);
                                                            return static_cast<decltype(number)>( (zero < number) - (number < zero) );
                                                       }
                                                      );

    return SharedMatrixRef<ElementByElementUnaryExpression<ReferenceType>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Absolute value abs(m).
 * @tparam ReferenceType Type of the input matrix expression.
 * @param m Shared reference to the input matrix.
 * @return Returns a SharedMatrixRef to the absolute value of
 *         each element of the input matrix expression
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline auto

abs(ReferenceType m)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<ReferenceType>()(0,0))>::type>::type;

    auto view = std::make_shared<ElementByElementUnaryExpression<ReferenceType>>(m, [](const value_type& number){ return std::abs(number); });

    return SharedMatrixRef<ElementByElementUnaryExpression<ReferenceType>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Square Root of each element of input matrix sqrt(m)
 * @tparam ReferenceType Type of the input matrix expression.
 * @param m Shared reference to the input matrix.
 * @return Returns a SharedMatrixRef to the square root of each
 *         element of the input matrix expression sqrt(m).
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline auto

sqrt(ReferenceType m)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<ReferenceType>()(0,0))>::type>::type;

    auto view = std::make_shared<ElementByElementUnaryExpression<ReferenceType>>(m, [](const value_type& number){ return std::sqrt(number); });

    return SharedMatrixRef<ElementByElementUnaryExpression<ReferenceType>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Exponential of each element of input matrix exp(m).
 * @tparam ReferenceType Type of the input matrix expression.
 * @param m Shared reference to the input matrix.
 * @return Returns a SharedMatrixRef to the exponential of each
 *         element of the input matrix expression exp(m).
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline auto

exp(ReferenceType m)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<ReferenceType>()(0,0))>::type>::type;

    auto view = std::make_shared<ElementByElementUnaryExpression<ReferenceType>>(m, [](const value_type& number){ return std::exp(number); });

    return SharedMatrixRef<ElementByElementUnaryExpression<ReferenceType>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief 2^x of each element of input matrix.
 * @tparam ReferenceType Type of the input matrix expression.
 * @param m Shared reference to the input matrix.
 * @return Returns a SharedMatrixRef to the value of 2^x of
 *         each element of the input matrix expression.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline auto

exp2(ReferenceType m)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<ReferenceType>()(0,0))>::type>::type;

    auto view = std::make_shared<ElementByElementUnaryExpression<ReferenceType>>(m, [](const value_type& number){ return std::exp2(number); });

    return SharedMatrixRef<ElementByElementUnaryExpression<ReferenceType>>(view);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_ELEMENT_BY_ELEMENT_UNARY_EXPRESSION_HPP_
