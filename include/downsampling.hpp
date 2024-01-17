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
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
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
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>
         
inline void simple_downsampling(const MatrixType1& source,
                                MatrixType2& destination,
                                int64_t start_index,
                                int64_t end_index,
                                bool sample_rows)
{
    // Determine the step, referring to how many source
    // values we skip as we downsample
    double step = double(end_index - start_index) / double(sample_rows ? destination.columns() : destination.rows());

    // Iterate over each row if downsampling rows or column otherwise
    for(int64_t source_i = 0, dest_i = 0;
        source_i < (sample_rows ? source.rows() : source.columns()) &&
        dest_i < (sample_rows ? destination.rows() : destination.columns());
        ++source_i, ++dest_i)
    {
        int64_t dest_index = 0;
        double source_index = double(start_index);

        // Now for each row (or column), we need to downsample the source
        for(;
            dest_index < (sample_rows ? destination.columns() : destination.rows()) &&
            ((step > 0) ? (source_index < end_index) : (source_index > end_index));
            ++dest_index, source_index += step)
        {
            if(sample_rows)
                destination(dest_i, dest_index) = source.circ_at(source_i, int64_t(source_index));
            else
                destination(dest_index, dest_i) = source.circ_at(int64_t(source_index), source_i);
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
template<typename MatrixType1,
         typename MatrixType2,
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
        double avg_y = 0;
        int64_t avg_range_start = source_start_index + static_cast<int64_t>((i + 1) * every) + 1;
        int64_t avg_range_end = source_start_index + static_cast<int64_t>((i + 2) * every) + 1;
        int64_t avg_range_length = std::abs(avg_range_end - avg_range_start);
        for (int64_t idx = avg_range_start; idx < avg_range_end; ++idx)
        {
            avg_y += source.circ_at(idx);
        }
        avg_y /= avg_range_length;
        double avg_x = (avg_range_start + avg_range_end) / 2.0; // Average x-value for bucket

        // Find the point that forms the largest triangle
        double max_area = -1;
        int64_t next_a_index = a_index;

        for (int64_t range_offs = avg_range_start; range_offs < avg_range_end; ++range_offs)
        {
            // Calculate the area of the triangle formed by points a, b, and avg
            double x0 = a_index, y0 = source.circ_at(a_index);
            double x1 = range_offs, y1 = source.circ_at(range_offs);
            double area = std::abs((x0 - avg_x) * (y1 - y0) - (x0 - x1) * (avg_y - y0)) / 2;

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
 * @brief Downsampling x/y data points using the Largest Triangle Three Buckets (LTTB) algorithm.
 *
 * This function applies the LTTB algorithm to downsample a pair of x/y data points, useful for reducing
 * the number of data points in a 2D plot while preserving the visual shape of the data.
 * The function allows specifying start and end indexes for flexible downsampling, including reverse or circular sampling.
 *
 * @param x_source The x-coordinates of the data points.
 * @param y_source The y-coordinates of the data points.
 * @param x_destination The x-coordinates of the downsampled data.
 * @param y_destination The y-coordinates of the downsampled data.
 * @param source_start_index The starting index in the source data for downsampling.
 * @param source_end_index The ending index in the source data for downsampling.
 */
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         typename MatrixType3,
         typename MatrixType4,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{} &&
                          is_type_a_matrix<MatrixType2>{} &&
                          is_type_a_matrix<MatrixType3>{} &&
                          is_type_a_matrix<MatrixType4>{}>* = nullptr>

inline void downsample_lttb_xy(const MatrixType1& x_source,
                               const MatrixType2& y_source,
                               MatrixType3& x_destination,
                               MatrixType4& y_destination,
                               int64_t source_start_index,
                               int64_t source_end_index)
{
    size_t source_size = std::abs(source_end_index - source_start_index);
    size_t destination_size = x_destination.size();  // Assume x and y destinations have the same size

    if (destination_size == 0 || source_size == 0)
        return;

    if (destination_size >= source_size)
    {
        size_t count = 0;
        for (int64_t i = source_start_index; count < source_size; ++i, ++count)
        {
            x_destination(count) = x_source.circ_at(i);
            y_destination(count) = y_source.circ_at(i);
        }
        return;
    }

    if (destination_size == 1)
    {
        x_destination(0) = x_source.circ_at(source_start_index);
        y_destination(0) = y_source.circ_at(source_start_index);
        return;
    }

    auto every = static_cast<double>(source_size - 2) / static_cast<double>(destination_size - 2);

    int64_t a_index = source_start_index;
    x_destination(0) = x_source.circ_at(a_index);
    y_destination(0) = y_source.circ_at(a_index);
    size_t dest_index = 1;

    for (size_t i = 0; i < destination_size - 2; ++i)
    {
        double avg_x = 0, avg_y = 0;
        int64_t avg_range_start = source_start_index + static_cast<int64_t>((i + 1) * every) + 1;
        int64_t avg_range_end = source_start_index + static_cast<int64_t>((i + 2) * every) + 1;
        int64_t avg_range_length = std::abs(avg_range_end - avg_range_start);
        for (int64_t idx = avg_range_start; idx < avg_range_end; ++idx)
        {
            avg_x += x_source.circ_at(idx);
            avg_y += y_source.circ_at(idx);
        }
        avg_x /= avg_range_length;
        avg_y /= avg_range_length;

        double max_area = -1;
        int64_t next_a_index = a_index;

        for (int64_t range_offs = avg_range_start; range_offs < avg_range_end; ++range_offs)
        {
            double x0 = x_source.circ_at(a_index), y0 = y_source.circ_at(a_index);
            double x1 = x_source.circ_at(range_offs), y1 = y_source.circ_at(range_offs);
            double area = std::abs((x0 - avg_x) * (y1 - y0) - (x0 - x1) * (avg_y - y0)) / 2;

            if (area > max_area)
            {
                max_area = area;
                next_a_index = range_offs;
            }
        }

        x_destination(dest_index) = x_source.circ_at(next_a_index);
        y_destination(dest_index++) = y_source.circ_at(next_a_index);
        a_index = next_a_index;
    }

    x_destination(dest_index) = x_source.circ_at(source_end_index);
    y_destination(dest_index) = y_source.circ_at(source_end_index);
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
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
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
/**
 * @brief Downsamples each column or row of a matrix using the Largest Triangle Three Buckets (LTTB) algorithm for x/y graph downsampling.
 *
 * This function applies the LTTB algorithm to independently downsample each column or row of the source matrix
 * in the context of x/y graph data. One row (or column) is used as the x-axis data for all downsampled y-axis data.
 * The x_index can be any integer, as circ_at handles circular and negative indexing.
 *
 * @param source_matrix The source matrix containing the original data points.
 * @param destination_matrix The destination matrix where the downsampled data will be stored.
 * @param x_index The index of the row (or column) representing the x-axis, can be any integer.
 * @param start_index The starting index for downsampling.
 * @param end_index The ending index for downsampling.
 * @param sample_rows If true, rows will be downsampled; if false, columns will be downsampled.
 *
 * Note: The function returns immediately if the size of either source or destination matrix is zero.
 */
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{} && is_type_a_matrix<MatrixType2>{}>* = nullptr>
         
inline void downsample_lttb_matrix_xy(const MatrixType1& source_matrix,
                                      MatrixType2& destination_matrix,
                                      int64_t x_index,
                                      int64_t start_index,
                                      int64_t end_index,
                                      bool sample_rows)
{
    if (source_matrix.size() == 0 || destination_matrix.size() == 0)
        return; // Return immediately if source or destination matrix is empty

    if (sample_rows)
    {
        auto x_source_row = select_a_single_row(source_matrix, x_index);
        auto x_destination_row = select_a_single_row_view(destination_matrix, x_index);

        for (size_t row = 0; row < source_matrix.rows(); ++row)
        {
            if (row == x_index) continue; // Skip x-axis row

            auto y_source_row = select_a_single_row(source_matrix, row);
            auto y_destination_row = select_a_single_row_view(destination_matrix, row);

            downsample_lttb_xy(x_source_row, y_source_row, x_destination_row, y_destination_row, start_index, end_index);
        }
    }
    else
    {
        auto x_source_column = select_a_single_column(source_matrix, x_index);
        auto x_destination_column = select_a_single_column_view(destination_matrix, x_index);

        for (size_t column = 0; column < source_matrix.columns(); ++column)
        {
            if (column == x_index) continue; // Skip x-axis column

            auto y_source_column = select_a_single_column(source_matrix, column);
            auto y_destination_column = select_a_single_column_view(destination_matrix, column);

            downsample_lttb_xy(x_source_column, y_source_column, x_destination_column, y_destination_column, start_index, end_index);
        }
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_DATA_SAMPLING_HPP_
