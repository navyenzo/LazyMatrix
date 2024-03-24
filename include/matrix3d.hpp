//-------------------------------------------------------------------
/**
 * @file matrix3d.hpp
 * @brief Defines the Matrix3D class, an integral part of the LazyMatrix library for 3D memory-mapped matrix operations.
 *
 * This file introduces the Matrix3D class, which represents a 3D memory-mapped matrix, 
 * extending the capabilities of the LazyMatrix library to three dimensions. A key feature 
 * of this class is the inclusion of a mutex in the matrix header, enabling its use in 
 * inter-process communication (IPC). This mutex allows multiple processes to safely access 
 * and manipulate the matrix data, ensuring data integrity in concurrent environments. 
 * The Matrix3D class supports various operations such as resizing, initializing, and loading 
 * matrices from files, and is compatible with external libraries like Eigen and dlib for 
 * complex mathematical computations. By utilizing memory-mapped files and IPC mechanisms, 
 * the Matrix3D class offers a robust solution for handling large 3D matrices efficiently 
 * in multi-process applications. It builds upon the BaseMatrix3D class, using the Curiously 
 * Recurring Template Pattern (CRTP) to achieve polymorphism without virtual functions.
 * 
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_MATRIX3D_HPP_
#define INCLUDE_MATRIX3D_HPP_



//-------------------------------------------------------------------
#include <vector>
#include <fstream>
#include <mutex>

#include "files.hpp"

#include "base_matrix3d.hpp"
#include "shared_references.hpp"

// mio library for cross-platform memory-mapping
#include <single_include/mio/mio.hpp>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Header and Footer sections for a 3d matrix
 */
//-------------------------------------------------------------------
const std::string matrix3d_header_byte_sequence = "---begin_3d_---\n";
const std::string matrix3d_footer_byte_sequence = ":---end_3d_---:\n";

struct Matrix3DHeader
{
    char header[16] = {'-', '-', '-', 'b', 'e', 'g', 'i', 'n', '_', '3', 'd', '_', '-', '-', '-', '\n'};
    uintptr_t size_of_data_type = 8;

    uintptr_t pages = 0;
    uintptr_t rows = 0;
    uintptr_t columns = 0;
};

struct Matrix3DFooter
{
    char footer[16] = {':', '-', '-', '-', 'e', 'n', 'd', '_', '3', 'd', '_', '-', '-', '-', ':', '\n'};
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Function used to check whether a memory section does indeed
 *        contain a memory mapped 3d matrix.
 * 
 * @param memory_mapped_matrix3d Pointer to the beginning of the memory
 *                               section containing the 3d matrix.
 * @param memory_size_in_bytes Size in bytes of the memory section.
 * @return Boolean letting users know whether the memory does contain
 *         a 3d matrix.
 */
//-------------------------------------------------------------------
inline bool does_memory_contain_mapped_matrix3d(const char* memory_mapped_matrix3d,
                                                uintptr_t memory_size_in_bytes)
{
    uintptr_t minimum_size = sizeof(Matrix3DHeader) + sizeof(Matrix3DFooter);

    if(memory_size_in_bytes < minimum_size)
        return false;
    
    const Matrix3DHeader* header = reinterpret_cast<const Matrix3DHeader*>(memory_mapped_matrix3d);

    uintptr_t expected_size = minimum_size + header->size_of_data_type * header->rows * header->columns;

    return memory_size_in_bytes >= expected_size;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Matrix3D -- Class defining a memory mapped 3d matrix storage.
 */
//-------------------------------------------------------------------
template<typename DataType>
class Matrix3D : public BaseMatrix3D<Matrix3D<DataType>,true>
{
public:

    using value_type = DataType;
    friend class MatrixFactory;
    friend class BaseMatrix3D<Matrix3D<DataType>,true>;
    
    /**
     * @brief Default constructor that takes the size and an initial value.
     * 
     * @param pages 
     * @param rows 
     * @param columns 
     * @param initial_value 
     */
    explicit Matrix3D(uintptr_t pages = 0, uintptr_t rows = 0, uintptr_t columns = 0, const DataType& initial_value = static_cast<DataType>(0));

    /**
     * @brief Copy constructor which does a "shallow copy" of the input matrix.
     * 
     * @param matrix The matrix to shallow copy
     * @return Matrix3D<DataType>& 
     */
    explicit Matrix3D(const Matrix3D<DataType>& matrix);

    /**
     * @brief Construct a new Matrix 3 D object by memory mapping a matrix file.
     * 
     * @param file_to_load_matrix_from The file containing the matrix to memory map.
     */
    explicit Matrix3D(const std::string& file_to_load_matrix_from);

    /**
     * @brief Construct the matrix object from a reference matrix expression.
    */
    template<typename ReferenceType, std::enable_if_t<is_matrix3d_reference<ReferenceType>{}>* = nullptr>
    Matrix3D(ReferenceType matrix_expression);

    /**
     * @brief Assignment operator that does a "shallow copy" of the input matrix.
     * 
     * @param matrix The matrix to shallow copy
     * @return Matrix3D<DataType>& 
     */
    Matrix3D<DataType>& operator=(const Matrix3D<DataType>& matrix);

    /**
     * @brief Assignment operator from a reference matrix expression.
    */
    template<typename ReferenceType, std::enable_if_t<is_matrix3d_reference<ReferenceType>{}>* = nullptr>
    Matrix3D<DataType>& operator=(ReferenceType matrix_expression);

    /**
     * @brief Checks whether the memory mapped file actually contains a 3d matrix.
     */
    bool is_valid()const;

    /**
     * @brief Get the number of pages in the 3d matrix.
     */
    uintptr_t pages()const;

    /**
     * @brief Get the number of rows in the 3d matrix.
     */
    uintptr_t rows()const;

    /**
     * @brief Get the number of columns in the 3d matrix.
     */
    uintptr_t columns()const;

    /**
     * @brief Get the capacity of the memory mapped file.
     * @return The number of entries the 3d matrix can contain in the
     *         memory mapped file.
     */
    uintptr_t capacity()const;

    /**
     * @brief Get the size of the memory mapped file.
     */
    std::size_t get_mapped_file_size()const;

    /**
     * @brief Get the filename of the memory mapped file containing this 3d matrix.
     */
    const fs::path& get_filename_of_memory_mapped_file()const;

    /**
     * @brief Initialze the 3d matrix by setting all values to the initial value.
     * 
     * @param initial_value The value to set all entries in the 3d matrix.
     */
    void initialize(const DataType& initial_value);

    /**
     * @brief Create the memory mapped and set the 3d matrix Header and
     *        Footer sections and initialize the matrix entries
     * 
     * @param pages 
     * @param rows 
     * @param columns 
     * @param initial_value 
     * @param matrix_filename 
     * @return std::error_code The error code encountered while trying to
     *         create the memory mapped file for the matrix.
     */

    /**
     * @brief Create the memory mapped file to hold the 3d matrix and initializes it.
     * 
     * @param pages Number of pages of the 3d matrix.
     * @param rows Number of rows of the 3d matrix.
     * @param columns Number of columns of the 3d matrix.
     * @param initial_value Value used to initialize all the entries of the 3d matrix.
     * @param filename_template Filename to use for the memory mapped file (template)
     * @param directory_where_file_will_reside Directory where memory mapped file will be created.
     * @return std::error_code Error encountered while trying to create the 3d matrix memory mapped file.
     */
    std::error_code create_matrix(uintptr_t pages,
                                  uintptr_t rows,
                                  uintptr_t columns,
                                  const DataType& initial_value = static_cast<DataType>(0),
                                  const fs::path& filename_template = "XXXXXX",
                                  const fs::path& directory_where_file_will_reside = fs::temp_directory_path());

    /**
     * @brief Memory map a file containing a 3d matrix object.
     * 
     * @param file_to_load_matrix_from The file we are attempting to memory map.
     * @return std::error_code The error code encounterd while trying to memory
     *         map the matrix file
     */
    std::error_code load_matrix(const std::string& file_to_load_matrix_from);

    // Functions used to handle page, row and column header names
    std::string get_page_header(int64_t page_index) { return headers_.get_page_header(page_index); }
    std::string get_row_header(int64_t row_index) { return headers_.get_row_header(row_index); }
    std::string get_column_header(int64_t column_index) { return headers_.get_column_header(column_index); }
    void set_page_header(int64_t page_index, const std::string& page_header) { headers_.set_page_header(page_index, page_header); }
    void set_row_header(int64_t row_index, const std::string& row_header) { headers_.set_row_header(row_index, row_header); }
    void set_column_header(int64_t column_index, const std::string& column_header) { headers_.set_column_header(column_index, column_header); }



private: // Private functions

    /**
     * @brief Resize the current 3d matrix to user specified size.
     * 
     * @param pages 
     * @param rows 
     * @param columns 
     * @param initial_value 
     * @return std::error_code Error encountered while trying to resize matrix.
     */
    std::error_code resize_(uintptr_t pages,
                            uintptr_t rows,
                            uintptr_t columns,
                            const DataType& initial_value = static_cast<DataType>(0));
    
    /**
     * @brief Returns a copy of the entry at the (page,row,column) position in the matrix.
     * 
     * @param page 
     * @param row 
     * @param column 
     * @return DataType 
     */
    DataType const_at_(int64_t page, int64_t row, int64_t column)const;

    /**
     * @brief Returns a reference to the entry at the (page,row,column) position in the matrix.
     * 
     * @param page 
     * @param row 
     * @param column 
     * @return DataType& 
     */
    DataType& non_const_at_(int64_t page, int64_t row, int64_t column);

    /**
     * @brief Get the Header section.
     * 
     * @return const Matrix3DHeader* 
     */
    const Matrix3DHeader* get_header()const;

    /**
     * @brief Get the Header section.
     * 
     * @return Matrix3DHeader* 
     */
    Matrix3DHeader* get_header();

    /**
     * @brief Get the Footer section.
     * 
     * @return const Matrix3DFooter* 
     */
    const Matrix3DFooter* get_footer()const;

    /**
     * @brief Get the Footer section.
     * 
     * @return Matrix3DFooter* 
     */
    Matrix3DFooter* get_footer();



private: // Private variables
    
    mio::shared_mmap_sink mapped_file_;             ///< The memory mapped file containing the 3d matrix object.
    fs::path filename_of_memory_mapped_file_;       ///< The filename of the memory mapped file.

    PageAndRowAndColumnNames headers_;              ///< Page, Row and Column Headers
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename DataType>

struct is_type_a_matrix3d< Matrix3D<DataType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function definitions
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline Matrix3D<DataType>::Matrix3D(uintptr_t pages, uintptr_t rows, uintptr_t columns, const DataType& initial_value)
{
    this->resize_(pages, rows, columns, initial_value);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline Matrix3D<DataType>::Matrix3D(const Matrix3D<DataType>& matrix)
{
    filename_of_memory_mapped_file_ = matrix.get_filename_of_memory_mapped_file();

    std::error_code mapping_error;
    mapped_file_.map(filename_of_memory_mapped_file_, mapping_error);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>
template<typename ReferenceType, std::enable_if_t<is_matrix3d_reference<ReferenceType>{}>*>

inline Matrix3D<DataType>::Matrix3D(ReferenceType matrix_expression)
{
    auto pages = matrix_expression.pages();
    auto rows = matrix_expression.rows();
    auto columns = matrix_expression.columns();

    std::error_code error = this->resize_(pages,rows, columns);

    if(error)
        return;

    for(int64_t k = 0; k < pages; ++k)
        for(int64_t i = 0; i < rows; ++i)
            for(int64_t j = 0; j < columns; ++j)
                (*this)(k,i,j) = matrix_expression(k,i,j);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline Matrix3D<DataType>::Matrix3D(const std::string& file_to_load_matrix_from)
{
    this->load_matrix(file_to_load_matrix_from);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline Matrix3D<DataType>& Matrix3D<DataType>::operator=(const Matrix3D<DataType>& matrix)
{
    filename_of_memory_mapped_file_ = matrix.get_filename_of_memory_mapped_file();

    std::error_code mapping_error;
    mapped_file_.map(filename_of_memory_mapped_file_.string(), mapping_error);
    
    return (*this);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>
template<typename ReferenceType, std::enable_if_t<is_matrix3d_reference<ReferenceType>{}>*>

inline Matrix3D<DataType>& Matrix3D<DataType>::operator=(ReferenceType matrix_expression)
{
    auto pages = matrix_expression.pages();
    auto rows = matrix_expression.rows();
    auto columns = matrix_expression.columns();

    std::error_code error = this->resize_(pages,rows, columns);

    if(error)
        return (*this);

    for(int64_t k = 0; k < pages; ++k)
        for(int64_t i = 0; i < rows; ++i)
            for(int64_t j = 0; j < columns; ++j)
                (*this)(k,i,j) = matrix_expression(k,i,j);
    
    return (*this);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline bool Matrix3D<DataType>::is_valid()const
{
    return mapped_file_.is_open();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline uintptr_t Matrix3D<DataType>::pages()const
{
    return this->get_header()->pages;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline uintptr_t Matrix3D<DataType>::rows()const
{
    return this->get_header()->rows;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline uintptr_t Matrix3D<DataType>::columns()const
{
    return this->get_header()->columns;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline uintptr_t Matrix3D<DataType>::capacity()const
{
    return (get_mapped_file_size() - sizeof(Matrix3DHeader) + sizeof(Matrix3DFooter)) / sizeof(DataType);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline std::size_t Matrix3D<DataType>::get_mapped_file_size()const
{
    return mapped_file_.size();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline void Matrix3D<DataType>::initialize(const DataType& initial_value)
{
    for(int64_t i = 0; i < this->pages(); ++i)
    {
        for(int64_t j = 0; j < this->rows(); ++j)
        {
            for(int64_t k = 0; k < this->columns(); ++k)
            {
                (*this)(i,j,k) = initial_value;
            }
        }
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline const fs::path& Matrix3D<DataType>::get_filename_of_memory_mapped_file()const
{
    return filename_of_memory_mapped_file_;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline std::error_code Matrix3D<DataType>::resize_(uintptr_t pages,
                                                   uintptr_t rows,
                                                   uintptr_t columns,
                                                   const DataType& initial_value)
{
    return this->create_matrix(pages, rows, columns, initial_value);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline DataType Matrix3D<DataType>::const_at_(int64_t page, int64_t row, int64_t column)const
{
    return reinterpret_cast<const DataType*>(mapped_file_.cbegin() + sizeof(Matrix3DHeader))[page*rows()*columns() + row*columns() + column];
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline DataType& Matrix3D<DataType>::non_const_at_(int64_t page, int64_t row, int64_t column)
{
    return reinterpret_cast<DataType*>(mapped_file_.begin() + sizeof(Matrix3DHeader))[page*rows()*columns() + row*columns() + column];
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline const Matrix3DHeader* Matrix3D<DataType>::get_header()const
{
    return reinterpret_cast<const Matrix3DHeader*>(mapped_file_.cbegin());
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline Matrix3DHeader* Matrix3D<DataType>::get_header()
{
    return reinterpret_cast<Matrix3DHeader*>(mapped_file_.begin());
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline const Matrix3DFooter* Matrix3D<DataType>::get_footer()const
{
    return reinterpret_cast<const Matrix3DFooter*>(mapped_file_.cbegin() + sizeof(Matrix3DHeader) + this->size()*sizeof(DataType));
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline Matrix3DFooter* Matrix3D<DataType>::get_footer()
{
    return reinterpret_cast<Matrix3DFooter*>(mapped_file_.begin() + sizeof(Matrix3DHeader) + this->size()*sizeof(DataType));
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline std::error_code

Matrix3D<DataType>::create_matrix(uintptr_t pages,
                                  uintptr_t rows,
                                  uintptr_t columns,
                                  const DataType& initial_value,
                                  const fs::path& filename_template,
                                  const fs::path& directory_where_file_will_reside)
{
    std::error_code mapping_error;

    // Check first if the current map is not
    // already the correct size
    if(mapped_file_.is_open())
    {
        if(this->capacity() >= pages * rows * columns)
        {
            // This means the matrix we are currently
            // mapped to is of equal or greater size
            // than the matrix we need, so we just re-initialize
            // and fit the new matrix within this one
            // No need to create a new file and map to it

            if(pages * rows * columns == 0)
            {
                this->get_header()->pages = 0;
                this->get_header()->rows = 0;
                this->get_header()->columns = 0;
            }
            else
            {
                uintptr_t old_number_of_pages = this->pages();
                uintptr_t old_number_of_rows = this->rows();
                uintptr_t old_number_of_columns = this->columns();

                this->get_header()->pages = pages;
                this->get_header()->rows = rows;
                this->get_header()->columns = columns;
                
                for(int64_t k = old_number_of_pages; k < this->pages(); ++k)
                {
                    for(int64_t i = old_number_of_rows; i < this->rows(); ++i)
                    {
                        for(int64_t j = old_number_of_columns; j < this->columns(); ++j)
                        {
                            (*this)(k,i,j) = initial_value;
                        }
                    }
                }   
            }

            return mapping_error;
        }
    }

    mapped_file_.unmap();

    // Calculate the necessary size of the file
    // so that it can hold the matrix
    uintptr_t size_of_file = sizeof(Matrix3DHeader) + sizeof(Matrix3DFooter) + pages*rows*columns*sizeof(DataType);

    // Create the file and size it accordingly
    filename_of_memory_mapped_file_ = create_file_with_specified_size_and_unique_name(size_of_file,
                                                                                      mapping_error,
                                                                                      filename_template,
                                                                                      directory_where_file_will_reside);

    if(mapping_error)
        return mapping_error;

    // Finally we memory map the entire file
    mapped_file_.map(filename_of_memory_mapped_file_.string(), mapping_error);

    if(mapping_error)
        return mapping_error;
    
    // Write the Matrix Header
    std::copy(matrix3d_header_byte_sequence.cbegin(),
              matrix3d_header_byte_sequence.cend(),
              &this->get_header()->header[0]);
    this->get_header()->size_of_data_type = sizeof(DataType);
    this->get_header()->pages = pages;
    this->get_header()->rows = rows;
    this->get_header()->columns = columns;

    // Write the Matrix Footer
    std::copy(matrix3d_footer_byte_sequence.cbegin(),
              matrix3d_footer_byte_sequence.cend(),
              &this->get_footer()->footer[0]);

    // Finally we initialize the matrix values
    this->initialize(initial_value);

    // We are done
    return mapping_error;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>

inline std::error_code Matrix3D<DataType>::load_matrix(const std::string& file_to_load_matrix_from)
{
    std::error_code mapping_error;

    mapped_file_.unmap();
    filename_of_memory_mapped_file_= file_to_load_matrix_from;

    // First we check if the file exists
    if(!fs::exists(filename_of_memory_mapped_file_))
    {
        // The file does not exist, therefore, we
        // return an error
        mapping_error.assign(1,std::iostream_category());
        return mapping_error;
    }

    // We now know the file exists, so we check its size
    // to make sure it is sized correctly to host the
    // matrix it supposedly hosts
    uintptr_t minimum_file_size_needed_to_hold_a_matrix = sizeof(Matrix3DHeader) + sizeof(Matrix3DFooter);

    if(fs::file_size(filename_of_memory_mapped_file_) < minimum_file_size_needed_to_hold_a_matrix)
    {
        // The specified file is smaller than the minimum
        // size required to hold matrix information, so
        // we return an error
        mapping_error.assign(1,std::iostream_category());
        return mapping_error;
    }

    // Finally we memory map the entire file
    mapped_file_.map(filename_of_memory_mapped_file_.string(), mapping_error);

    if(mapping_error)
        return mapping_error;

    // Now that we have mapped the file, we check
    // whether the file is actually storing a mapped
    // matrix
    if(!does_memory_contain_mapped_matrix3d(mapped_file_.data(), mapped_file_.size()))
    {
        // This means the file did not actually store
        // a mapped matrix, so we just return an error
        mapping_error.assign(1,std::iostream_category());
        return mapping_error;
    }

    // We are done
    return mapping_error;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_MATRIX3D_HPP_
