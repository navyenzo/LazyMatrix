//-------------------------------------------------------------------
/**
 * @file csv_matrix.hpp
 * @brief Memory maps CSV files and provides matrix-like access to numeric data within.
 *
 * The CSVMatrix class in the LazyMatrix library is designed to handle large CSV files 
 * efficiently by memory-mapping them and allowing numeric data extraction with a matrix-like interface. 
 * It accounts for complexities in CSV formatting, such as data enclosed in double quotes. 
 * The class is templated on the DataType and extends the BaseMatrix class. Key features include 
 * customizable delimiters for strings, rows, columns, and decimal points, and methods to extract 
 * both raw string and numeric data from the CSV. It can also handle optional row and column headers. 
 * The class emphasizes efficient data access in large CSV files, making it suitable for applications 
 * that require processing and analyzing large datasets stored in CSV format. 
 * The integration with the mio library ensures cross-platform compatibility for memory-mapping operations.
 * 
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_CSV_MATRIX_HPP_
#define INCLUDE_CSV_MATRIX_HPP_



//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include <unordered_map>
#include <utility>

#include <string_view>

// mio library for cross-platform memory-mapping
#include "single_include/mio/mio.hpp"

#include "base_matrix.hpp"
#include "convert_numbers.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Forward declation of the MatrixFactory class which is
 *        used to create SharedMatrixRef references of csv matrices.
 */
//-------------------------------------------------------------------
class MatrixFactory;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class CSVMatrix
 * @brief Memory-mapped CSV file reader providing matrix-like access to data.
 *
 * This class represents a memory-mapped CSV file as a matrix, allowing for efficient access 
 * to its contents. It is particularly useful for large CSV files where loading the entire file 
 * into memory would be impractical. The class allows accessing individual data points with 
 * matrix-like row and column indices. It supports customizable delimiters for parsing 
 * the CSV file and can handle complex cases where data fields are enclosed in quotes. 
 * Additionally, it provides functionality to handle optional row and column headers, 
 * making it versatile for various CSV file formats. The CSVMatrix class extends 
 * the BaseMatrix interface, enabling it to integrate seamlessly into systems that 
 * operate on matrix-like data structures. This makes it suitable for applications 
 * in data analysis, scientific computing, and any domain where large CSV datasets 
 * are prevalent.
 *
 * @tparam DataType The data type of the elements in the CSV file. It dictates how the 
 *                  string contents of the CSV are converted into numerical values.
 */
//-------------------------------------------------------------------
template<typename DataType>

class CSVMatrix : public BaseMatrix<CSVMatrix<DataType>,false>
{
public:

    // Type of value that is stored in the expression
    using value_type = DataType;

    friend class BaseMatrix<CSVMatrix<DataType>,false>;

    friend class MatrixFactory;

    // Default constructor
    CSVMatrix(char string_delimiter = '\"',
              const std::string& row_delimiters = "\n\r",
              const std::string& column_delimiters = ", ",
              char decimal_point_delimiter = '.');



    char get_string_delimiter()const { return string_delimiter_; }
    const std::string& get_row_delimiters()const { return row_delimiters_; }
    const std::string& get_column_delimiters()const { return column_delimiters_; }
    char get_decimal_point_delimiter()const { return decimal_point_delimiter_; }
    void set_string_delimiter(const char& string_delimiter) { string_delimiter_ = string_delimiter; }
    void set_row_delimiters(const std::string& row_delimiters) { row_delimiters_ = row_delimiters; }
    void set_column_delimiters(const std::string& column_delimiters) { column_delimiters_ = column_delimiters; }
    void set_decimal_point_delimiter(char decimal_point_delimiter) { decimal_point_delimiter_ = decimal_point_delimiter; }

    uintptr_t rows()const { return rows_; }
    uintptr_t columns()const { return columns_; }

    std::string_view string_at(int64_t row, int64_t column)const;
    std::pair<Poco::Dynamic::Var, ResultType> dynamic_at(int64_t row, int64_t column)const;



    // Function used to load (actually map the file, not load) the data from a CSV file
    std::error_code load(const std::string& csv_data_filename,
                         bool does_first_row_contain_column_header_names,
                         bool does_first_column_contain_row_header_names);

    // Functions used to handle row and column header names
    std::string get_row_header(int64_t row_index) { return headers_.get_row_header(row_index); }
    std::string get_column_header(int64_t column_index) { return headers_.get_column_header(column_index); }
    void set_row_header(int64_t row_index, const std::string& row_header) { headers_.set_row_header(row_index, row_header); }
    void set_column_header(int64_t column_index, const std::string& column_header) { headers_.set_column_header(column_index, column_header); }



private: // Private functions


    decltype(auto) const_at_(int64_t row, int64_t column)const;
    
    uintptr_t find_end_of_current_row(uintptr_t current_position_in_csv_string)const;
    uintptr_t find_end_of_current_column(uintptr_t current_position_in_csv_string, uintptr_t end_of_row)const;
    
    uintptr_t find_nth_row(uintptr_t row_index)const;
    uintptr_t find_nth_column_in_current_row(uintptr_t column_index, uintptr_t current_position, uintptr_t end_of_row)const;

    std::pair<int, int> find_begin_end_indeces_of_csv_entry(uintptr_t row, uintptr_t column)const;

    uintptr_t count_number_of_columns_for_current_row(uintptr_t start_of_row, uintptr_t end_of_row)const;    
    void count_number_of_rows_and_columns();

    void parse_row_headers();
    void parse_column_headers();



private: // Private variables

    // Size of csv matrix (not counting row/column headers)
    uintptr_t rows_ = 0;
    uintptr_t columns_ = 0;

    // The memory map holding the csv data
    mio::shared_mmap_sink mapped_csv_;

    // Boolean flags used to indicate whether
    // the csv file contains row and column headers
    bool does_first_row_contain_column_header_names_ = false;
    bool does_first_column_contain_row_header_names_ = false;

    // Tokens used to parse the csv data
    char string_delimiter_ = '\"';
    std::string row_delimiters_ = "\n\r";
    std::string column_delimiters_ = ", ";
    std::string delimiters_to_checkFor_ = ", \n\r";
    char decimal_point_delimiter_ = '.';

    // Empty string returned when a user asks
    // for a row or column name that does not
    // exist
    std::string no_name_found_ = "";

    // Default string returned when user asks
    // for a (row,column) that is not found
    std::string default_value = "0";

    // Row and Column Headers
    RowAndColumnNames headers_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename DataType>

struct is_type_a_matrix< CSVMatrix<DataType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Default constructor
//-------------------------------------------------------------------
template<typename DataType>

inline CSVMatrix<DataType>::CSVMatrix(char string_delimiter,
                                      const std::string& row_delimiters,
                                      const std::string& column_delimiters,
                                      char decimal_point_delimiter)
{
    set_string_delimiter(string_delimiter);
    set_row_delimiters(row_delimiters);
    set_column_delimiters(column_delimiters);
    set_decimal_point_delimiter(decimal_point_delimiter);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to grab the numerical value
// at the user specified location assuming
// the csv represents a numerical matrix M(i,j)
//-------------------------------------------------------------------
template<typename DataType>

inline decltype(auto) CSVMatrix<DataType>::const_at_(int64_t row, int64_t column)const
{
    auto [begin,end] = find_begin_end_indeces_of_csv_entry(row, column);
    
    DataType value;
    
    from_string(value, &mapped_csv_.data()[0], begin, end, decimal_point_delimiter_);

    return value;
}



// String specializations
template<>
inline decltype(auto) CSVMatrix<std::string>::const_at_(int64_t row, int64_t column)const
{
    return this->string_at(row, column);
}

template<>
inline decltype(auto) CSVMatrix<std::string_view>::const_at_(int64_t row, int64_t column)const
{
    return this->string_at(row, column);
}



// Poco::Dynamic::Var specializations
template<>
inline decltype(auto) CSVMatrix<Poco::Dynamic::Var>::const_at_(int64_t row, int64_t column)const
{
    return this->dynamic_at(row, column);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to return a string view looking
// at the string entry at the user specified (i,j)
//-------------------------------------------------------------------
template<typename DataType>

inline std::string_view CSVMatrix<DataType>::string_at(int64_t row, int64_t column) const
{
    auto [begin, end] = find_begin_end_indeces_of_csv_entry(row, column);

    // Check if the string at the specified location
    // starts and ends with double quotes
    if(end > begin + 1 &&
       mapped_csv_[begin] == string_delimiter_ && mapped_csv_[end - 1] == string_delimiter_)
    {
        ++begin;
        --end;
    }

    std::string_view value(&mapped_csv_.data()[begin], end - begin);

    return value;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to return a pair consisting of the Type of data
// at the (row,column) position and a Poco::Dynamic::Var variable
// contained the data at the (row,column) position
//-------------------------------------------------------------------
template<typename DataType>

std::pair<Poco::Dynamic::Var, ResultType> CSVMatrix<DataType>::dynamic_at(int64_t row, int64_t column)const
{
    return interpret_string( this->string_at(row,column) );
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to load (actually map not load) the data from
// a CSV file
// - This function doesn't load anything, it actually memory maps
//   the specified CSV file and calculates the number of rows and
//   columns in the CSV file
// - It does not cache the locations of the rows and columns as those
//   are found on the fly
//   - TODO:  I might add caching at a later point
// - NOTE:  The algorithm does not count zero-sized rows and columns
//-------------------------------------------------------------------
template<typename DataType>

inline std::error_code CSVMatrix<DataType>::load(const std::string& csv_data_filename,
                                                 bool does_first_row_contain_column_header_names,
                                                 bool does_first_column_contain_row_header_names)
{
    does_first_row_contain_column_header_names_ = does_first_row_contain_column_header_names;
    does_first_column_contain_row_header_names_ = does_first_column_contain_row_header_names;

    std::error_code mapping_error;
    mapped_csv_.unmap();
    mapped_csv_.map(csv_data_filename, mapping_error);

    if(mapping_error)
        return mapping_error;
    
    count_number_of_rows_and_columns();

    if(does_first_column_contain_row_header_names_)
        parse_row_headers();
    
    if(does_first_row_contain_column_header_names_)
        parse_column_headers();

    return mapping_error;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to get the indices in the mapped
// csv string that corresponds to the (i,j)
// csv matrix location
// - The function returns a pair of integers
//   corresponding to the beginning/end of the
//   place in the csv string where the (i,j) entry
//   is stored
//-------------------------------------------------------------------
template<typename DataType>

inline std::pair<int, int> CSVMatrix<DataType>::find_begin_end_indeces_of_csv_entry(uintptr_t row, uintptr_t column)const
{
    uintptr_t nth_row_position_in_csv = find_nth_row(row);

    if(nth_row_position_in_csv >= mapped_csv_.size())
        return std::pair<int, int>(0,0);
    
    uintptr_t end_of_nth_row = find_end_of_current_row(nth_row_position_in_csv);

    std::pair<int,int> begin_end_of_column;

    begin_end_of_column.first = find_nth_column_in_current_row(column, nth_row_position_in_csv, end_of_nth_row);

    if(begin_end_of_column.first >= end_of_nth_row)
        begin_end_of_column.second = begin_end_of_column.first;
    
    begin_end_of_column.second = find_end_of_current_column(begin_end_of_column.first, end_of_nth_row);
    
    return begin_end_of_column;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to parse the row headers from the first column
//-------------------------------------------------------------------
template<typename DataType>

inline void CSVMatrix<DataType>::parse_row_headers()
{
    // The row headers are found in the first column
    // But if the csv file also contains column headers
    // we skip the first column of the first row

    uintptr_t header_begin = 0;

    if(does_first_row_contain_column_header_names_)
    {
        // In this case we skip the first row
        header_begin = find_end_of_current_row(0) + 1;
    }
    
    
    uintptr_t header_end = header_begin;

    uintptr_t end_of_row = 0;

    uintptr_t header_index = 0;

    while(header_begin < mapped_csv_.size())
    {
        end_of_row = find_end_of_current_row(header_begin);

        header_end = find_end_of_current_column(header_begin, end_of_row);

        auto header = std::string(&mapped_csv_.data()[header_begin], &mapped_csv_.data()[header_end]);

        this->set_row_header(header_index, header);

        ++header_index;

        header_begin = find_end_of_current_row(header_begin) + 1;
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to parse the column headers from the first row
//-------------------------------------------------------------------
template<typename DataType>

inline void CSVMatrix<DataType>::parse_column_headers()
{
    // The column headers are found in the first row
    // But if the csv also contains row headers, we
    // skip the first column

    uintptr_t headers_row_begin = 0;
    uintptr_t headers_row_end = find_end_of_current_row(headers_row_begin);

    if(does_first_column_contain_row_header_names_)
    {
        // In this case we skip the first column
        headers_row_begin = find_end_of_current_column(0, headers_row_end) + 1;
    }

    uintptr_t header_begin = headers_row_begin;
    uintptr_t header_end = headers_row_begin;

    uintptr_t header_index = 0;

    while(header_begin < headers_row_end)
    {
        header_end = find_end_of_current_column(header_begin, headers_row_end);

        auto header = std::string(&mapped_csv_.data()[header_begin], &mapped_csv_.data()[header_end]);

        this->set_column_header(header_index, header);

        ++header_index;

        header_begin = header_end + 1;
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// This function parses the csv string and
// counts the number of rows and columns
// - It does not count 0-sized rows and 0-sized columns
// - If some rows have a different number of
//   columns, it uses the maximum number as the
//   number of columns for the matrix
// - It does not cache the location of the starting/ending
//   positions in the csv file of the rows and columns
//   - TODO:  I might add optional caching of locations of
//            rows and columns at a later point
//-------------------------------------------------------------------
template<typename DataType>

inline void CSVMatrix<DataType>::count_number_of_rows_and_columns()
{
    rows_ = 0;
    columns_ = 0;

    if(!mapped_csv_.is_open())
        return;

    uintptr_t number_of_columns_in_current_row = 0;

    uintptr_t current_position = 0;
    uintptr_t end_of_row = 0;

    while(current_position < mapped_csv_.size())
    {
        end_of_row = this->find_end_of_current_row(current_position);

        number_of_columns_in_current_row = count_number_of_columns_for_current_row(current_position, end_of_row);

        if(number_of_columns_in_current_row > columns_)
            columns_ = number_of_columns_in_current_row;
        
        if(number_of_columns_in_current_row > 0)
            ++rows_;
        
        current_position = end_of_row + 1;
    }

    if(does_first_row_contain_column_header_names_ && rows_ > 0)
        --rows_;
    
    if(does_first_column_contain_row_header_names_ && columns_ > 0)
        --columns_;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to find the beginning of the
// nth row in the csv matrix
//-------------------------------------------------------------------
template<typename DataType>

inline uintptr_t CSVMatrix<DataType>::find_nth_row(uintptr_t row_index)const
{
    uintptr_t nth_row_begin_position = 0;
    uintptr_t nth_row_end_position = 0;

    int64_t current_row = -1;

    if(does_first_row_contain_column_header_names_)
    {
        nth_row_begin_position = find_end_of_current_row(nth_row_begin_position);

        if(nth_row_begin_position < mapped_csv_.size())
            ++nth_row_begin_position;
    }

    while(nth_row_begin_position < mapped_csv_.size() &&
          current_row < int64_t(row_index))
    {
        nth_row_end_position = find_end_of_current_row(nth_row_begin_position);

        if((nth_row_end_position - nth_row_begin_position) > 1)
            ++current_row;

        if(current_row < row_index)
        {
            nth_row_begin_position = nth_row_end_position;

            if(nth_row_begin_position < mapped_csv_.size())
                ++nth_row_begin_position;
        }
    }

    return nth_row_begin_position;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to find the beginning of the
// nth column in the current row
//-------------------------------------------------------------------
template<typename DataType>

inline uintptr_t CSVMatrix<DataType>::find_nth_column_in_current_row(uintptr_t column_index, uintptr_t current_position, uintptr_t end_of_row)const
{
    uintptr_t nth_column_position = current_position;
    uintptr_t current_column = 0;

    if(does_first_column_contain_row_header_names_)
    {
        nth_column_position = find_end_of_current_column(nth_column_position, end_of_row);

        if(nth_column_position < end_of_row)
            ++nth_column_position;
    }

    while(nth_column_position < end_of_row &&
            current_column < column_index)
    {
        nth_column_position = find_end_of_current_column(nth_column_position, end_of_row);

        if(nth_column_position < end_of_row)
        {
            ++nth_column_position;
            ++current_column;
        }
    }

    return nth_column_position;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to count the number of
// columns within a row
// - This function does not count 0-sized columns also
//-------------------------------------------------------------------
template<typename DataType>

inline uintptr_t CSVMatrix<DataType>::count_number_of_columns_for_current_row(uintptr_t start_of_row, uintptr_t end_of_row)const
{
    uintptr_t number_of_columns = 0;

    if(start_of_row >= end_of_row)
        return number_of_columns;

    uintptr_t current_number_of_string_delimiters = 0;

    uintptr_t current_position = start_of_row;

    while(current_position < end_of_row)
    {
        char current_character = mapped_csv_.data()[current_position];

        if(current_character == string_delimiter_)
        {
            ++current_number_of_string_delimiters;
        }
        else if(current_number_of_string_delimiters % 2 == 0)
        {
            if(column_delimiters_.find(current_character) != std::string::npos)
                ++number_of_columns;
        }

        ++current_position;
    }
    
    ++number_of_columns;

    return number_of_columns;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to find the end of the
// current row
//-------------------------------------------------------------------
template<typename DataType>

inline uintptr_t CSVMatrix<DataType>::find_end_of_current_row(uintptr_t current_position_in_csv_string)const
{
    // This is to avoid counting row delimiters
    // inside string quotes
    uintptr_t number_of_found_string_delimiters = 0;

    uintptr_t end_of_current_row = current_position_in_csv_string;

    while(end_of_current_row < mapped_csv_.size())
    {
        char current_character = mapped_csv_.data()[end_of_current_row];

        if(current_character == string_delimiter_)
        {
            ++number_of_found_string_delimiters;
        }
        else if(number_of_found_string_delimiters % 2 == 0)
        {
            if(row_delimiters_.find(current_character) != std::string::npos)
                return end_of_current_row;
        }

        ++end_of_current_row;
    }

    return end_of_current_row;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to find the end of the
// current column
//-------------------------------------------------------------------
template<typename DataType>

inline uintptr_t CSVMatrix<DataType>::find_end_of_current_column(uintptr_t current_position_in_csv_string, uintptr_t end_of_row)const
{
    // This is to avoid counting row/column delimiters
    // inside string quotes
    uintptr_t number_of_found_string_delimiters = 0;

    uintptr_t end_of_current_column = current_position_in_csv_string;

    while(end_of_current_column < end_of_row)
    {
        char current_character = mapped_csv_.data()[end_of_current_column];

        if(current_character == string_delimiter_)
        {
            ++number_of_found_string_delimiters;
        }
        else if(number_of_found_string_delimiters % 2 == 0)
        {
            if(column_delimiters_.find(current_character) != std::string::npos)
                return end_of_current_column;
        }

        ++end_of_current_column;
    }

    return end_of_current_column;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif // INCLUDE_CSV_MATRIX_HPP_