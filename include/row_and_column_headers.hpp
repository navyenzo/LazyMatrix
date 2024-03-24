//-------------------------------------------------------------------
/**
 * @file row_and_column_headers.hpp
 * @brief Defines the class used to apply row and column names to 2d-matrix-like data.
 *
 * The RowAndColumnHeaders class is used to apply header names to rows and columns
 * to 2d-matrix-like data.
 * 
 * This is used by matrix expression templates to apply/store names to individual
 * rows and columns (Not all rows or columns might have a header/name).
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_ROW_AND_COLUMN_HEADERS_
#define INCLUDE_ROW_AND_COLUMN_HEADERS_



//-------------------------------------------------------------------
#include <iostream>
#include <cstdint>
#include <type_traits>
#include <unordered_map>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Base class for applying header names to rows and columns.
 */
//-------------------------------------------------------------------
class RowAndColumnNames
{
public:

    // Default constructor
    RowAndColumnNames()
    {
    }

    // Get row header
    std::string get_row_header(int64_t row_index) const
    {
        auto row_header_iter = row_headers_.find(row_index);
        if(row_header_iter != row_headers_.cend())
            return row_header_iter->second;
        else
            return std::string("row: ") + std::to_string(row_index);
    }

    // Get column header
    std::string get_column_header(int64_t column_index) const
    {
        auto column_header_iter = column_headers_.find(column_index);
        if(column_header_iter != column_headers_.cend())
            return column_header_iter->second;
        else
            return std::string("col: ") + std::to_string(column_index);
    }

    // Set row header
    void set_row_header(int64_t row_index, const std::string& row_header) const
    {
        if(row_header.empty())
            column_headers_.erase(row_index);
        else
            row_headers_[row_index] = row_header;
    }

    // Set column header
    void set_column_header(int64_t column_index, const std::string& column_header) const
    {
        if(column_header.empty())
            column_headers_.erase(column_index);
        else
            column_headers_[column_index] = column_header;
    }

    // Clear all row names
    void clear_row_header_names()
    {
        row_headers_.clear();
    }

    // Clear all column names
    void clear_column_header_names()
    {
        column_headers_.clear();
    }

    // Return number of set row header names
    int64_t get_number_of_set_row_header_names()const
    {
        return row_headers_.size();
    }

    // Return number of set column header names
    int64_t get_number_of_set_column_header_names()const
    {
        return column_headers_.size();
    }



private:

    mutable std::unordered_map<int64_t, std::string> row_headers_;
    mutable std::unordered_map<int64_t, std::string> column_headers_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_ROW_AND_COLUMN_HEADERS_
