//-------------------------------------------------------------------
/**
 * @file page_row_and_column_headers.hpp
 * @brief Defines the class used to apply page and row and column names to 3d-matrix-like data.
 *
 * The PageAndRowAndColumnHeaders class is used to apply header names to pages, rows
 * and columns to 3d-matrix-like data.
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



#ifndef INCLUDE_PAGE_AND_ROW_AND_COLUMN_HEADERS_
#define INCLUDE_PAGE_AND_ROW_AND_COLUMN_HEADERS_



//-------------------------------------------------------------------
#include "row_and_column_headers.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Base class for applying header names to pages and rows and columns.
 */
//-------------------------------------------------------------------
class PageAndRowAndColumnNames : public RowAndColumnNames
{
public:

    // Default constructor
    PageAndRowAndColumnNames() : RowAndColumnNames()
    {
    }

    // Get page header
    std::string get_page_header(int64_t page_index)
    {
        auto page_header_iter = page_headers_.find(page_index);
        if(page_header_iter != page_headers_.cend())
            return page_header_iter->second;
        else
            return std::string("page: ") + std::to_string(page_index);
    }

    // Set page header
    void set_page_header(int64_t page_index, const std::string& page_header)
    {
        if(page_header.empty())
            page_headers_.erase(page_index);
        else
            page_headers_[page_index] = page_header;
    }

    // Clear all page names
    void clear_page_header_names()
    {
        page_headers_.clear();
    }

    // Return number of set page header names
    int64_t get_number_of_set_page_header_names()const
    {
        return page_headers_.size();
    }



private:

    std::unordered_map<int64_t, std::string> page_headers_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_PAGE_AND_ROW_AND_COLUMN_HEADERS_
