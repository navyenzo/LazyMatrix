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
    std::mutex lock;
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
class Matrix : public BaseMatrix< Matrix<DataType> >
{
public:

    // Type of value that is stored in the matrix
    using value_type = DataType;

    friend class MatrixFactory;
    friend class BaseMatrix< Matrix<DataType> >;

    /**
     * @brief Default constructor. Initializes a matrix with given rows and columns.
     * @param rows Number of rows in the matrix. Default is 0.
     * @param columns Number of columns in the matrix. Default is 0.
     * @param initial_value The initial value to fill the matrix. Default is 0.
     */
    Matrix<DataType>(int64_t rows = 0, int64_t columns = 0, const DataType& initial_value = static_cast<DataType>(0));

    /**
     * @brief Copy constructor for creating a matrix from another matrix expression.
     * @tparam MatrixType Type of the source matrix. Must satisfy is_type_a_matrix.
     * @param matrix The source matrix to be copied.
     */
    template<typename MatrixType, std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>
    Matrix<DataType>(const MatrixType& matrix);

    /**
     * @brief Shallow copy constructor. Copies the matrix structure but not the data.
     * @param matrix The source matrix for the shallow copy.
     */
    Matrix<DataType>(const Matrix<DataType>& matrix);

    /**
     * @brief Constructor to create and memory map a matrix from a file.
     * @param file_to_load_matrix_from The file path to load the matrix from.
     */
    Matrix<DataType>(const std::string& file_to_load_matrix_from);

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
    Matrix<DataType>(const dlib::matrix<DataType2, NR, NC, mem_manager, layout>& dlib_matrix);

    /**
     * @brief Constructor to create a matrix from an Eigen matrix.
     * @tparam DataType2 Data type of the Eigen matrix.
     * @param m The Eigen matrix to convert from.
     */
    template<typename DataType2>
    Matrix<DataType>(const Eigen::MatrixBase<DataType2>& m);

    /**
     * @brief Converts from an Eigen matrix to this matrix.
     * @tparam Derived The derived type of Eigen matrix.
     * @param m The Eigen matrix to convert from.
     * @return Reference to this matrix after conversion.
     */
    template <typename Derived>
    Matrix<DataType>& from_eigen_matrix(const Eigen::MatrixBase<Derived>& m);

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
     * @brief Assignment operator from a generic matrix expression.
     * @tparam MatrixType The type of the source matrix.
     * @param matrix The source matrix for assignment.
     * @return Reference to this matrix after assignment.
     */
    template<typename MatrixType>
    Matrix<DataType>& operator=(const BaseMatrix<MatrixType>& matrix);

    /**
     * @brief Assignment operator for shallow copying from another matrix.
     * @param matrix The source matrix for the shallow copy.
     * @return Reference to this matrix after shallow copying.
     */
    Matrix<DataType>& operator=(const Matrix<DataType>& matrix);

    // Typedefs and functions used to transform to eigen matrices from the eigen library
    using EigenMapType = Eigen::Map<Eigen::Matrix<DataType,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> >;
    using const_EigenMapType = Eigen::Map<Eigen::Matrix<const DataType,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> >;
    const_EigenMapType to_const_eigen_map()const;
    EigenMapType to_eigen_map();

    /**
     * @brief Performs a shallow copy of another mapped matrix into this matrix.
     * @param matrix_to_copy The matrix to be shallow copied.
     * @return Error code indicating success or failure of the operation.
     */
    std::error_code shallow_copy(const Matrix<DataType>& matrix_to_copy);

    /**
     * @brief Performs a deep copy of another mapped matrix into this matrix.
     * @param matrix_to_copy The matrix to be deep copied.
     * @return Error code indicating success or failure of the operation.
     */
    std::error_code deep_copy(const Matrix<DataType>& matrix_to_copy);

    /**
     * @brief Returns a deep copy of this matrix.
     * @return A new matrix that is a deep copy of this matrix.
     */
    Matrix<DataType> get_a_deep_copy()const;

    // Getters
    bool is_valid()const;
    uintptr_t rows()const;
    uintptr_t columns()const;
    uintptr_t capacity()const;
    uintptr_t get_mapped_file_size()const;
    const fs::path& get_filename_of_memory_mapped_file()const;

    /**
     * @brief Initializes the matrix with a specified value.
     * @param initial_value The value to initialize the matrix with.
     */
    void initialize(const DataType& initial_value);

    /**
     * @brief Creates a matrix with a given size.
     * @param rows The number of rows for the new matrix.
     * @param columns The number of columns for the new matrix.
     * @param initial_value The initial value to fill the matrix with.
     * @return Error code indicating success or failure of the operation.
     */
    std::error_code create_matrix(int64_t rows, int64_t columns, const DataType& initial_value = static_cast<DataType>(0));

    /**
     * @brief Loads a matrix from a file.
     * @param file_to_load_matrix_from The file path to load the matrix from.
     * @return Error code indicating success or failure of the operation.
     */
    std::error_code load_matrix(const std::string& file_to_load_matrix_from);

    /**
     * @brief Gets a lock for synchronized access.
     * @return Reference to the mutex lock.
     */
    std::mutex& get_lock();



private: // Private functions

    /**
     * @brief Resizes the matrix to a specified size.
     * @param rows The new number of rows.
     * @param columns The new number of columns.
     * @param initial_value The initial value to fill new elements with.
     * @return Error code indicating success or failure of the operation.
     */
    std::error_code resize_(int64_t rows, int64_t columns, const DataType& initial_value = static_cast<DataType>(0));

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

    // Functions used to get the header and footer
    // of the mapped matrix from the mapped file
    
    const MatrixHeader* get_header()const
    {
        return reinterpret_cast<const MatrixHeader*>(mapped_file_.cbegin());
    }

    MatrixHeader* get_header()
    {
        return reinterpret_cast<MatrixHeader*>(mapped_file_.begin());
    }
    
    const MatrixFooter* get_footer()const
    {
        return reinterpret_cast<const MatrixFooter*>(mapped_file_.cbegin() + sizeof(MatrixHeader) + this->size()*sizeof(DataType));
    }

    MatrixFooter* get_footer()
    {
        return reinterpret_cast<MatrixFooter*>(mapped_file_.begin() + sizeof(MatrixHeader) + this->size()*sizeof(DataType));
    }



private: // Private variables
    
    // The mapped file used as the memory
    // for this memory mapped matrix and
    // its filename
    mio::shared_mmap_sink mapped_file_;
    fs::path filename_of_memory_mapped_file_;
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

inline Matrix<DataType>::Matrix(int64_t rows, int64_t columns, const DataType& initial_value)
                                : BaseMatrix< Matrix<DataType> >()
{
    this->resize_(rows, columns, initial_value);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Copy Constructor from a generic matrix expression
//-------------------------------------------------------------------
template<typename DataType>

template<typename MatrixType, std::enable_if_t<is_type_a_matrix<MatrixType>{}>*>

inline Matrix<DataType>::Matrix(const MatrixType& matrix)
                                : BaseMatrix< Matrix<DataType> >()
{
    // First we create and initialize the matrix
    this->resize_(matrix.rows(), matrix.columns());

    // We then copy the values from the matrix
    for(int64_t i = 0; i < this->rows(); ++i)
    {
        for(int64_t j = 0; j < this->columns(); ++j)
        {
            (*this)(i,j) = matrix(i,j);
        }
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Copy constructor (We do shallow copy)
//-------------------------------------------------------------------
template<typename DataType>

inline Matrix<DataType>::Matrix(const Matrix<DataType>& matrix)
                                : BaseMatrix< Matrix<DataType> >()
{
    this->shallow_copy(matrix);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Constructor to memory map a matrix from file
//-------------------------------------------------------------------
template<typename DataType>

inline Matrix<DataType>::Matrix(const std::string& file_to_load_matrix_from)
                                : BaseMatrix< Matrix<DataType> >()
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
                                : BaseMatrix< Matrix<DataType> >()
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
    from_eigen_matrix(m);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Convert from an eigen matrix
//-------------------------------------------------------------------
template<typename DataType>

template <typename Derived>

inline Matrix<DataType>& Matrix<DataType>::from_eigen_matrix(const Eigen::MatrixBase<Derived>& m)
{
    if(this->rows() != m.rows() || this->columns() != m.cols())
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
// Assignment from a generic matrix expression
//-------------------------------------------------------------------
template<typename DataType>

template<typename MatrixType>

inline Matrix<DataType>& Matrix<DataType>::operator=(const BaseMatrix<MatrixType>& matrix)
{
    this->resize_(matrix.rows(), matrix.columns());
    
    for(int i = 0; i < this->rows(); ++i)
    {
        for(int j = 0; j < this->columns(); ++j)
        {
            (*this)(i,j) = matrix(i,j);
        }
    }
    
    return (*this);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Assignment operator (does shallow copy)
//-------------------------------------------------------------------
template<typename DataType>

inline Matrix<DataType>& Matrix<DataType>::operator=(const Matrix<DataType>& matrix)
{
    this->shallow_copy(matrix);
    return (*this);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Functions used to transform to eigen matrices from the eigen library
//-------------------------------------------------------------------
template<typename DataType>

inline typename Matrix<DataType>::const_EigenMapType Matrix<DataType>::to_const_eigen_map()const
{
    return const_EigenMapType(&(*this)(0,0),this->rows(),this->columns());
}



template<typename DataType>

inline typename Matrix<DataType>::EigenMapType Matrix<DataType>::to_eigen_map()
{
    return EigenMapType(&(*this)(0,0),this->rows(),this->columns());
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Shallow copy of another mapped matrix into this matrix.
//-------------------------------------------------------------------
template<typename DataType>

inline std::error_code Matrix<DataType>::shallow_copy(const Matrix<DataType>& matrix_to_copy)
{
    filename_of_memory_mapped_file_ = matrix_to_copy.get_filename_of_memory_mapped_file();

    std::error_code mapping_error;
    mapped_file_.map(filename_of_memory_mapped_file_.string(), mapping_error);

    return mapping_error;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Deep copy of another mapped matrix into this matrix.
//-------------------------------------------------------------------
template<typename DataType>

inline std::error_code Matrix<DataType>::deep_copy(const Matrix<DataType>& matrix_to_copy)
{
    std::error_code error;

    error = this->resize_(matrix_to_copy.rows(), matrix_to_copy.columns());

    if(error)
        return error;
    
    for(int64_t i = 0; i < rows(); ++i)
    {
        for(int64_t j = 0; j < columns(); ++j)
        {
            (*this)(i,j) = matrix_to_copy(i,j);
        }
    }

    return error;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Return a deep copy of this matrix
//-------------------------------------------------------------------
template<typename DataType>

inline Matrix<DataType> Matrix<DataType>::get_a_deep_copy()const
{
    Matrix<DataType> copy_of_this_matrix;
    copy_of_this_matrix.deep_copy(*this);
    return copy_of_this_matrix;
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
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Get a lock
//-------------------------------------------------------------------
template<typename DataType>

inline std::mutex& Matrix<DataType>::get_lock()
{
    return this->get_header()->lock;
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

inline std::error_code Matrix<DataType>::resize_(int64_t rows, int64_t columns, const DataType& initial_value)
{
    return this->create_matrix(rows, columns, initial_value);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to create a matrix
//-------------------------------------------------------------------
template<typename DataType>

inline std::error_code Matrix<DataType>::create_matrix(int64_t rows,
                                                       int64_t columns,
                                                       const DataType& initial_value)
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
                this->get_header()->rows = rows;
                this->get_header()->columns = columns;

                this->initialize(initial_value);
            }

            return mapping_error;
        }
    }

    mapped_file_.unmap();

    // Calculate the necessary size of the file
    // so that it can hold the matrix
    std::size_t size_of_file = sizeof(MatrixHeader) + sizeof(MatrixFooter) + rows*columns*sizeof(DataType);

    // We attempt to create the temp file for the matrix
    filename_of_memory_mapped_file_ = LazyMatrix::create_file_with_specified_size_and_unique_name(size_of_file, mapping_error, "matrix_XXXXXX", fs::temp_directory_path());

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

    // Write the Matris Footer
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
