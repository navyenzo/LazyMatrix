//-------------------------------------------------------------------
/**
 * @file selector_view.hpp
 * @brief Provides functionality for creating views that select specific rows and/or columns from matrices.
 *
 * This file contains templates for MultipleVectorSelectorView, ColumnSelectorView, and RowAndColumnSelectorView,
 * which enable users to create views focusing on specific rows, columns, or both from a given matrix.
 * Unlike the non-view selectors, these classes allow modification of the original matrix through the view.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_SELECTOR_VIEW_HPP_
#define INCLUDE_SELECTOR_VIEW_HPP_



//-------------------------------------------------------------------
#include <type_traits>
#include <vector>
#include <unordered_map>

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
 * @class SingleVectorSelectorView
 * @brief Class for creating a view that selects a single vector from a matrix (row or column).
 *
 * @tparam ReferenceType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

class SingleVectorSelectorView : public BaseMatrix< SingleVectorSelectorView<ReferenceType>,
                                                    has_non_const_access<ReferenceType>::value>
{
public:

    // Type of value that is stored in the expression
    using value_type = typename ReferenceType::value_type;

    friend class BaseMatrix<SingleVectorSelectorView<ReferenceType>,
                            has_non_const_access<ReferenceType>::value>;

    /**
     * @brief Construct a new Single Vector Selector View< Reference Type> object.
     * 
     * @param expression The input matrix expression.
     * @param selected_vector  Index of row to select.
     * @param are_we_selecting_a_row Whether we need to select a row or column.
     */
    SingleVectorSelectorView(ReferenceType expression, int64_t selected_vector, bool are_we_selecting_a_row)
    {
        set_expression(expression);
        set_selected_vector(selected_vector);
        set_are_we_selecting_a_row(are_we_selecting_a_row);
    }

    /**
     * @brief Sets the reference to the matrix expression
     * @param expression Reference to the matrix.
     */
    void set_expression(ReferenceType expression)
    {
        expression_ = expression;
    }

    /**
     * @brief Set the selected row object
     * 
     * @param selected_vector 
     */
    void set_selected_vector(int64_t selected_vector)
    {
        selected_vector_ = selected_vector;
    }

    /**
     * @brief Set the are we selecting a row or column
     * 
     * @param are_we_selecting_a_row 
     */
    void set_are_we_selecting_a_row(bool are_we_selecting_a_row)
    {
        are_we_selecting_a_row_ = are_we_selecting_a_row;
    }

    /**
     * @brief Returns the number of rows Of the resulting matrix.
     */
    uintptr_t rows()const
    {
        if(are_we_selecting_a_row_)
            return uintptr_t(1);
        else
            return expression_.rows();
    }

    /**
     * @brief Returns the total number of columns of the resulting matrix.
     */
    uintptr_t columns()const
    {
        if(are_we_selecting_a_row_)
            return expression_.columns();
        else
            return uintptr_t(1);
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
        if(are_we_selecting_a_row_)
            return expression_.circ_at(selected_vector_, column);
        else
            return expression_.circ_at(row, selected_vector_);
    }
    
    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A reference to the element at the specified position.
     */
    std::enable_if_t<has_non_const_access<ReferenceType>::value, value_type&>
    non_const_at_(int64_t row, int64_t column)
    {
        if(are_we_selecting_a_row_)
            return expression_.circ_at(selected_vector_, column);
        else
            return expression_.circ_at(row, selected_vector_);
    }



private: // Private variables

    ReferenceType expression_;
    int64_t selected_vector_ = 0;
    bool are_we_selecting_a_row_ = true;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename ReferenceType>

struct is_type_a_matrix< SingleVectorSelectorView<ReferenceType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class MultipleVectorSelectorView
 * @brief Class for creating a view that selects multiple rows or columns
 *        from an input matrix expression.
 *
 * @tparam ReferenceType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

class MultipleVectorSelectorView : public BaseMatrix<MultipleVectorSelectorView<ReferenceType>,
                                                     has_non_const_access<ReferenceType>::value>
{
public:

    // Type of value that is stored in the expression
    using value_type = typename ReferenceType::value_type;

    friend class BaseMatrix<MultipleVectorSelectorView<ReferenceType>,
                            has_non_const_access<ReferenceType>::value>;

    /**
     * @brief Construct a new Multiple Vector Selector View< Reference Type> object
     * 
     * @param expression The input matrix expression.
     * @param selected_vectors The vector containing the indeces of the vectors to select.
     * @param are_we_selecting_rows Whether we need to select rows or columns.
     */
    MultipleVectorSelectorView(ReferenceType expression,
                               const std::vector<int64_t>& selected_vectors,
                               bool are_we_selecting_rows)
    {
        set_expression(expression);
        set_selected_vectors(selected_vectors);
        set_are_we_selecting_rows(are_we_selecting_rows);
    }

    /**
     * @brief Sets the reference to the matrix expression
     * @param expression Reference to the matrix.
     */
    void set_expression(ReferenceType expression)
    {
        expression_ = expression;
    }

    /**
     * @brief Set the selected vectors
     * 
     * @param selected_vectors vector containing indeces of vectors to select
     */
    void set_selected_vectors(const std::vector<int64_t>& selected_vectors)
    {
        selected_vectors_ = selected_vectors;
    }

    /**
     * @brief Set the are we selecting rows or columns
     * 
     * @param are_we_selecting_rows 
     */
    void set_are_we_selecting_rows(bool are_we_selecting_rows)
    {
        are_we_selecting_rows_ = are_we_selecting_rows_;
    }

    /**
     * @brief Returns the number of rows Of the resulting matrix.
     */
    uintptr_t rows()const
    {
        if(are_we_selecting_rows_)
            return selected_vectors_.size();
        else
            return expression_.rows();
    }

    /**
     * @brief Returns the total number of columns of the resulting matrix.
     */
    uintptr_t columns()const
    {
        if(are_we_selecting_rows_)
            return expression_.columns();
        else
            return selected_vectors_.size();
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
        if(are_we_selecting_rows_)
            return expression_.circ_at(selected_vectors_[row], column);
        else
            return expression_.circ_at(row, selected_vectors_[column]);
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A reference to the element at the specified position.
     */
    std::enable_if_t<has_non_const_access<ReferenceType>::value, value_type&>
    non_const_at_(int64_t row, int64_t column)
    {
        if(are_we_selecting_rows_)
            return expression_.circ_at(selected_vectors_[row], column);
        else
            return expression_.circ_at(row, selected_vectors_[column]);
    }



private: // Private variables

    ReferenceType expression_;
    std::vector<int64_t> selected_vectors_;
    bool are_we_selecting_rows_ = true;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename ReferenceType>

struct is_type_a_matrix< MultipleVectorSelectorView<ReferenceType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class RowAndColumnSelectorView
 * @brief Class for creating a view that selects specific rows and columns from a matrix.
 *
 * @tparam ReferenceType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

class RowAndColumnSelectorView : public BaseMatrix<RowAndColumnSelectorView<ReferenceType>,
                                                   has_non_const_access<ReferenceType>::value>
{
public:

    // Type of value that is stored in the expression
    using value_type = typename ReferenceType::value_type;

    friend class BaseMatrix<RowAndColumnSelectorView<ReferenceType>,
                            has_non_const_access<ReferenceType>::value>;

    /**
     * @brief Construct a new Row And Column Selector View< Reference Type> object.
     * 
     * @param expression The input matrix expression.
     * @param selected_rows The vector containing the indeces of the selected rows.
     * @param selected_columns The vector containing the indeces of the selected columns.
     */
    RowAndColumnSelectorView(ReferenceType expression,
                             const std::vector<int64_t>& selected_rows,
                             const std::vector<int64_t>& selected_columns)
    {
        set_expression(expression);
        set_selected_rows(selected_rows);
        set_selected_columns(selected_columns);
    }

    /**
     * @brief Sets the reference to the matrix expression
     * @param expression Reference to the matrix.
     */
    void set_expression(ReferenceType expression)
    {
        expression_ = expression;
    }

    /**
     * @brief Set the selected rows
     * 
     * @param selected_rows vector containing indeces of rows to select
     */
    void set_selected_rows(const std::vector<int64_t>& selected_rows)
    {
        selected_rows_ = selected_rows;
    }

    /**
     * @brief Set the selected columns
     * 
     * @param selected_columns vector containing indeces of columnss to select
     */
    void set_selected_columns(const std::vector<int64_t>& selected_columns)
    {
        selected_columns_ = selected_columns;
    }

    /**
     * @brief Returns the number of rows Of the resulting matrix.
     */
    uintptr_t rows()const
    {
        if(expression_.rows() == 0)
            return expression_.rows();
        else
            return selected_rows_.size();
    }

    /**
     * @brief Returns the total number of columns of the resulting matrix.
     */
    uintptr_t columns()const
    {
        if(expression_.columns() == 0)
            return expression_.columns();
        else
            return selected_rows_.size();
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
        return expression_.circ_at(selected_rows_[row], selected_columns_[column]);
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A reference to the element at the specified position.
     */
    std::enable_if_t<has_non_const_access<ReferenceType>::value, value_type&>
    non_const_at_(int64_t row, int64_t column)
    {
        return expression_.circ_at(selected_rows_[row], selected_columns_[column]);
    }



private: // Private variables

    ReferenceType expression_;
    std::vector<int64_t> selected_rows_;
    std::vector<int64_t> selected_columns_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename ReferenceType>

struct is_type_a_matrix< RowAndColumnSelectorView<ReferenceType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Helper functions
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Creates a view with a selected row or column vector from an input
 *        matrix expression.
 * @tparam ReferenceType Type of the input matrix expression.
 * @param m Shared reference to the input matrix expression
 * @param selected_vector Index of vector we are selecting.
 * @param are_we_selecting_a_row Flag to indicate whether to select a row.
 * @return A SharedMatrixRef to the SingleVectorSelectorView matrix object.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline auto

create_single_vector_selector_view(ReferenceType m,
                                   int64_t selected_vector,
                                   bool are_we_selecting_a_row)
{
    auto view = std::make_shared<SingleVectorSelectorView<ReferenceType>>(m, selected_vector, are_we_selecting_a_row);

    if constexpr (has_non_const_access<ReferenceType>::value)
    {
        return SharedMatrixRef<SingleVectorSelectorView<ReferenceType>>(view);
    }
    else
    {
        return ConstSharedMatrixRef<SingleVectorSelectorView<ReferenceType>>(view);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Creates a view with a multiple selected rows or columns
 *        from an input matrix expression.
 * @tparam ReferenceType Type of the input matrix expression.
 * @param m Shared reference to the input matrix expression
 * @param selected_vectors Vector containing indeces of selected rows or columns.
 * @param are_we_selecting_a_row Flag to indicate whether to select rows or columns.
 * @return A SharedMatrixRef to the MultipleVectorSelectorView matrix object.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline auto

create_multiple_vector_selector_view(ReferenceType m,
                                     const std::vector<int64_t>& selected_vectors,
                                     bool are_we_selecting_rows)
{
    auto view = std::make_shared<MultipleVectorSelectorView<ReferenceType>>(m, selected_vectors, are_we_selecting_rows);

    if constexpr (has_non_const_access<ReferenceType>::value)
    {
        return SharedMatrixRef<MultipleVectorSelectorView<ReferenceType>>(view);
    }
    else
    {
        return ConstSharedMatrixRef<MultipleVectorSelectorView<ReferenceType>>(view);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Creates a view with multiple selected rows and multiple
 *        selected columns from an input matrix expression.
 * @tparam ReferenceType Type of the input matrix expression.
 * @param m The input matrix expression.
 * @param selected_rows The vector containing the indeces of the selected rows.
 * @param selected_columns The vector containing the indeces of the selected columns.
 * @return A SharedMatrixRef to the RowAndColumnSelectorView matrix object.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline auto

create_multiple_vector_selector_view(ReferenceType m,
                                     const std::vector<int64_t>& selected_rows,
                                     const std::vector<int64_t>& selected_columns)
{
    auto view = std::make_shared<RowAndColumnSelectorView<ReferenceType>>(m, selected_rows, selected_columns);

    if constexpr (has_non_const_access<ReferenceType>::value)
    {
        return SharedMatrixRef<RowAndColumnSelectorView<ReferenceType>>(view);
    }
    else
    {
        return ConstSharedMatrixRef<RowAndColumnSelectorView<ReferenceType>>(view);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_SELECTOR_VIEW_HPP_
