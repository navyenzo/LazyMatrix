//-------------------------------------------------------------------
/**
 * @file selector_view.hpp
 * @brief Provides functionality for creating views that select specific rows and/or columns from matrices.
 *
 * This file contains templates for RowSelectorView, ColumnSelectorView, and RowAndColumnSelectorView,
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
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------
/**
 * @class SingleRowSelectorView
 * @brief Class for creating a view that selects a single row from a matrix.
 *
 * @tparam MatrixType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct SingleRowSelectorView : public BaseMatrix< SingleRowSelectorView<MatrixType> >
{
    // Type of value that is stored in the expression
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;

    SingleRowSelectorView<MatrixType>(MatrixType& expression, int64_t selected_row)
    : expression_(expression),
      selected_row_(selected_row_)
    {
    }
    


    uintptr_t rows()const
    {
        return uintptr_t(1);
    }

    uintptr_t columns()const
    {
        return expression_.columns();
    }



    const MatrixType& get_expression()const
    {
        return expression_;
    }



    const value_type& at_(int64_t row, int64_t column)const
    {
        return expression_.circ_at(selected_row_, column);
    }

    value_type& at_(int64_t row, int64_t column)
    {
        return expression_.circ_at(selected_row_, column);
    }



private:

    MatrixType& expression_;
    int64_t selected_row_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< SingleRowSelectorView<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class SingleColumnSelector
 * @brief Class for selecting a single column from a matrix.
 *
 * @tparam MatrixType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct SingleColumnSelectorView : public BaseMatrix< SingleColumnSelectorView<MatrixType> >
{
    // Type of value that is stored in the expression
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;

    SingleColumnSelectorView<MatrixType>(MatrixType& expression, int64_t selected_column)
    : expression_(expression),
      selected_column_(selected_column_)
    {
    }
    


    uintptr_t rows()const
    {
        return expression_.rows();
    }

    uintptr_t columns()const
    {
        return uintptr_t(1);
    }



    const MatrixType& get_expression()const
    {
        return expression_;
    }



    const value_type& at_(int64_t row, int64_t column)const
    {
        return expression_.circ_at(row, selected_column_);
    }

    value_type& at_(int64_t row, int64_t column)
    {
        return expression_.circ_at(row, selected_column_);
    }



private:

    MatrixType& expression_;
    int64_t selected_column_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< SingleColumnSelectorView<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class RowSelectorView
 * @brief Class for creating a view that selects specific rows from a matrix.
 *
 * @tparam MatrixType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct RowSelectorView : public BaseMatrix< RowSelectorView<MatrixType> >
{
    // Type of value that is stored in the expression
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type>::type;

    

    RowSelectorView<MatrixType>(MatrixType& expression,
                                const std::vector<int64_t>& selected_rows)
    : expression_(expression),
      selected_rows_(selected_rows)
    {
    }
    


    uintptr_t rows()const
    {
        if(expression_.rows() == 0)
            return 0;

        return selected_rows_.size();
    }

    uintptr_t columns()const
    {
        return expression_.columns();
    }



    const MatrixType& get_expression()const
    {
        return expression_;
    }
    


    const value_type& at_(int64_t row, int64_t column)const
    {
        return expression_.circ_at(selected_rows_[row], column);
    }

    value_type& at_(int64_t row, int64_t column)
    {
        return expression_.circ_at(selected_rows_[row], column);
    }



private:

    MatrixType& expression_;
    std::vector<int64_t> selected_rows_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< RowSelectorView<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class ColumnSelectorView
 * @brief Class for creating a view that selects specific columns from a matrix.
 *
 * @tparam MatrixType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct ColumnSelectorView : public BaseMatrix< ColumnSelectorView<MatrixType> >
{
    // Type of value that is stored in the expression
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;

    ColumnSelectorView<MatrixType>(MatrixType& expression,
                                   const std::vector<int64_t>& selected_columns)
    : expression_(expression),
      selected_columns_(selected_columns)
    {
    }
    


    uintptr_t rows()const
    {
        return expression_.rows();
    }

    uintptr_t columns()const
    {
        if(expression_.columns() == 0)
            return 0;

        return selected_columns_.size();
    }



    const MatrixType& get_expression()const
    {
        return expression_;
    }
    


    const value_type& at_(int64_t row, int64_t column)const
    {
        return expression_.circ_at(row, selected_columns_[column]);
    }

    value_type& at_(int64_t row, int64_t column)
    {
        return expression_.circ_at(row, selected_columns_[column]);
    }



private:

    MatrixType& expression_;
    std::vector<int64_t> selected_columns_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< ColumnSelectorView<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class RowAndColumnSelectorView
 * @brief Class for creating a view that selects specific rows and columns from a matrix.
 *
 * @tparam MatrixType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct RowAndColumnSelectorView : public BaseMatrix< RowAndColumnSelectorView<MatrixType> >
{
    // Type of value that is stored in the expression
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;

    RowAndColumnSelectorView<MatrixType>(MatrixType& expression,
                                         const std::vector<int64_t>& selected_rows,
                                         const std::vector<int64_t>& selected_columns)
    : expression_(expression),
      selected_rows_(selected_rows),
      selected_columns_(selected_columns)
    {
    }
    


    uintptr_t rows()const
    {
        if(expression_.rows() == 0)
            return 0;

        return selected_rows_.size();
    }

    uintptr_t columns()const
    {
        if(expression_.columns() == 0)
            return 0;

        return selected_columns_.size();
    }



    const MatrixType& get_expression()const
    {
        return expression_;
    }
    


    const value_type& at_(int64_t row, int64_t column)const
    {
        return expression_.circ_at(selected_rows_[row], selected_columns_[column]);
    }

    value_type& at_(int64_t row, int64_t column)
    {
        return expression_.circ_at(selected_rows_[row], selected_columns_[column]);
    }



private:

    MatrixType& expression_;
    std::vector<int64_t> selected_rows_;
    std::vector<int64_t> selected_columns_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< RowAndColumnSelectorView<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Helper functions
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto select_a_single_row_view(MatrixType& m, int64_t selected_row)
{
    return SingleRowSelectorView<MatrixType>(m, selected_row);
}



template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto select_a_single_column_view(MatrixType& m, int64_t selected_column)
{
    return SingleColumnSelectorView<MatrixType>(m, selected_column);
}



template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto select_rows_view(MatrixType& m,
                             const std::vector<int64_t>& selected_rows)
{
    return RowSelectorView<MatrixType>(m, selected_rows);
}



template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto select_columns_view(MatrixType& m,
                                const std::vector<int64_t>& selected_columns)
{
    return ColumnSelectorView<MatrixType>(m, selected_columns);
}



template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto select_rows_and_columns_view(MatrixType& m,
                                         const std::vector<int64_t>& selected_rows,
                                         const std::vector<int64_t>& selected_columns)
{
    return RowAndColumnSelectorView<MatrixType>(m, selected_rows, selected_columns);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_SELECTOR_VIEW_HPP_
