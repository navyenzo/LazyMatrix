//-------------------------------------------------------------------
/**
 * @file database_matrix.hpp
 * @brief Defines the DatabaseMatrix class and associated safety classes.
 *
 * This header file provides the definition of the DatabaseMatrix class, which
 * allows accessing and manipulating data in a SQL database table as if it were
 * a 2D matrix. It utilizes Poco's Data library for database interaction and
 * supports basic matrix operations. The file also includes SafeTableName and
 * SafeColumnName classes to prevent SQL injection by sanitizing table and
 * column names.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_DATABASE_MATRIX_HPP_
#define INCLUDE_DATABASE_MATRIX_HPP_



//-------------------------------------------------------------------
#include <string>
#include <regex>
#include <stdexcept>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cstdint>
#include <ctime>
#include <chrono>
#include <iostream>
#include <sstream>
#include <fstream>
#include <locale>

#include <Poco/Data/Session.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>
#include <Poco/Data/Statement.h>

#include "base_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Forward declation of the MatrixFactory class which is
 *        used to create SharedMatrixRef references of database matrices.
 */
//-------------------------------------------------------------------
class MatrixFactory;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class SafeName
 * @brief Class for validating and encapsulating database identifiers like table and column names.
 *
 * Validates a database identifier (table name, column name, etc.) against
 * a strict pattern. Stores an error message if the identifier is invalid.
 */
//-------------------------------------------------------------------
class SafeName
{
public:
 
    /**
     * Constructor for SafeName.
     * @param name The raw database identifier to be validated.
     */
    SafeName(const std::string& name)
    {
        set(name);
    }

    /**
     * Constructor for SafeName.
     * @param name The raw database identifier to be validated.
     */
    void set(const std::string& name)
    {
        if (!std::regex_match(name, std::regex("^[a-zA-Z0-9_]+$")))
        {
            last_error_ = "Invalid identifier: " + name;
            sanitized_name_.clear(); // Clear the name as it's invalid
        }
        else
        {
            sanitized_name_ = name; // Set the name if it's valid
        }
    }

    /**
     * Gets the sanitized database identifier.
     * @return The sanitized identifier.
     */
    const std::string& get() const
    {
        return sanitized_name_;
    }

    /**
     * Gets the last error message.
     * @return The last error message.
     */
    const std::string& get_last_error() const
    {
        return last_error_;
    }

private:

    std::string sanitized_name_; ///< The sanitized database identifier.
    std::string last_error_;    ///< Last error message.
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class SafeRowSortingMethod
 * @brief Class for safely handling row sorting methods in database queries.
 *
 * Encapsulates a sorting method for database rows. Validates the column
 * name and sorting order. Stores an error message if there is an issue.
 */
//-------------------------------------------------------------------
class SafeRowSortingMethod
{
public:

    /**
     * Constructor for SafeRowSortingMethod.
     * @param column The column name to be used in sorting, validated via SafeName.
     * @param order The sorting order ('ASC' or 'DESC').
     */
    SafeRowSortingMethod(const std::string& column = "", const std::string& order = "")
    {
        set_parameters(column, order);
    }

    /**
     * Sets the parameters for SafeRowSortingMethod.
     * @param column The column name to be used in sorting, validated via SafeName.
     * @param order The sorting order ('ASC' or 'DESC').
     */
    void set_parameters(const std::string& column, const std::string& order)
    {
        SafeName safe_column(column);

        if (!safe_column.get_last_error().empty())
        {
            last_error_ = safe_column.get_last_error();
            return; // Exit the constructor as the column name is invalid
        }

        sort_method = safe_column.get();

        if (order != "ASC" && order != "DESC")
        {
            last_error_ = "Invalid sorting order: " + order;
            sort_method.clear(); // Clear the sort method as it's invalid
        }
        else
        {
            sort_method += " " + order;
        }
    }

    /**
     * Gets the safe row sorting method.
     * @return The safe row sorting method string.
     */
    const std::string& get() const
    {
        return sort_method;
    }

    /**
     * Gets the last error message.
     * @return The last error message.
     */
    const std::string& get_last_error() const
    {
        return last_error_;
    }

private:

    std::string sort_method; ///< The safe sorting method string.
    std::string last_error_; ///< Last error message.
};
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
 * @brief Represents and accesses a SQL database table as a 2D matrix.
 *
 * This class provides an interface to treat SQL database tables as 2D matrices,
 * enabling matrix-like data access and operations. It uses a DatabaseWindow
 * for caching and efficient data retrieval. The class supports basic matrix
 * operations and ensures safe interaction with the database.
 */
//-------------------------------------------------------------------
class DatabaseMatrix : public BaseMatrix<DatabaseMatrix,false>
{
public:

    // Type of value that is stored in the expression
    using value_type = Poco::Dynamic::Var;

    friend class BaseMatrix<DatabaseMatrix,false>;

    friend class MatrixFactory;

     /**
     * @brief Constructor for DatabaseMatrix.
     *        Initializes the matrix with the given database session and table information.
     * @param session Database session for connectivity.
     * @param table_name SafeName object representing the sanitized table name.
     * @param condition SQL condition for data retrieval (default is empty).
     * @param cache_window_size Size of cache window for data retrieval (default is 100).
     * @param row_sorting_method SafeRowSortingMethod object for defining row sorting (default is empty).
     */
    DatabaseMatrix(Poco::Data::Session& session,
                   const SafeName& table_name,
                   const std::string& condition = "",
                   uintptr_t cache_window_size = 100,
                   const SafeRowSortingMethod& row_sorting_method = SafeRowSortingMethod("", ""));

    /**
     * @brief Gets the number of rows in the matrix.
     */
    uintptr_t rows() const;

    /**
     * @brief Gets the number of columns in the matrix.
     */
    uintptr_t columns() const;

    /**
     * @brief Sets the row sorting method for the matrix.
     * @param row_sorting_method The new sorting method.
     */
    void set_row_sorting_method(const SafeRowSortingMethod& row_sorting_method)const;

    /**
     * @brief Sets the SQL condition for data retrieval.
     * @param condition The new SQL condition.
     */
    void set_condition(const std::string& condition)const;

    /**
     * @brief Retrieves the last error message, if any.
     * @return The last error message.
     */
    const std::string& get_last_error() const;



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
     * Accesses an element of the matrix at the specified row and column.
     * @param row Row index.
     * @param column Column index.
     * @return The value at the specified position in the matrix.
     */
    value_type const_at_(int64_t row, int64_t column) const;

    /**
     * @brief Counts the number of rows in the matrix.
     */
    void count_rows()const;

    /**
     * @brief Counts the number of columns in the matrix.
     */
    void count_columns()const;

    /**
     * @brief Preloads data into the cache for the specified row and column.
     * @param row Row index around which to preload data.
     * @param column Column index around which to preload data.
     */
    void preload_data(int64_t row, int64_t column) const;



private: // Private variables

    Poco::Data::Session& session_;                      ///< Database session for connectivity.
    SafeName table_name_;                               ///< Sanitized table name.
    mutable SafeRowSortingMethod row_sorting_method_;   ///< Method for row sorting.
    mutable std::string condition_;                     ///< SQL condition for data retrieval.

    mutable DatabaseWindow cache_window_;               ///< Cache window for efficient data retrieval.
    mutable uintptr_t cache_window_size_ = 100;         ///< Size of the cache window.

    mutable uintptr_t rows_ = 0;                        ///< Number of rows in the matrix.
    mutable std::vector<std::string> column_names_;     ///< Names of columns in the matrix.

    mutable std::string last_error_;                    ///< Last error message, if any.
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
                                      const SafeName& table_name,
                                      const std::string& condition,
                                      uintptr_t cache_window_size,
                                      const SafeRowSortingMethod& row_sorting_method)
                                      : session_(session),
                                        table_name_(table_name),
                                        condition_(condition),
                                        cache_window_size_(cache_window_size),
                                        row_sorting_method_(row_sorting_method)
{
    count_rows();
    count_columns();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void DatabaseMatrix::count_rows()const
{
    try
    {
        rows_ = 0;
        Poco::Data::Statement row_statement(session_);
        
        // Use the already validated and sanitized table name directly
        row_statement << "SELECT COUNT(*) FROM " << table_name_.get();

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
inline void DatabaseMatrix::count_columns()const
{
    try
    {
        column_names_.clear();

        // Clear the cache
        cache_window_.clear();

        // Construct and execute the SQL query with a WHERE condition
        Poco::Data::Statement column_statement(session_);
        column_statement << "SELECT * FROM " << table_name_.get();

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
            last_error_ = "No columns found for table " + table_name_.get();
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
inline DatabaseMatrix::value_type DatabaseMatrix::const_at_(int64_t row, int64_t column) const
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

    select << "SELECT * FROM " << table_name_.get();

    if (!condition_.empty())
        select << " WHERE " << condition_;
    
    if (!row_sorting_method_.get().empty())
        select << " ORDER BY " << row_sorting_method_.get();
    
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
inline void DatabaseMatrix::set_row_sorting_method(const SafeRowSortingMethod& row_sorting_method)const
{
    if(row_sorting_method_.get() != row_sorting_method.get())
    {
        row_sorting_method_ = row_sorting_method;
        cache_window_.clear();
        preload_data(0,0);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void DatabaseMatrix::set_condition(const std::string& condition)const
{
    if(condition_ != condition)
    {
        condition_ = condition;
        count_rows();
        count_columns();
        cache_window_.clear();
        preload_data(0,0);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif // INCLUDE_DATABASE_MATRIX_HPP_