//-------------------------------------------------------------------
/**
 * @file transpose_view.hpp
 * @brief Provides the TransposeView class for transposing matrix expressions in LazyMatrix.
 *
 * This file defines the TransposeView class, which creates a view of a matrix expression
 * where the rows and columns are swapped (transposed). The view is not a copy of the data;
 * it references the original matrix, allowing changes to be reflected in the original matrix.
 * This class is particularly useful for operations that require a transpose without the cost
 * of physically rearranging the data.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_TRANSPOSE_VIEW_HPP_
#define INCLUDE_TRANSPOSE_VIEW_HPP_



//-------------------------------------------------------------------
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
 * @class Transpose
 * @brief A view class that provides a transposed view of a matrix.
 *
 * Transpose presents a transposed view of the input matrix expression.
 * It swaps rows and columns, allowing for operations as if the original
 * matrix were transposed. This class does not create a copy of the
 * input matrix, just a view of it.
 *
 * @tparam ReferenceType The type of the underlying matrix expression.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

class Transpose : public BaseMatrix<Transpose<ReferenceType>,
                                    has_non_const_access<ReferenceType>::value>
{
public:

    // Type of value that is stored in the matrix
    using value_type = typename ReferenceType::value_type;

    friend class BaseMatrix<Transpose<ReferenceType>,
                            has_non_const_access<ReferenceType>::value>;

    /**
     * @brief Construct a new Transpose View< Reference Type> object
     * 
     * @param expression The input matrix expression.
     */
    explicit Transpose(ReferenceType expression)
    {
        set_expression(expression);
    }
    
    /**
     * @brief Sets the reference to the input matrix expression
     * @param expression Reference to the input matrix.
     */
    void set_expression(ReferenceType expression)
    {
        expression_ = expression;
    }

    /**
     * @brief Returns the number of rows of the resulting matrix.
     */
    uintptr_t rows()const
    {
        return this->expression_.columns();
    }

    /**
     * @brief Returns the number of columns of the resulting matrix.
     */
    uintptr_t columns()const
    {
        return this->expression_.rows();
    }

    // Functions used to handle row and column header names
    std::string get_row_header(int64_t row_index) const
    {
        return expression_.get_column_header(row_index);
    }
    
    std::string get_column_header(int64_t column_index) const
    {
        return expression_.get_row_header(column_index);
    }

    void set_row_header(int64_t row_index, const std::string& row_header) const
    {
        expression_.set_column_header(row_index, row_header);
    }

    void set_column_header(int64_t column_index, const std::string& column_header) const
    {
        expression_.set_row_header(column_index, column_header);
    }



private: // Private functions

    /**
     * @brief Dummy "resize" function needed for the matrix interface, but
     *        here it doesn't do anything
     * 
     * @param rows 
     * @param columns 
     * @return std::error_code 
     */
    std::error_code resize_(uintptr_t rows, uintptr_t columns)
    {
        return std::error_code();
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A copy of the value of the element at the specified position.
     */
    value_type const_at_(int64_t row, int64_t column)const
    {
        return expression_(column, row);
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A reference to the element at the specified position.
     */
    template<typename T = ReferenceType>
    std::enable_if_t<has_non_const_access<T>::value, value_type&>
    non_const_at_(int64_t row, int64_t column)
    {
        return expression_(column, row);
    }



private: // Private variables

    ReferenceType expression_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename ReferenceType>

struct is_type_a_matrix< Transpose<ReferenceType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Returns the transpose of the input matrix expression.
 * @tparam ReferenceType Type of the input matrix expression.
 * @param m Shared reference to the input matrix expression
 * @return A SharedMatrixRef to the transpose view of the input
 *         matrix expression
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline auto transpose(ReferenceType m)
{
    auto view = std::make_shared<Transpose<ReferenceType>>(m);

    if constexpr (has_non_const_access<ReferenceType>::value)
    {
        return SharedMatrixRef<Transpose<ReferenceType>>(view);
    }
    else
    {
        return ConstSharedMatrixRef<Transpose<ReferenceType>>(view);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_TRANSPOSE_VIEW_HPP_
