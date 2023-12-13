//-------------------------------------------------------------------
/**
 * @file downsampling.hpp
 * @brief This file contains functions useful for downsampling data in different ways.
 *
 * The functions in this file are designed to assist with downsampling and processing 
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
 * @brief Downsamples data from a source matrix to a destination matrix using circular access.
 *
 * This function selectively downsamples either rows or columns from the source matrix based on the specified
 * start and end indices. Circular access allows for flexibility in indexing, enabling scenarios such as
 * reverse sampling and wrapping around the matrix if indices are out of bounds.
 * 
 * Examples of usage:
 * - Forward Sampling: If start_index is 0 and end_index is 10, and we are sampling 5 data points, it
 *   will sample points at indices 0, 2, 4, 6, and 8.
 * - Reverse Sampling: If start_index is 10 and end_index is 0, it samples in reverse, from index 10 to 0.
 * - Circular Sampling: If start_index is 8 and end_index is 3 in a 10-element matrix, it will wrap around
 *   the matrix to sample the data.
 * 
 * The source and destination matrices can be different types but must be valid matrix expressions.
 * If downsampling rows, each row represents a data point, and the indices refer to columns. If downsampling
 * columns, each column represents a data point, and the indices refer to rows.
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
template<typename MatrixType1, typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>
         
inline void simple_downsampling(const MatrixType1& source,
                                MatrixType2& destination,
                                int64_t start_index,
                                int64_t end_index,
                                bool sample_rows)
{
    // Determine if reverse sampling is needed (start_index > end_index)
    bool reverse_sampling = start_index > end_index;

    // Determine the interval and range for sampling based on the sampling mode (rows/columns)
    int64_t interval, range;
    if (sample_rows)
    {
        // Calculate interval based on columns for row sampling
        interval = std::abs(end_index - start_index) / (destination.columns() - 1);
        range = destination.rows();
    }
    else
    {
        // Calculate interval based on rows for column sampling
        interval = std::abs(end_index - start_index) / (destination.rows() - 1);
        range = destination.columns();
    }

    // Adjust the interval for reverse sampling
    if (reverse_sampling)
    {
        interval = -interval;
    }

    // Iterate over the range of the destination matrix (rows or columns)
    for (int64_t i = 0; i < range; ++i)
    {
        // Iterate over the columns (for row sampling) or rows (for column sampling) of the destination matrix
        for (int64_t j = 0; j < (sample_rows ? destination.columns() : destination.rows()); ++j)
        {
            // Calculate the index in the source matrix to sample from
            int64_t sourceIndex = start_index + j * interval;

            // Use circular access to copy the sampled data from the source to the destination matrix
            if (sample_rows)
            {
                // Sampling rows: circularly copy columns from source to destination
                destination(j, i) = source.circ_at(j, sourceIndex);
            }
            else
            {
                // Sampling columns: circularly copy rows from source to destination
                destination(i, j) = source.circ_at(sourceIndex, j);
            }
        }
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_DATA_SAMPLING_HPP_
