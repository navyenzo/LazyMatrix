//-------------------------------------------------------------------
/**
 * @file sorting_view.hpp
 * @brief Provides functionality to create a view of a matrix with sorted rows or columns.
 *
 * This file contains the function for generating a vector of sorted indices based on 
 * the values in a specified row or column of a matrix. This functionality is key in 
 * creating views of matrices where the rows or columns are sorted, without altering 
 * the original matrix data. The generated index vector is used for accessing the matrix 
 * elements in a sorted order.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef SORTED_VIEW_HPP
#define SORTED_VIEW_HPP



//-------------------------------------------------------------------
#include <vector>
#include <algorithm>
#include <numeric>

#include "base_matrix.hpp"
#include "shared_references.hpp"
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
 * @tparam ReferenceType The type of the matrix expression.
 * @param matrix The matrix expression.
 * @param index The index of the row or column to be sorted.
 * @param sort_by_row Boolean flag to indicate row-wise (true) or column-wise (false) sorting.
 * @param sorted_indices A reference to a vector where sorted indices will be stored.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline void get_sorted_indices(const ReferenceType& matrix,
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
 * @brief Class for creating a sorted view of a matrix expression
 *        that either sorts rows or columns.
 *        It uses a user specified row or column to sort all other rows
 *        or columns of the input matrix expression.
 *
 * @tparam Reference The type of the shared matrix reference.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

class SortedView : public BaseMatrix<SortedView<ReferenceType>,
                                     typename ReferenceType::value_type,
                                     has_non_const_access<ReferenceType>::value>
{
public:

    // Type of value that is stored in the expression
    using value_type = typename ReferenceType::value_type;

    friend class BaseMatrix<SortedView<ReferenceType>,
                            typename ReferenceType::value_type,
                            has_non_const_access<ReferenceType>::value>;

    /**
     * @brief Construct a new Sorted View object
     * 
     * @param expression The input matrix to be sorted.
     * @param index_of_row_or_column_to_use_to_sort Which row or column to use when sorting.
     * @param sort_by_rows Boolean whether to sort by rows or columns
     */
    SortedView(ReferenceType expression,
               int64_t index_of_row_or_column_to_use_to_sort,
               bool sort_by_rows)
    : expression_(expression),
      index_of_row_or_column_to_use_to_sort_(index_of_row_or_column_to_use_to_sort),
      sort_by_rows_(sort_by_rows)
    {
        get_sorted_indices(expression_, index_of_row_or_column_to_use_to_sort_, sort_by_rows_, sorted_indeces_);
    }

    /**
     * @brief Sets the reference to the matrix expression
     * @param left_side_expression Reference to the matrix.
     */
    void set_expression(ReferenceType expression)
    {
        expression_ = expression;
        get_sorted_indices(expression_, index_of_row_or_column_to_use_to_sort_, sort_by_rows_, sorted_indeces_);
    }

    /**
     * @brief Set the index of row or column to use to sort object
     * 
     * @param index_of_row_or_column_to_use_to_sort 
     */
    void set_index_of_row_or_column_to_use_to_sort(int64_t index_of_row_or_column_to_use_to_sort)
    {
        index_of_row_or_column_to_use_to_sort_ = index_of_row_or_column_to_use_to_sort;
        get_sorted_indices(expression_, index_of_row_or_column_to_use_to_sort_, sort_by_rows_, sorted_indeces_);
    }

    /**
     * @brief Set the sort by rows object
     * 
     * @param sort_by_rows 
     */
    void set_sort_by_rows(bool sort_by_rows)
    {
        sort_by_rows_ = sort_by_rows;
        get_sorted_indices(expression_, index_of_row_or_column_to_use_to_sort_, sort_by_rows_, sorted_indeces_);
    }

    /**
     * @brief Returns the number of rows Of the resulting matrix.
     */
    uintptr_t rows()const
    {
        return expression_.rows();
    }

    /**
     * @brief Returns the total number of columns of the resulting matrix.
     */
    uintptr_t columns()const
    {
        return expression_.columns();
    }
    
    /**
     * @brief Get the index of row or column to use to sort object
     * @return The index of row or column used to sort object
     */
    int64_t get_index_of_row_to_use_to_sort()const
    {
        return index_of_row_or_column_to_use_to_sort_;
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
        if(sort_by_rows_)
            return expression_.circ_at(row, sorted_indeces_[column]);
        else
            return expression_.circ_at(sorted_indeces_[row], column);
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
        if(sort_by_rows_)
            return expression_.circ_at(row, sorted_indeces_[column]);
        else
            return expression_.circ_at(sorted_indeces_[row], column);
    }



private: // Private variables

    ReferenceType expression_;

    int64_t index_of_row_or_column_to_use_to_sort_;

    bool sort_by_rows_;

    std::vector<int64_t> sorted_indeces_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename ReferenceType>

struct is_type_a_matrix< SortedView<ReferenceType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// helper functions
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Creates a sorted view of a matrix.
 * @tparam ReferenceType Type of the input matrix expression.
 * @param m Shared reference to the input matrix expression
 * @param index_of_row_or_column_to_use_to_sort Index of the row or column to use for sorting.
 * @param sort_by_rows Flag to indicate whether to sort by rows.
 * @return A SharedMatrixRef to the SortedView matrix object.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

inline auto

create_sorted_matrix_view(ReferenceType m,
                          int64_t index_of_row_or_column_to_use_to_sort,
                          bool sort_by_rows)
{
    auto view = std::make_shared<SortedView<ReferenceType>>(m, index_of_row_or_column_to_use_to_sort, sort_by_rows);

    if constexpr (has_non_const_access<ReferenceType>::value)
    {
        return SharedMatrixRef<SortedView<ReferenceType>>(view);
    }
    else
    {
        return ConstSharedMatrixRef<SortedView<ReferenceType>>(view);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif // SORTED_VIEW_HPP