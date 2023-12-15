//-------------------------------------------------------------------
/**
 * @file sorting.hpp
 * @brief Provides functionality to create a view of a matrix with sorted rows or columns.
 *
 * This file contains the function for generating a vector of sorted indices based on 
 * the values in a specified row or column of a matrix. This functionality is key in 
 * creating views of matrices where the rows or columns are sorted, without altering 
 * the original matrix data. The generated index vector is used for accessing the matrix 
 * elements in a sorted order.
 *
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @see https://github.com/navyenzo/LazyMatrix.git
 * @namespace LazyMatrix
 */
//-------------------------------------------------------------------



#ifndef SORTED_VIEW_HPP
#define SORTED_VIEW_HPP



//-------------------------------------------------------------------
#include <vector>
#include <algorithm>
#include <numeric>

#include "base_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Modifies a given vector to contain sorted indices based on a matrix row or column.
 *
 * This function takes an existing vector and resizes it to match the number of elements
 * in the specified row or column of the matrix. It then fills this vector with indices
 * corresponding to these elements and sorts the indices based on the values in the row 
 * or column. The sorting is performed in ascending order.
 *
 * @tparam MatrixType The type of the matrix expression.
 * @param matrix The matrix expression.
 * @param index The index of the row or column to be sorted.
 * @param sort_by_row Boolean flag to indicate row-wise (true) or column-wise (false) sorting.
 * @param sorted_indices A reference to a vector where sorted indices will be stored.
 */
//-------------------------------------------------------------------
 template<typename MatrixType,
          std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline void get_sorted_indices(const MatrixType& matrix,
                               int64_t index, 
                               bool sort_by_row,
                               std::vector<int64_t>& sorted_indices)
{
    // Resize the vector to match the number of elements in the row or column
    sorted_indices.resize(sort_by_row ? matrix.columns() : matrix.rows());
    
    // Initialize the vector with sequential indices
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);

    // Sort the indices based on the matrix values in the specified row or column
    if (sort_by_row)
    {
        // Sorting based on a specific row
        std::sort(sorted_indices.begin(), sorted_indices.end(),
                  [&matrix, index](int64_t i1, int64_t i2) { return matrix.circ_at(index, i1) < matrix.circ_at(index, i2); });
    }
    else
    {
        // Sorting based on a specific column
        std::sort(sorted_indices.begin(), sorted_indices.end(),
                  [&matrix, index](int64_t i1, int64_t i2) { return matrix.circ_at(i1, index) < matrix.circ_at(i2, index); });
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class SortedView
 * @brief Class for sorting rows or columns of a matrix expression
 *        using a user specified row or column to use as the sorting
 *        vector for all the other rows or columns in the matrix
 *        expression
 *
 * @tparam MatrixType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

class SortedView : public BaseMatrix< SortedView<MatrixType> >
{
public:

    // Type of value that is stored in the expression
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type>::type;



    SortedView<MatrixType>(MatrixType& expression, int64_t index_of_row_or_column_to_use_to_sort, bool sort_by_rows)
    : expression_(expression),
      index_of_row_or_column_to_use_to_sort_(index_of_row_or_column_to_use_to_sort),
      sort_by_rows_(sort_by_rows)
    {
        get_sorted_indices(expression_, index_of_row_or_column_to_use_to_sort_, sort_by_rows_, sorted_indeces_);
    }
    


    uintptr_t rows()const
    {
        return this->expression_.rows();
    }

    uintptr_t columns()const
    {
        return this->expression_.columns();
    }



    const MatrixType& get_expression()
    {
        return expression_;
    }

    int64_t get_index_of_row_to_use_to_sort()const
    {
        return index_of_row_or_column_to_use_to_sort_;
    }



    value_type& at_(int64_t row, int64_t column)
    {
        if(sort_by_rows_)
            return expression_.circ_at(sorted_indeces_[row], column);
        else
            return expression_.circ_at(row, sorted_indeces_[column]);
    }

    const value_type& at_(int64_t row, int64_t column)const
    {
        if(sort_by_rows_)
            return expression_.circ_at(sorted_indeces_[row], column);
        else
            return expression_.circ_at(row, sorted_indeces_[column]);
    }



private:

    MatrixType& expression_;

    int64_t index_of_row_or_column_to_use_to_sort_;

    bool sort_by_rows_;

    std::vector<int64_t> sorted_indeces_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< SortedView<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class Sorted
 * @brief Class for sorting rows or columns of a matrix expression
 *        using a user specified row or column to use as the sorting
 *        vector for all the other rows or columns in the matrix
 *        expression
 * 
 *        This one does not allow modification of original matrix data
 *
 * @tparam MatrixType The type of the matrix expression.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

class Sorted : public BaseMatrix< Sorted<MatrixType> >
{
public:

    // Type of value that is stored in the expression
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type>::type;



    Sorted<MatrixType>(const MatrixType& expression, int64_t index_of_row_or_column_to_use_to_sort, bool sort_by_rows)
    : expression_(expression),
      index_of_row_or_column_to_use_to_sort_(index_of_row_or_column_to_use_to_sort),
      sort_by_rows_(sort_by_rows)
    {
        get_sorted_indices(expression_, index_of_row_or_column_to_use_to_sort_, sort_by_rows_, sorted_indeces_);
    }
    


    uintptr_t rows()const
    {
        return this->expression_.rows();
    }

    uintptr_t columns()const
    {
        return this->expression_.columns();
    }



    const MatrixType& get_expression()
    {
        return expression_;
    }

    int64_t get_index_of_row_to_use_to_sort()const
    {
        return index_of_row_or_column_to_use_to_sort_;
    }



    const value_type& at_(int64_t row, int64_t column)const
    {
        if(sort_by_rows_)
            return expression_.circ_at(sorted_indeces_[row], column);
        else
            return expression_.circ_at(row, sorted_indeces_[column]);
    }



private:

    const MatrixType& expression_;

    int64_t index_of_row_or_column_to_use_to_sort_;

    bool sort_by_rows_;

    std::vector<int64_t> sorted_indeces_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< Sorted<MatrixType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// helper functions
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

sorted_matrix_view(MatrixType& m, int64_t index_of_row_or_column_to_use_to_sort, bool sort_by_rows)
{
    return SortedView<MatrixType>(m, index_of_row_or_column_to_use_to_sort, sort_by_rows);
}



template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

inline auto

sorted_matrix(const MatrixType& m, int64_t index_of_row_or_column_to_use_to_sort, bool sort_by_rows)
{
    return Sorted<MatrixType>(m, index_of_row_or_column_to_use_to_sort, sort_by_rows);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif // SORTED_VIEW_HPP