//-------------------------------------------------------------------
/**
 * @file selector.hpp
 * @brief Provides functionality for selecting specific rows and/or columns from matrices.
 *
 * This file contains templates for RowSelector, ColumnSelector, and RowAndColumnSelector,
 * enabling users to create views that focus on specific rows, columns, or both from a given
 * matrix. These classes are essential for operations that require manipulation or analysis
 * of selected portions of a matrix.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_SELECTOR_HPP_
#define INCLUDE_SELECTOR_HPP_



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



//-------------------------------------------------------------------
/**
 * @class SingleRowSelector
 * @brief Class for selecting a single row from a matrix.
 *
 * @tparam MatrixType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct SingleRowSelector : public BaseMatrix< SingleRowSelector<MatrixType> >
{
    // Type of value that is stored in the expression
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;

    SingleRowSelector<MatrixType>(const MatrixType& expression, int64_t selected_row)
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



private:

    const MatrixType& expression_;
    int64_t selected_row_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< SingleRowSelector<MatrixType> > : std::true_type
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

struct SingleColumnSelector : public BaseMatrix< SingleColumnSelector<MatrixType> >
{
    // Type of value that is stored in the expression
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;

    SingleColumnSelector<MatrixType>(const MatrixType& expression, int64_t selected_column)
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



private:

    const MatrixType& expression_;
    int64_t selected_column_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< SingleColumnSelector<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class RowSelector
 * @brief Class for selecting specific rows from a matrix.
 *
 * @tparam MatrixType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct RowSelector : public BaseMatrix< RowSelector<MatrixType> >
{
    // Type of value that is stored in the expression
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;

    RowSelector<MatrixType>(const MatrixType& expression,
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



private:

    const MatrixType& expression_;
    std::vector<int64_t> selected_rows_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< RowSelector<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class ColumnSelector
 * @brief Class for selecting specific columns from a matrix.
 *
 * @tparam MatrixType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct ColumnSelector : public BaseMatrix< ColumnSelector<MatrixType> >
{
    // Type of value that is stored in the expression
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;

    ColumnSelector<MatrixType>(const MatrixType& expression,
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



private:

    const MatrixType& expression_;
    std::vector<int64_t> selected_columns_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< ColumnSelector<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class RowAndColumnSelector
 * @brief Class for selecting specific rows and columns from a matrix.
 *
 * @tparam MatrixType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct RowAndColumnSelector : public BaseMatrix< RowAndColumnSelector<MatrixType> >
{
    // Type of value that is stored in the expression
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;

    RowAndColumnSelector<MatrixType>(const MatrixType& expression,
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



private:

    const MatrixType& expression_;
    std::vector<int64_t> selected_rows_;
    std::vector<int64_t> selected_columns_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< RowAndColumnSelector<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Helper functions
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto select_a_single_row(const MatrixType& m, int64_t selected_row)
{
    return SingleRowSelector<MatrixType>(m, selected_row);
}



template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto select_a_single_column(const MatrixType& m, int64_t selected_column)
{
    return SingleColumnSelector<MatrixType>(m, selected_column);
}



template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto select_rows(const MatrixType& m,
                        const std::vector<int64_t>& selected_rows)
{
    return RowSelector<MatrixType>(m, selected_rows);
}



template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto select_columns(const MatrixType& m,
                           const std::vector<int64_t>& selected_columns)
{
    return ColumnSelector<MatrixType>(m, selected_columns);
}



template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto select_rows_and_columns(const MatrixType& m,
                                    const std::vector<int64_t>& selected_rows,
                                    const std::vector<int64_t>& selected_columns)
{
    return RowAndColumnSelector<MatrixType>(m, selected_rows, selected_columns);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_SELECTOR_HPP_
