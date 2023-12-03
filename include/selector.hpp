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
 * @link https://www.linkedin.com/in/vincenzobarbato/
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
 * @class RowSelector
 * @brief Class for selecting specific rows from a matrix.
 *
 * @tparam MatrixType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct RowSelector : public BaseMatrix< RowSelector<MatrixType> >
{
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

    decltype(auto) at_(int64_t row, int64_t column)const
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
    


    decltype(auto) at_(int64_t row, int64_t column)const
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
    
    

    decltype(auto) at_(int64_t row, int64_t column)const
    {
        return expression_.circ_at(selected_rows_[row], selected_columns_[column]);
    }

    decltype(auto) at_(int64_t row, int64_t column)
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
