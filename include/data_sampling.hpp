//-------------------------------------------------------------------
/**
 * @file sample_data_functions.hpp
 * @brief This file contains functions useful for sampling data in different ways.
 *
 * The functions in this file are designed to assist with sampling and processing 
 * data matrices, particularly for applications that require efficient and dynamic 
 * data manipulation like data visualization or analysis.
 *
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @see https://github.com/navyenzo/LazyMatrix.git
 * @namespace LazyMatrix
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_DATA_SAMPLING_HPP_
#define INCLUDE_DATA_SAMPLING_HPP_



//-------------------------------------------------------------------
#include <cstdint>

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
 * @brief Samples data from a source matrix to a destination matrix.
 *
 * This function selectively samples either rows or columns from the source matrix based on the specified
 * start and end indices. The source and destination matrices can be different types but must be valid matrix
 * expressions. If sampling rows, each row represents a data point, and the indices refer to columns.
 * If sampling columns, each column represents a data point, and the indices refer to rows.
 *
 * @tparam MatrixType1 The type of the source matrix (should support standard indexing operations).
 * @tparam MatrixType2 The type of the destination matrix (should support standard indexing operations).
 * @param source The source matrix to sample from.
 * @param destination The destination matrix where sampled data is stored.
 * @param start_index The starting index for sampling.
 * @param end_index The ending index for sampling.
 * @param sample_rows A boolean flag indicating whether to sample rows (true) or columns (false).
 */
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>

inline void simple_data_sampling(const MatrixType1& source,
                                 MatrixType2& destination,
                                 int64_t start_index,
                                 int64_t end_index,
                                 bool sample_rows)
{
    // Let's make sure start_index and end_index are valid
    start_index = std::max(int64_t(0), start_index);
    end_index = std::max(int64_t(0), end_index);

    // Determine the interval and range for sampling
    // based on the sampling mode (rows/columns)
    int64_t interval, range;
    if (sample_rows)
    {
        // Sampling rows: start/end indices refer to columns
        interval = std::max(uintptr_t(1), uintptr_t(end_index - start_index + 1) / destination.columns());
        range = destination.rows();
    }
    else
    {
        // Sampling columns: start/end indices refer to rows
        interval = std::max(uintptr_t(1), uintptr_t(end_index - start_index + 1) / destination.rows());
        range = destination.columns();
    }

    // Iterate over the range (rows or columns) of the destination matrix
    for (int i = 0; i < range; ++i)
    {
        // Calculate the index in the source matrix to sample from
        int sourceIndex = start_index + i * interval;
        if (sourceIndex > end_index) 
        {
            break; // Prevent going past the end index
        }

        // Copy the sampled data from the source to the destination matrix
        for (int j = 0; j < (sample_rows ? destination.columns() : destination.rows()); ++j)
        {
            if (sample_rows)
            {
                // Sampling rows: copy columns from source to destination
                destination(j, i) = source(j, sourceIndex);
            }
            else
            {
                // Sampling columns: copy rows from source to destination
                destination(i, j) = source(sourceIndex, j);
            }
        }
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_DATA_SAMPLING_HPP_
