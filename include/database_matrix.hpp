//-------------------------------------------------------------------
/**
 * @file database_matrix.hpp
 * @brief Defines the DatabaseMatrix class for accessing database data as a matrix.
 *
 * This header file provides the definition of the DatabaseMatrix class, which
 * allows accessing and manipulating data in a SQL database table as if it were
 * a 2D matrix. It utilizes Poco's Data library for database interaction and
 * supports basic matrix operations.
 *
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @see https://github.com/navyenzo/LazyMatrix.git
 * @namespace LazyMatrix
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_DATABASE_MATRIX_HPP_
#define INCLUDE_DATABASE_MATRIX_HPP_



//-------------------------------------------------------------------
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

#include <Poco/Data/Session.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Data/Statement.h>

#include "base_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @struct DatabaseWindow
 * @brief Represents a window of cached data from the database.
 *
 * This structure manages a cache window for efficient data retrieval from
 * the database. It stores a range of rows and columns and the corresponding
 * cached data.
 */
//-------------------------------------------------------------------
struct DatabaseWindow
{
    std::vector<Poco::Dynamic::Var> cache; ///< The cache holding data within the window.
    int row1, column1;                     ///< Upper left corner of the window.
    int row2, column2;                     ///< Lower right corner of the window.

    DatabaseWindow() : row1(0), column1(0), row2(0), column2(0) {}

    bool is_data_found_in_window(int row, int column) const
    {
        return row >= row1 && row < row2 && column >= column1 && column < column2;
    }

    void resize_window(int r1, int c1, int r2, int c2)
    {
        row1 = std::min(r1, r2);
        column1 = std::min(c1, c2);
        row2 = std::max(r1, r2);
        column2 = std::max(c1, c2);
        cache.resize((row2 - row1) * (column2 - column1));
    }

    void clear()
    {
        this->resize_window(0, 0, 0, 0);
    }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class DatabaseMatrix
 * @brief Allows representing and accessing a SQL database table as a 2D matrix.
 *
 * DatabaseMatrix provides an interface to treat SQL database tables as 2D matrices,
 * enabling matrix-like data access. It uses a DatabaseWindow for caching and efficient
 * data retrieval.
 */
//-------------------------------------------------------------------
class DatabaseMatrix : public BaseMatrix<DatabaseMatrix>
{
public:

    using value_type = Poco::Dynamic::Var;

    DatabaseMatrix(Poco::Data::Session& session,
                   const std::string& table_name,
                   const std::string& condition = "",
                   uintptr_t cache_window_size = 100,
                   const std::string& row_sorting_method = "");

    const std::string& get_last_error() const;

    uintptr_t rows() const;
    uintptr_t columns() const;
    value_type at_(int64_t row, int64_t column) const;

    void set_row_sorting_method(const std::string& row_sorting_method);
    void set_condition(const std::string& condition);



private:

    void initial_setup();
    void count_rows();
    void count_columns();
    
    void preload_data(int64_t row, int64_t column) const;


    
    Poco::Data::Session& session_;
    std::string table_name_;
    std::string row_sorting_method_;
    std::string condition_;

    mutable DatabaseWindow cache_window_;
    uintptr_t cache_window_size_ = 100;

    uintptr_t rows_ = 0;
    std::vector<std::string> column_names_;

    std::string last_error_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is a matrix expression type
//-------------------------------------------------------------------
template<>

struct is_type_a_matrix< DatabaseMatrix > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline DatabaseMatrix::DatabaseMatrix(Poco::Data::Session& session,
                                      const std::string& table_name,
                                      const std::string& condition,
                                      uintptr_t cache_window_size,
                                      const std::string& row_sorting_method)
    : BaseMatrix<DatabaseMatrix>(),
      session_(session),
      table_name_(table_name),
      cache_window_size_(cache_window_size),
      row_sorting_method_(row_sorting_method)
{
    count_rows();
    count_columns();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void DatabaseMatrix::count_rows()
{
    try
    {
        rows_ = 0;
        Poco::Data::Statement row_statement(session_);
        row_statement << "SELECT COUNT(*) FROM " << table_name_;
        
        if (!condition_.empty())
            row_statement << " WHERE " << condition_;
        
        row_statement, Poco::Data::Keywords::into(rows_),
            Poco::Data::Keywords::now;
        row_statement.execute();
    }
    catch (const std::exception& e)
    {
        last_error_ = "Error counting rows: " + std::string(e.what());
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void DatabaseMatrix::count_columns()
{
    try
    {
        column_names_.clear();

        // Clear the cache
        cache_window_.clear();

        // Construct and execute the SQL query with a WHERE condition
        Poco::Data::Statement column_statement(session_);
        column_statement << "SELECT * FROM " << table_name_;
        
        if (!condition_.empty())
            column_statement << " WHERE " << condition_;
        
        column_statement << " LIMIT 1";
        column_statement.execute();

        // Retrieve column names from the result metadata
        Poco::Data::RecordSet record_set(column_statement);
        for (size_t i = 0; i < record_set.columnCount(); ++i)
        {
            column_names_.push_back(record_set.columnName(i));
        }

        if (column_names_.empty())
        {
            last_error_ = "No columns found for table " + table_name_;
        }
    }
    catch (const std::exception& e)
    {
        last_error_ = "Error counting columns: " + std::string(e.what());
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline uintptr_t DatabaseMatrix::rows() const
{
    return rows_;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline uintptr_t DatabaseMatrix::columns() const
{
    return column_names_.size();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline DatabaseMatrix::value_type DatabaseMatrix::at_(int64_t row, int64_t column) const
{
    // Check if the data is in the current cache window
    if (!cache_window_.is_data_found_in_window(row, column))
    {
        // If not, preload the data around the requested row and column
        preload_data(row, column);
    }

    // Calculate the index in the cache for the requested data
    size_t cache_index = (row - cache_window_.row1) * (cache_window_.column2 - cache_window_.column1) + (column - cache_window_.column1);

    // Return the data from the cache
    return cache_window_.cache[cache_index];
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void DatabaseMatrix::preload_data(int64_t row, int64_t column) const
{
    // Determine the range of rows to preload
    int64_t start_row = std::max(int64_t(0), row - int64_t(cache_window_size_) / int64_t(2));
    int64_t end_row = std::min(start_row + int64_t(cache_window_size_), int64_t(this->rows()));

    // Resize and reset the cache window
    cache_window_.resize_window(start_row, 0, end_row, this->columns());

    // Construct and execute the SQL query
    Poco::Data::Statement select(session_);

    select << "SELECT * FROM " << table_name_;

    if(!condition_.empty())
        select << " WHERE " << condition_;
    
    if(!condition_.empty())
        select << " ORDER BY " << row_sorting_method_;
    
    select << " LIMIT " << (end_row - start_row) << " OFFSET " << start_row;
    
    select.execute();

    // Load the data into the cache
    Poco::Data::RecordSet record_set(select);
    size_t cache_index = 0;
    bool more = record_set.moveFirst();
    while (more)
    {
        for (size_t i = 0; i < record_set.columnCount(); ++i)
        {
            cache_window_.cache[cache_index++] = record_set[i];
        }
        more = record_set.moveNext();
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void DatabaseMatrix::set_row_sorting_method(const std::string& row_sorting_method)
{
    if(row_sorting_method_ != row_sorting_method)
    {
        row_sorting_method_ = row_sorting_method;
        cache_window_.clear();
        preload_data(0,0);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void DatabaseMatrix::set_condition(const std::string& condition)
{
    if(condition_ != condition)
    {
        condition_ = condition;
        cache_window_.clear();
        preload_data(0,0);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif // INCLUDE_DATABASE_MATRIX_HPP_