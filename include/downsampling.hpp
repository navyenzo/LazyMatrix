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
#include "selector.hpp"
#include "selector_view.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Downsamples data from a source matrix to a destination matrix.
 *
 * This function selectively downsamples either rows or columns from the source matrix based on the specified
 * start and end indices. The flexibility of the 'circ_at' function allows for handling all types of indexing, 
 * including reverse and circular sampling.
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
    // Determine the interval and range for sampling based on the sampling mode (rows/columns)
    int64_t interval = std::abs(end_index - start_index) / (sample_rows ? destination.columns() - 1 : destination.rows() - 1);
    int64_t range = sample_rows ? destination.rows() : destination.columns();

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
/**
 * @brief Downsampling a single row or column vector using the Largest Triangle Three Buckets (LTTB) algorithm.
 *
 * This function applies the LTTB algorithm to downsample a single-dimensional data set (row or column vector).
 * It is particularly useful in reducing the number of data points while preserving the visual shape of the data.
 * The function allows specifying start and end indexes for flexible downsampling, including reverse or circular sampling.
 *
 * @param source The source vector (either row or column) containing the original data points.
 * @param destination The destination vector where the downsampled data will be stored.
 * @param source_start_index The starting index in the source data for downsampling.
 * @param source_end_index The ending index in the source data for downsampling.
 *
 * Example usage:
 * - Standard Downsampling:
 *   downsample_lttb(source, destination, 0, source.size() - 1);
 * - Reverse Downsampling:
 *   downsample_lttb(source, destination, source.size() - 1, -1);
 * - Circular Downsampling:
 *   downsample_lttb(source, destination, mid_point, mid_point - 1);
 *   where mid_point is some index in the middle of the source data.
 */
//-------------------------------------------------------------------
template<typename MatrixType1, typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>
         
inline void downsample_lttb(const MatrixType1& source,
                            MatrixType2& destination, 
                            int64_t source_start_index,
                            int64_t source_end_index)
{
    // Calculate the absolute size of the source data to sample
    size_t source_size = std::abs(source_end_index - source_start_index);
    size_t destination_size = destination.size();

    // Check if downsampling is needed
    if (destination_size == 0 || source_size == 0)
        return;

    // Directly copy source to destination if destination size is larger or equal to source
    if (destination_size >= source_size)
    {
        size_t count = 0;
        for (int64_t i = source_start_index; count < source_size; ++i, ++count)
        {
            destination(count) = source.circ_at(i);
        }
        return;
    }

    // If destination size is 1, just take the first point from the source
    if (destination_size == 1)
    {
        destination(0) = source.circ_at(source_start_index);
        return;
    }

    // Calculate the number of data points to skip (bucket size)
    auto every = static_cast<double>(source_size - 2) / static_cast<double>(destination_size - 2);

    int64_t a_index = source_start_index;
    destination(0) = source.circ_at(a_index);  // Always keep the first point
    size_t dest_index = 1;  // Start filling the destination from the second element

    // Iterate over each bucket to find the best point
    for (size_t i = 0; i < destination_size - 2; ++i)
    {
        // Calculate the average point for the next bucket (containing c)
        double avg_x = 0, avg_y = 0;
        int64_t avg_range_start = source_start_index + static_cast<int64_t>((i + 1) * every) + 1;
        int64_t avg_range_end = source_start_index + static_cast<int64_t>((i + 2) * every) + 1;

        int64_t avg_range_length = std::abs(avg_range_end - avg_range_start);
        for (int64_t idx = avg_range_start; idx < avg_range_end; ++idx)
        {
            avg_x += source.circ_at(idx).x;
            avg_y += source.circ_at(idx).y;
        }
        avg_x /= avg_range_length;
        avg_y /= avg_range_length;

        // Find the point that forms the largest triangle
        double max_area = -1;
        int64_t next_a_index = a_index;

        for (int64_t range_offs = avg_range_start; range_offs < avg_range_end; ++range_offs)
        {
            // Calculate the area of the triangle formed by points a, b, and avg
            double area = std::abs(
                (source.circ_at(a_index).x - avg_x) * (source.circ_at(range_offs).y - source.circ_at(a_index).y) -
                (source.circ_at(a_index).x - source.circ_at(range_offs).x) * (avg_y - source.circ_at(a_index).y)
            ) / 2;

            // Select the point that maximizes the area (forming the largest triangle)
            if (area > max_area)
            {
                max_area = area;
                next_a_index = range_offs;
            }
        }

        // Add the selected point to the destination at the current index
        destination(dest_index++) = source.circ_at(next_a_index);
        a_index = next_a_index;  // Update the starting point for the next triangle
    }

    // Always include the last point from the source
    destination(dest_index) = source.circ_at(source_end_index);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Downsamples each column or row of a matrix using the Largest Triangle Three Buckets (LTTB) algorithm.
 *
 * This function applies the LTTB algorithm to independently downsample each column or row of the source matrix.
 * It is useful for reducing the number of data points in a matrix while preserving the visual shape of the data.
 *
 * @param source_matrix The source matrix containing the original data points.
 * @param destination_matrix The destination matrix where the downsampled data will be stored.
 * @param start_index The starting index for downsampling.
 * @param end_index The ending index for downsampling.
 * @param sample_rows If true, rows will be downsampled; if false, columns will be downsampled.
 *
 * Example usage:
 * - Downsample columns:
 *   downsample_lttb_matrix(source_matrix, destination_matrix, 0, source_matrix.columns() - 1, false);
 * - Downsample rows:
 *   downsample_lttb_matrix(source_matrix, destination_matrix, 0, source_matrix.rows() - 1, true);
 */
template<typename MatrixType1, typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>

inline void downsample_lttb_matrix(const MatrixType1& source_matrix,
                                   MatrixType2& destination_matrix,
                                   int64_t start_index,
                                   int64_t end_index,
                                   bool sample_rows)
{
    if (sample_rows)
    {
        for (size_t row = 0; row < source_matrix.rows(); ++row)
        {
            auto current_source_row = select_a_single_row(source_matrix, row);
            auto current_destination_row = select_a_single_row_view(destination_matrix, row);
            downsample_lttb(current_source_row, current_destination_row, start_index, end_index);
        }
    }
    else
    {
        for (size_t column = 0; column < source_matrix.columns(); ++column)
        {
            auto current_source_column = select_a_single_column(source_matrix, column);
            auto current_destination_column = select_a_single_column_view(destination_matrix, column);
            downsample_lttb(current_source_column, current_destination_column, start_index, end_index);
        }
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_DATA_SAMPLING_HPP_
