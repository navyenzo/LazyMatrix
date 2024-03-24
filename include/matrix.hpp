//-------------------------------------------------------------------
/**
 * @file matrix.hpp
 * @brief Defines the Matrix class, a key component of the LazyMatrix library for memory-mapped matrix operations.
 *
 * This file introduces the Matrix class, representing a memory-mapped matrix for efficient storage and
 * manipulation of large datasets. A notable feature of this class is the inclusion of a mutex in the matrix header,
 * facilitating its use for inter-process communication (IPC). This mutex allows for synchronized access to the matrix
 * across different processes, ensuring safe and consistent data handling. The Matrix class supports various operations 
 * such as resizing, initialization, loading from and saving to files. It integrates with external libraries like Eigen 
 * and dlib for advanced mathematical computations. By leveraging memory-mapped files and IPC mechanisms, the Matrix class 
 * provides robust functionality for high-performance matrix operations in a multi-process environment. It inherits from the 
 * BaseMatrix class, utilizing the Curiously Recurring Template Pattern (CRTP) for polymorphic behavior.
 * 
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------




#ifndef INCLUDE_MATRIX_HPP_
#define INCLUDE_MATRIX_HPP_



//-------------------------------------------------------------------
#include <vector>
#include <fstream>
#include <mutex>

#include "files.hpp"

#include "base_matrix.hpp"
#include "shared_references.hpp"

// mio library for cross-platform memory-mapping
#include <single_include/mio/mio.hpp>

// eigen library for fast/efficient matrix math
#include "Eigen/Eigen"

// dlib library for neural networks
#include "dlib/mlp.h"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
const std::string matrix_header_byte_sequence = "::---begin---::\n";
const std::string matrix_footer_byte_sequence = "::----end----::\n";
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Struct representing the header of a memory-mapped matrix.
 * 
 * Contains metadata for the matrix including its size, row and column count. 
 * It also contains a mutex for synchronizing access in a multi-threaded environment.
 */
//-------------------------------------------------------------------
struct MatrixHeader
{
    char header[16] = {':', ':', '-', '-', '-', 'b', 'e', 'g', 'i', 'n', '-', '-', '-', ':', ':', '\n'};
    uintptr_t size_of_data_type = 8;
    uintptr_t rows = 0;
    uintptr_t columns = 0;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Struct representing the footer of a memory-mapped matrix.
 * 
 * Used to mark the end of the memory-mapped matrix data.
 */
//-------------------------------------------------------------------
struct MatrixFooter
{
    char footer[16] = {':', ':', '-', '-', '-', '-', 'e', 'n', 'd', '-', '-', '-', '-', ':', ':', '\n'};
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Checks if the given memory region contains a memory-mapped matrix.
 * 
 * It inspects the memory region to determine if it adheres to the layout 
 * and size requirements of a memory-mapped matrix, as specified by the MatrixHeader.
 * 
 * @param memory_mapped_matrix Pointer to the start of the memory-mapped region.
 * @param memory_size_in_bytes Size of the memory region in bytes.
 * @return true If the memory region contains a valid memory-mapped matrix.
 * @return false If the memory region does not contain a valid memory-mapped matrix.
 */
//-------------------------------------------------------------------
inline bool does_memory_contain_mapped_matrix(const char* memory_mapped_matrix,
                                              uintptr_t memory_size_in_bytes)
{
    uintptr_t minimum_size = sizeof(MatrixHeader) + sizeof(MatrixFooter);

    if(memory_size_in_bytes < minimum_size)
        return false;
    
    const MatrixHeader* header = reinterpret_cast<const MatrixHeader*>(memory_mapped_matrix);

    uintptr_t expected_size = minimum_size + header->size_of_data_type * header->rows * header->columns;

    return memory_size_in_bytes >= expected_size;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Forward declation of the MatrixFactory class which is used
 *        to create SharedMatrixRef references of matrices.
 */
//-------------------------------------------------------------------
class MatrixFactory;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Template class for a matrix that utilizes memory-mapped files for storage.
 * 
 * This allows the matrix to be shared across multiple processes. 
 * It inherits from BaseMatrix using CRTP (Curiously Recurring Template Pattern) 
 * to gain a polymorphic behavior while avoiding virtual functions.
 * 
 * @tparam DataType The type of data stored in the matrix.
 */
//-------------------------------------------------------------------
template<typename DataType>
class Matrix : public BaseMatrix<Matrix<DataType>,true>
{
public:

    // Type of value that is stored in the matrix
    using value_type = DataType;

    friend class MatrixFactory;

    friend class BaseMatrix<Matrix<DataType>,true>;

    /**
     * @brief Default constructor. Initializes a matrix with given rows and columns.
     * @param rows Number of rows in the matrix. Default is 0.
     * @param columns Number of columns in the matrix. Default is 0.
     * @param initial_value The initial value to fill the matrix. Default is 0.
     */
    Matrix(uintptr_t rows = 0, uintptr_t columns = 0, const DataType& initial_value = static_cast<DataType>(0));

    /**
     * @brief Shallow copy constructor. Copies the matrix structure but not the data.
     * @param matrix The source matrix for the shallow copy.
     */
    Matrix(const Matrix<DataType>& matrix);

    /**
     * @brief Constructor to memory map a matrix from a file.
     * @param file_to_load_matrix_from The file path to load the matrix from.
     */
    Matrix(const std::string& file_to_load_matrix_from);

    /**
     * @brief Constructor to create a matrix from a dlib matrix.
     * @tparam DataType2 Data type of the dlib matrix.
     * @tparam NR Number of rows in the dlib matrix.
     * @tparam NC Number of columns in the dlib matrix.
     * @tparam mem_manager Memory manager type of the dlib matrix.
     * @tparam layout Memory layout type of the dlib matrix.
     * @param dlib_matrix The dlib matrix to convert from.
     */
    template<typename DataType2, long NR, long NC, typename mem_manager, typename layout>
    Matrix(const dlib::matrix<DataType2, NR, NC, mem_manager, layout>& dlib_matrix);

    /**
     * @brief Constructor to create a matrix from an Eigen matrix.
     * @tparam DataType2 Data type of the Eigen matrix.
     * @param m The Eigen matrix to convert from.
     */
    template<typename DataType2>
    Matrix(const Eigen::MatrixBase<DataType2>& m);

    /**
     * @brief Construct a new Matrix object copying a matrix reference
     * 
     * @param matrix_expression The matrix to deep copy
     */
    template<typename ReferenceType, std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>
    Matrix(ReferenceType matrix_expression);

    /**
     * @brief Assignment operator for shallow copying from another matrix.
     * @param matrix The source matrix for the shallow copy.
     * @return Reference to this matrix after shallow copying.
     */
    Matrix<DataType>& operator=(const Matrix<DataType>& matrix);

    /**
     * @brief Assignment operator from a dlib matrix.
     * @tparam DataType2 Data type of the dlib matrix.
     * @tparam NR Number of rows in the dlib matrix.
     * @tparam NC Number of columns in the dlib matrix.
     * @tparam mem_manager Memory manager type of the dlib matrix.
     * @tparam layout Memory layout type of the dlib matrix.
     * @param dlib_matrix The dlib matrix to assign from.
     * @return Reference to this matrix after assignment.
     */
    template<typename DataType2, long NR, long NC, typename mem_manager, typename layout>
    Matrix<DataType>& operator=(const dlib::matrix<DataType2, NR, NC, mem_manager, layout>& dlib_matrix);

    /**
     * @brief Assignment operator from an eigen matrix expression.
     * @tparam DataType2 Data type of the Eigen matrix.
     * @param m The Eigen matrix to copy from.
     * @return Reference to this matrix object after assignment.
     */
    template<typename DataType2>
    Matrix<DataType>& operator=(const Eigen::MatrixBase<DataType2>& m);

    /**
     * @brief Assignement from a reference to a matrix expression.
     * 
     * @param matrix_expression The matrix to copy from
     */
    template<typename ReferenceType, std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>
    Matrix<DataType>& operator=(ReferenceType matrix_expression);

    /**
     * @brief Checks whether the memory mapped file actually contains a matrix.
     */
    bool is_valid()const;

    /**
     * @brief Get the number of rows in the matrix.
     */
    uintptr_t rows()const;

    /**
     * @brief Get the number of columns in the matrix.
     */
    uintptr_t columns()const;

    /**
     * @brief Get the capacity of the memory mapped file.
     * @return The number of entries the matrix can contain in the
     *         memory mapped file.
     */
    uintptr_t capacity()const;

    /**
     * @brief Get the size of the memory mapped file.
     */
    std::size_t get_mapped_file_size()const;

    /**
     * @brief Get the filename of the memory mapped file containing this matrix.
     */
    const fs::path& get_filename_of_memory_mapped_file()const;

    /**
     * @brief Initializes the matrix with a specified value.
     * @param initial_value The value to initialize the matrix with.
     */
    void initialize(const DataType& initial_value);

    /**
     * @brief Create the memory mapped file to hold the 3d matrix and initializes it.
     * 
     * @param rows Number of rows of the 3d matrix.
     * @param columns Number of columns of the 3d matrix.
     * @param initial_value Value used to initialize all the entries of the 3d matrix.
     * @param filename_template Filename to use for the memory mapped file (template)
     * @param directory_where_file_will_reside Directory where memory mapped file will be created.
     * @return std::error_code Error encountered while trying to create the 3d matrix memory mapped file.
     */
    std::error_code create_matrix(uintptr_t rows,
                                  uintptr_t columns,
                                  const DataType& initial_value = static_cast<DataType>(0),
                                  const fs::path& filename_template = "XXXXXX",
                                  const fs::path& directory_where_file_will_reside = fs::temp_directory_path());

    /**
     * @brief Loads a matrix from a file.
     * @param file_to_load_matrix_from The file path to load the matrix from.
     * @return Error code indicating success or failure of the operation.
     */
    std::error_code load_matrix(const std::string& file_to_load_matrix_from);

    // Functions used to handle row and column header names
    std::string get_row_header(int64_t row_index) const { return headers_.get_row_header(row_index); }
    std::string get_column_header(int64_t column_index) const { return headers_.get_column_header(column_index); }
    void set_row_header(int64_t row_index, const std::string& row_header) const { headers_.set_row_header(row_index, row_header); }
    void set_column_header(int64_t column_index, const std::string& column_header) const { headers_.set_column_header(column_index, column_header); }



private: // Private functions

    /**
     * @brief Resizes the matrix to a specified size.
     * @param rows The new number of rows.
     * @param columns The new number of columns.
     * @param initial_value The initial value to fill new elements with.
     * @return Error code indicating success or failure of the operation.
     */
    std::error_code resize_(uintptr_t rows, uintptr_t columns, const DataType& initial_value = static_cast<DataType>(0));

    /**
     * @brief Access operator for constant access.
     * @param row Row index.
     * @param column Column index.
     * @return Constant reference to the data at the specified index.
     */
    DataType const_at_(int64_t row, int64_t column)const;

    /**
     * @brief Access operator for mutable access.
     * @param row Row index.
     * @param column Column index.
     * @return Reference to the data at the specified index.
     */
    DataType& non_const_at_(int64_t row, int64_t column);

    /**
     * @brief Get the Header section.
     * 
     * @return const MatrixHeader* 
     */
    const MatrixHeader* get_header()const;

    /**
     * @brief Get the Header section.
     * 
     * @return MatrixHeader* 
     */
    MatrixHeader* get_header();

    /**
     * @brief Get the Footer section.
     * 
     * @return const MatrixFooter* 
     */
    const MatrixFooter* get_footer()const;

    /**
     * @brief Get the Footer section.
     * 
     * @return MatrixFooter* 
     */
    MatrixFooter* get_footer();



private: // Private variables
    
    // The mapped file used as the memory
    // for this memory mapped matrix and
    // its filename
    mio::shared_mmap_sink mapped_file_;
    fs::path filename_of_memory_mapped_file_;

    // Row and Column Headers
    mutable RowAndColumnNames headers_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is a matrix expression type
//-------------------------------------------------------------------
template<typename DataType>

struct is_type_a_matrix< Matrix<DataType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Default constructor from rows, columns and initial value
//-------------------------------------------------------------------
template<typename DataType>

inline Matrix<DataType>::Matrix(uintptr_t rows, uintptr_t columns, const DataType& initial_value)
{
    this->resize_(rows, columns, initial_value);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Copy constructor (We do shallow copy)
//-------------------------------------------------------------------
template<typename DataType>

inline Matrix<DataType>::Matrix(const Matrix<DataType>& matrix)
{
    filename_of_memory_mapped_file_ = matrix.get_filename_of_memory_mapped_file();

    std::error_code mapping_error;
    mapped_file_.map(filename_of_memory_mapped_file_.string(), mapping_error);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Constructor to memory map a matrix from file
//-------------------------------------------------------------------
template<typename DataType>

inline Matrix<DataType>::Matrix(const std::string& file_to_load_matrix_from)
{
    this->load_matrix(file_to_load_matrix_from);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Constructor from a dlib matrix
//-------------------------------------------------------------------
template<typename DataType>
template<typename DataType2, long NR, long NC, typename mem_manager, typename layout>

inline Matrix<DataType>::Matrix(const dlib::matrix<DataType2, NR, NC, mem_manager, layout>& dlib_matrix)
{
    this->resize_(dlib_matrix.nr(), dlib_matrix.nc());

    for(int64_t i = 0; i < this->rows(); ++i)
    {
        for(int64_t j = 0; j < this->columns(); ++j)
        {
            (*this)(i,j) = dlib_matrix(i,j);
        }
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Constructor from an eigen matrix
//-------------------------------------------------------------------
template<typename DataType>
template<typename DataType2>

inline Matrix<DataType>::Matrix(const Eigen::MatrixBase<DataType2>& m)
{
    this->resize_(m.rows(),m.cols());

    for(int64_t i = 0; i < m.rows(); ++i)
    {
        for(int64_t j = 0; j < m.cols(); ++j)
        {
            (*this)(i,j) = m(i,j);
        }
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Constructor from a matrix expression reference
//-------------------------------------------------------------------
template<typename DataType>
template<typename ReferenceType, std::enable_if_t<is_matrix_reference<ReferenceType>{}>*>

inline Matrix<DataType>::Matrix(ReferenceType matrix_expression)
{
    uintptr_t rows = matrix_expression.rows();
    uintptr_t columns = matrix_expression.columns();

    this->resize_(rows,columns);

    for(int64_t i = 0; i < rows; ++i)
    {
        for(int64_t j = 0; j < columns; ++j)
        {
            (*this)(i,j) = matrix_expression(i,j);
        }
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Assignment operator (does shallow copy)
//-------------------------------------------------------------------
template<typename DataType>

inline Matrix<DataType>& Matrix<DataType>::operator=(const Matrix<DataType>& matrix)
{
    filename_of_memory_mapped_file_ = matrix.get_filename_of_memory_mapped_file();

    std::error_code mapping_error;
    mapped_file_.map(filename_of_memory_mapped_file_.string(), mapping_error);

    return (*this);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Assignment from a dlib matrix
//-------------------------------------------------------------------
template<typename DataType>
template<typename DataType2, long NR, long NC, typename mem_manager, typename layout>

inline Matrix<DataType>& Matrix<DataType>::operator=(const dlib::matrix<DataType2, NR, NC, mem_manager, layout>& dlib_matrix)
{
    this->resize_(dlib_matrix.nr(), dlib_matrix.nc());

    for(int64_t i = 0; i < this->rows(); ++i)
    {
        for(int64_t j = 0; j < this->columns(); ++j)
        {
            (*this)(i,j) = dlib_matrix(i,j);
        }
    }

    return (*this);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Assignment from an eigen matrix
//-------------------------------------------------------------------
template<typename DataType>
template<typename DataType2>

inline Matrix<DataType>& Matrix<DataType>::operator=(const Eigen::MatrixBase<DataType2>& m)
{
    this->resize_(m.rows(),m.cols());

    for(int64_t i = 0; i < m.rows(); ++i)
    {
        for(int64_t j = 0; j < m.cols(); ++j)
        {
            (*this)(i,j) = m(i,j);
        }
    }

    return (*this);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Assignment from a reference to a matrix expression
//-------------------------------------------------------------------
template<typename DataType>
template<typename ReferenceType, std::enable_if_t<is_matrix_reference<ReferenceType>{}>*>

inline Matrix<DataType>& Matrix<DataType>::operator=(ReferenceType matrix_expression)
{
    auto rows = matrix_expression.rows();
    auto columns = matrix_expression.columns();

    std::error_code error = this->resize_(rows, columns);

    if(error)
        return (*this);

    for(int64_t i = 0; i < rows; ++i)
        for(int64_t j = 0; j < columns; ++j)
            (*this)(i,j) = matrix_expression(i,j);

    return (*this);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Getters
//-------------------------------------------------------------------
template<typename DataType>

inline bool Matrix<DataType>::is_valid()const
{
    return mapped_file_.is_open();
}



template<typename DataType>

inline uintptr_t Matrix<DataType>::rows()const
{
    return this->get_header()->rows;
}



template<typename DataType>

inline uintptr_t Matrix<DataType>::columns()const
{
    return this->get_header()->columns;
}



template<typename DataType>

inline uintptr_t Matrix<DataType>::capacity()const
{
    return (get_mapped_file_size() - sizeof(MatrixHeader) + sizeof(MatrixFooter)) / sizeof(DataType);
}



template<typename DataType>

inline uintptr_t Matrix<DataType>::get_mapped_file_size()const
{
    return mapped_file_.size();
}



template<typename DataType>

inline const fs::path& Matrix<DataType>::get_filename_of_memory_mapped_file()const
{
    return filename_of_memory_mapped_file_;
}



template<typename DataType>

inline const MatrixHeader* Matrix<DataType>::get_header()const
{
    return reinterpret_cast<const MatrixHeader*>(mapped_file_.cbegin());
}



template<typename DataType>

inline MatrixHeader* Matrix<DataType>::get_header()
{
    return reinterpret_cast<MatrixHeader*>(mapped_file_.begin());
}



template<typename DataType>

inline const MatrixFooter* Matrix<DataType>::get_footer()const
{
    return reinterpret_cast<const MatrixFooter*>(mapped_file_.cbegin() + sizeof(MatrixHeader) + this->size()*sizeof(DataType));
}



template<typename DataType>

inline MatrixFooter* Matrix<DataType>::get_footer()
{
    return reinterpret_cast<MatrixFooter*>(mapped_file_.begin() + sizeof(MatrixHeader) + this->size()*sizeof(DataType));
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Access operators
//-------------------------------------------------------------------
template<typename DataType>

inline DataType Matrix<DataType>::const_at_(int64_t row, int64_t column)const
{
    return reinterpret_cast<const DataType*>(mapped_file_.cbegin() + sizeof(MatrixHeader))[row*columns() + column];
}



template<typename DataType>

inline DataType& Matrix<DataType>::non_const_at_(int64_t row, int64_t column)
{
    return reinterpret_cast<DataType*>(mapped_file_.begin() + sizeof(MatrixHeader))[row*columns() + column];
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to initialize the matrix values
//-------------------------------------------------------------------
template<typename DataType>

inline void Matrix<DataType>::initialize(const DataType& initial_value)
{
    for(int64_t i = 0; i < this->rows(); ++i)
    {
        for(int64_t j = 0; j < this->columns(); ++j)
        {
            (*this)(i,j) = initial_value;
        }
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Alias for creating/resizing a matrix
//-------------------------------------------------------------------
template<typename DataType>

inline std::error_code Matrix<DataType>::resize_(uintptr_t rows, uintptr_t columns, const DataType& initial_value)
{
    return this->create_matrix(rows, columns, initial_value);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to create a matrix
//-------------------------------------------------------------------
template<typename DataType>

inline std::error_code

Matrix<DataType>::create_matrix(uintptr_t rows,
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
        if(this->capacity() >= rows * columns)
        {
            // This means the matrix we are currently
            // mapped to is of equal or greater size
            // than the matrix we need, so we just re-initialize
            // and fit the new matrix within this one
            // No need to create a new file and map to it

            if(rows * columns == 0)
            {
                this->get_header()->rows = 0;
                this->get_header()->columns = 0;
            }
            else
            {
                uintptr_t old_number_of_rows = this->rows();
                uintptr_t old_number_of_columns = this->columns();

                this->get_header()->rows = rows;
                this->get_header()->columns = columns;

                for(int64_t i = old_number_of_rows; i < this->rows(); ++i)
                {
                    for(int64_t j = old_number_of_columns; j < this->columns(); ++j)
                    {
                        (*this)(i,j) = initial_value;
                    }
                }
            }

            return mapping_error;
        }
    }

    mapped_file_.unmap();

    // Calculate the necessary size of the file
    // so that it can hold the matrix
    uintptr_t size_of_file = sizeof(MatrixHeader) + sizeof(MatrixFooter) + rows*columns*sizeof(DataType);

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
    std::copy(matrix_header_byte_sequence.cbegin(),
              matrix_header_byte_sequence.cend(),
              &this->get_header()->header[0]);
    this->get_header()->size_of_data_type = sizeof(DataType);
    this->get_header()->rows = rows;
    this->get_header()->columns = columns;

    // Write the Matrix Footer
    std::copy(matrix_footer_byte_sequence.cbegin(),
              matrix_footer_byte_sequence.cend(),
              &this->get_footer()->footer[0]);

    // Finally we initialize the matrix values
    this->initialize(initial_value);

    // We are done
    return mapping_error;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to load a matrix from file
//-------------------------------------------------------------------
template<typename DataType>

inline std::error_code Matrix<DataType>::load_matrix(const std::string& file_to_load_matrix_from)
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
    uintptr_t minimum_file_size_needed_to_hold_a_matrix = sizeof(MatrixHeader) + sizeof(MatrixFooter);

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
    if(!does_memory_contain_mapped_matrix(mapped_file_.data(), mapped_file_.size()))
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



#endif  // INCLUDE_MATRIX_HPP_
