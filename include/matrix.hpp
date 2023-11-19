#ifndef INCLUDE_MATRIX_HPP_
#define INCLUDE_MATRIX_HPP_



//-------------------------------------------------------------------
#include <vector>
#include <fstream>
#include <mutex>

#include "files.hpp"

#include "base_matrix.hpp"

// mio library for cross-platform memory-mapping
#include "single_include/mio/mio.hpp"

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
// Header and Footer section of a memory mapped matrix
//-------------------------------------------------------------------
const std::string matrix_header_byte_sequence = "::---begin---::\n";
const std::string matrix_footer_byte_sequence = "::----end----::\n";

struct MatrixHeader
{
    char header[16] = {':', ':', '-', '-', '-', 'b', 'e', 'g', 'i', 'n', '-', '-', '-', ':', ':', '\n'};
    std::mutex lock;
    int size_of_data_type = 8;
    int rows = 0;
    int columns = 0;
};

struct MatrixFooter
{
    char footer[16] = {':', ':', '-', '-', '-', '-', 'e', 'n', 'd', '-', '-', '-', '-', ':', ':', '\n'};
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to check whether a memory section does indeed
// contain a memory mapped matrix
//-------------------------------------------------------------------
inline bool does_memory_contain_mapped_matrix(const char* memory_mapped_matrix,
                                              int memory_size_in_bytes)
{
    int minimum_size = sizeof(MatrixHeader) + sizeof(MatrixFooter);

    if(memory_size_in_bytes < minimum_size)
        return false;
    
    const MatrixHeader* header = reinterpret_cast<const MatrixHeader*>(memory_mapped_matrix);

    int expected_size = minimum_size + header->size_of_data_type * header->rows * header->columns;

    return memory_size_in_bytes >= expected_size;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Defines a Matrix that lives in a memory mapped file, which can
// be shared across multiple processes/threads
//-------------------------------------------------------------------
template<typename DataType>
class Matrix : public BaseMatrix< Matrix<DataType> >
{
public:

    // Constructor from a generic matrix expression
    template<typename MatrixType,
             std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>

    Matrix<DataType>(const MatrixType& matrix)
    : BaseMatrix< Matrix<DataType> >()
    {
        // First we create and initialize the matrix
        this->resize(matrix.rows(), matrix.columns());

        // We then copy the values from the matrix
        for(int i = 0; i < this->rows(); ++i)
        {
            for(int j = 0; j < this->columns(); ++j)
            {
                this->at(i,j) = matrix(i,j);
            }
        }
    }

    // Copy constructor (We do shallow copy)
    Matrix<DataType>(const Matrix<DataType>& matrix)
    {
        this->shallow_copy(matrix);
    }

    // Constructor from rows, columns and initial value
    Matrix<DataType>(int rows = 0, int columns = 0, const DataType& initial_value = static_cast<DataType>(0))
    : BaseMatrix< Matrix<DataType> >()
    {
        resize(rows, columns, initial_value);
    }

    // Constructor to memory map a matrix from file
    Matrix<DataType>(const std::string& file_to_load_matrix_from)
    : BaseMatrix< Matrix<DataType> >()
    {
        this->load_matrix(file_to_load_matrix_from);
    }

    // Constructor from a dlib matrix
    template<typename DataType2, long NR, long NC, typename mem_manager, typename layout>
    Matrix<DataType>(const dlib::matrix<DataType2, NR, NC, mem_manager, layout>& dlib_matrix)
    : BaseMatrix< Matrix<DataType> >()
    {
        this->resize(dlib_matrix.nr(), dlib_matrix.nc());

        for(int64_t i = 0; i < this->rows(); ++i)
        {
            for(int64_t j = 0; j < this->columns(); ++j)
            {
                this->at(i,j) = dlib_matrix(i,j);
            }
        }
    }

    // Constructor from an eigen matrix
    template<typename DataType2>
    Matrix<DataType>(const Eigen::MatrixBase<DataType2>& m)
    {
        from_eigen_matrix(m);
    }

    // Convert from an eigen matrix
    template <typename Derived>
    Matrix<DataType>& from_eigen_matrix(const Eigen::MatrixBase<Derived>& m)
    {
        if(this->rows() != m.rows() || this->columns() != m.cols())
            this->resize(m.rows(),m.cols());

        for(int64_t i = 0; i < m.rows(); ++i)
        {
            for(int64_t j = 0; j < m.cols(); ++j)
            {
                this->at(i,j) = m(i,j);
            }
        }

        return (*this);
    }

    // Assignment from a dlib matrix
    template<typename DataType2, long NR, long NC, typename mem_manager, typename layout>
    Matrix<DataType>& operator=(const dlib::matrix<DataType2, NR, NC, mem_manager, layout>& dlib_matrix)
    {
        this->resize(dlib_matrix.nr(), dlib_matrix.nc());

        for(int64_t i = 0; i < this->rows(); ++i)
        {
            for(int64_t j = 0; j < this->columns(); ++j)
            {
                this->at(i,j) = dlib_matrix(i,j);
            }
        }

        return (*this);
    }

    // Assignment from a generic matrix expression
    template<typename MatrixType>
    Matrix<DataType>& operator=(const BaseMatrix<MatrixType>& matrix)
    {
        this->resize(matrix.rows(), matrix.columns());
        
        for(int i = 0; i < this->rows(); ++i)
        {
            for(int j = 0; j < this->columns(); ++j)
            {
                this->at(i,j) = matrix(i,j);
            }
        }
        
        return (*this);
    }

    // Assignment operator (does shallow copy)
    Matrix<DataType>& operator=(const Matrix<DataType>& matrix)
    {
        this->shallow_copy(matrix);
        return (*this);
    }



    // Typedefs and functions used to transform to eigen matrices from the eigen library
    using EigenMapType = Eigen::Map<Eigen::Matrix<DataType,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> >;
    using const_EigenMapType = Eigen::Map<Eigen::Matrix<const DataType,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> >;

    const_EigenMapType to_const_eigen_map()const
    {
        return const_EigenMapType(&this->at(0,0),this->rows(),this->columns());
    }

    EigenMapType to_eigen_map()
    {
        return EigenMapType(&this->at(0,0),this->rows(),this->columns());
    }



    // Carry out a shallow copy of another mapped matrix
    std::error_code shallow_copy(const Matrix<DataType>& matrix_to_copy)
    {
        filename_of_memory_mapped_file_ = matrix_to_copy.get_filename_of_memory_mapped_file();

        std::error_code mapping_error;
        mapped_file_.map(filename_of_memory_mapped_file_.string(), mapping_error);

        return mapping_error;
    }



    // Carry out a deep copy of another mapped matrix
    std::error_code deep_copy(const Matrix<DataType>& matrix_to_copy)
    {
        std::error_code error;

        error = this->resize(matrix_to_copy.rows(), matrix_to_copy.columns());

        if(error)
            return error;
        
        for(int i = 0; i < rows(); ++i)
        {
            for(int j = 0; j < columns(); ++j)
            {
                this->at(i,j) = matrix_to_copy(i,j);
            }
        }

        return error;
    }



    // Return a deep copy of this matrix
    Matrix<DataType> get_a_deep_copy()const
    {
        Matrix<DataType> copy_of_this_matrix;
        copy_of_this_matrix.deep_copy(*this);
        return copy_of_this_matrix;
    }

    bool is_valid()const
    {
        return mapped_file_.is_open();
    }
    
    int rows()const
    {
        return this->get_header()->rows;
    }

    int columns()const
    {
        return this->get_header()->columns;
    }

    int capacity()const
    {
        int cap = (get_mapped_file_size() - sizeof(MatrixHeader) + sizeof(MatrixFooter)) / sizeof(DataType);
        return std::max(0, cap);
    }

    std::size_t get_mapped_file_size()const
    {
        return mapped_file_.size();
    }
    
    const DataType& at(int row, int column)const;
    DataType& at(int row, int column);

    void initialize(const DataType& initial_value);

    std::error_code resize(int rows, int columns, const DataType& initial_value = static_cast<DataType>(0));
    std::error_code create_matrix(int rows, int columns, const DataType& initial_value = static_cast<DataType>(0));
    std::error_code load_matrix(const std::string& file_to_load_matrix_from);

    std::mutex& get_lock()
    {
        return this->get_header()->lock;
    }

    const fs::path& get_filename_of_memory_mapped_file()const
    {
        return filename_of_memory_mapped_file_;
    }



private:

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
// Access operators
//-------------------------------------------------------------------
template<typename DataType>

inline const DataType& Matrix<DataType>::at(int row, int column)const
{
    return reinterpret_cast<const DataType*>(mapped_file_.cbegin() + sizeof(MatrixHeader))[row*columns() + column];
}



template<typename DataType>

inline DataType& Matrix<DataType>::at(int row, int column)
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
    for(int i = 0; i < this->rows(); ++i)
    {
        for(int j = 0; j < this->columns(); ++j)
        {
            this->at(i,j) = initial_value;
        }
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Alias for creating/resizing a matrix
//-------------------------------------------------------------------
template<typename DataType>

inline std::error_code Matrix<DataType>::resize(int rows, int columns, const DataType& initial_value)
{
    return this->create_matrix(rows, columns, initial_value);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to create a matrix
//-------------------------------------------------------------------
template<typename DataType>

inline std::error_code Matrix<DataType>::create_matrix(int rows,
                                                       int columns,
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
    int minimum_file_size_needed_to_hold_a_matrix = sizeof(MatrixHeader) + sizeof(MatrixFooter);

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
