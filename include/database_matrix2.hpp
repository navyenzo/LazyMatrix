//-------------------------------------------------------------------
/**
 * @file database_matrix.hpp
 * @brief Defines the DatabaseMatrix class for accessing database data as a matrix.
 *
 * This header file provides the definition of the DatabaseMatrix2 class, which
 * allows accessing and manipulating data in a SQL database table as if it were
 * a 2D matrix. It utilizes SOCI library for database interaction and supports
 * basic matrix operations.
 *
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @see https://github.com/navyenzo/LazyMatrix.git
 * @namespace LazyMatrix
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_DATABASE_MATRIX2_HPP_
#define INCLUDE_DATABASE_MATRIX2_HPP_



//-------------------------------------------------------------------
#include <vector>
#include <variant>
#include <string>
#include <cstddef>

#include <soci/soci.h>

#include "base_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Types recognized by DatabaseMatrix2 when reading data from databases
//-------------------------------------------------------------------
using DatabaseVariant = std::variant<
                                        char, short, int, long, long long, unsigned long, unsigned long long,
                                        double,
                                        std::string, std::vector<char>,
                                        std::tm,
                                        std::vector<char>,
                                        bool
                                    >;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @struct DatabaseWindow2
 * @brief Represents a window of cached data from the database using SOCI.
 *
 * This structure manages a cache window for efficient data retrieval from
 * the database. It stores a range of rows and the corresponding
 * cached data as variants.
 */
//-------------------------------------------------------------------
struct DatabaseWindow2
{
    std::vector<DatabaseVariant> cache;     ///< The cache holding data within the window.
    int row1, column1;                      ///< Upper left corner of the window.
    int row2, column2;                      ///< Lower right corner of the window.

    DatabaseWindow2() : row1(0), column1(0), row2(0), column2(0) {}

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
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class DatabaseMatrix2
 * @brief Allows representing and accessing a SQL database table as a 2D matrix.
 *
 * DatabaseMatrix provides an interface to treat SQL database tables as 2D matrices,
 * enabling matrix-like data access. It uses a DatabaseWindow2 for caching and efficient
 * data retrieval.
 */
//-------------------------------------------------------------------
class DatabaseMatrix2 : public BaseMatrix<DatabaseMatrix2>
{
public:

    DatabaseMatrix2(soci::session& session, const std::string& tableName, uintptr_t cache_window_size = 100);

    uintptr_t rows() const;
    uintptr_t columns() const;
    const DatabaseVariant& at_(int64_t row, int64_t column) const;



private:
    
    soci::session& session_;
    std::string tableName_;
    std::vector<std::string> column_names_;

    mutable DatabaseWindow2 cache_window_;
    uintptr_t cache_window_size_ = 100;

    uintptr_t rows_ = 0;

    void initial_setup();

    void preload_data(int64_t row, int64_t column) const;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is a matrix expression type
//-------------------------------------------------------------------
template<>

struct is_type_a_matrix< DatabaseMatrix2 > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline DatabaseMatrix2::DatabaseMatrix2(soci::session& session, const std::string& tableName, uintptr_t cache_window_size)
    : BaseMatrix<DatabaseMatrix2>(),
      session_(session),
      tableName_(tableName),
      cache_window_size_(cache_window_size)
{
    initial_setup();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void DatabaseMatrix2::initial_setup()
{
    // Fetching column names
    column_names_.clear();
    std::string query = "PRAGMA table_info(" + tableName_ + ")";
    soci::rowset<soci::row> rs = (session_.prepare << query);

    for (const auto& row : rs)
    {
        std::string columnName = row.get<std::string>(1); // Assuming the column name is in the second field
        column_names_.push_back(columnName);
    }

    // Fetching the number of rows
    rows_ = 0;
    session_ << "SELECT COUNT(*) FROM " << tableName_, soci::into(rows_);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline uintptr_t DatabaseMatrix2::rows() const
{
    return rows_;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline uintptr_t DatabaseMatrix2::columns() const
{
    return column_names_.size();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline const DatabaseVariant& DatabaseMatrix2::at_(int64_t row, int64_t column) const
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
inline void DatabaseMatrix2::preload_data(int64_t row, int64_t column) const
{
    // Determine the range of rows to preload
    int64_t start_row = std::max(int64_t(0), row - int64_t(cache_window_size_) / int64_t(2));
    int64_t end_row = std::min(start_row + int64_t(cache_window_size_), int64_t(this->rows()));

    // Resize and reset the cache window
    cache_window_.resize_window(start_row, 0, end_row, this->columns());

    // Construct and execute the SQL query
    soci::rowset<soci::row> rs = (session_.prepare << "SELECT * FROM " << tableName_ << " LIMIT " << (end_row - start_row) << " OFFSET " << start_row);

    // Load the data into the cache
    size_t cache_index = 0;
    for (auto it = rs.begin(); it != rs.end(); ++it)
    {
        const soci::row& db_row = *it;
        for (size_t i = 0; i < db_row.size(); ++i, ++cache_index)
        {
            const soci::column_properties& props = db_row.get_properties(i);
            switch (props.get_data_type())
            {
                case soci::data_type::dt_string:
                    cache_window_.cache[cache_index] = db_row.get<std::string>(i);
                    break;
                case soci::data_type::dt_double:
                    cache_window_.cache[cache_index] = db_row.get<double>(i);
                    break;
                case soci::data_type::dt_integer:
                    cache_window_.cache[cache_index] = db_row.get<int>(i);
                    break;
                case soci::data_type::dt_long_long:
                    cache_window_.cache[cache_index] = db_row.get<long long>(i);
                    break;
                case soci::data_type::dt_unsigned_long_long:
                    cache_window_.cache[cache_index] = db_row.get<unsigned long long>(i);
                    break;
                case soci::data_type::dt_date:
                    cache_window_.cache[cache_index] = db_row.get<std::tm>(i);
                    break;
                case soci::data_type::dt_blob:
                {
                    // Check if the column is null
                    bool isNull = db_row.get_indicator(i) == soci::i_null;

                    if (!isNull)
                    {
                        // Retrieve the column properties to determine if it's a blob
                        const soci::column_properties& props = db_row.get_properties(i);
                        if (props.get_data_type() == soci::data_type::dt_blob)
                        {
                            // Perform additional query to get the blob size
                            int blob_size = 0;
                            session_ << "SELECT LENGTH(" << props.get_name() << ") FROM " << tableName_ << " WHERE your_condition",
                                soci::into(blob_size);

                            // Use std::visit to resize the data in the existing variant if it's a vector<char>
                            std::visit([&](auto& val)
                            {
                                if constexpr (std::is_same_v<std::decay_t<decltype(val)>, std::vector<char>>)
                                {
                                    val.resize(blob_size, 0);
                                }
                            }, cache_window_.cache[cache_index]);

                            // Read data from blob into the vector if it's a vector<char>
                            if (blob_size > 0)
                            {
                                std::visit([&](auto& val)
                                {
                                    if constexpr (std::is_same_v<std::decay_t<decltype(val)>, std::vector<char>>)
                                    {
                                        // Execute another query to fetch the blob data
                                        session_ << "SELECT " << props.get_name() << " FROM " << tableName_ << " WHERE your_condition",
                                            soci::into(val);
                                    }
                                }, cache_window_.cache[cache_index]);
                            }
                        }
                    }
                    else
                    {
                        // Handle the case when the column is null by providing an empty vector
                        std::visit([&](auto& val)
                        {
                            if constexpr (std::is_same_v<std::decay_t<decltype(val)>, std::vector<char>>)
                            {
                                val.clear();
                            }
                        }, cache_window_.cache[cache_index]);
                    }
                    break;
                }
            }
        }
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif // INCLUDE_DATABASE_MATRIX2_HPP_
