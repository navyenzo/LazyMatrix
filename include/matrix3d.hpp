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
#include "base_matrix3d.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Header and Footer section of a multi-dimentional memory mapped matrix
//-------------------------------------------------------------------
const std::string matrix3d_header_byte_sequence = "---begin_3d_---\n";
const std::string matrix3d_footer_byte_sequence = ":---end_3d_---:\n";

struct Matrix3DHeader
{
    char header[16] = {'-', '-', '-', 'b', 'e', 'g', 'i', 'n', '_', '3', 'd', '_', '-', '-', '-', '\n'};
    std::mutex lock;
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
// Function used to check whether a memory section does indeed
// contain a memory mapped matrix
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
// Defines a Matrix, stores matrix values, forces an expresssion to
// be evaluated
//-------------------------------------------------------------------
template<typename DataType>
class Matrix3D : public BaseMatrix3D<Matrix3D<DataType>,
                                     DataType,
                                     true>
{
public:

    // Type of value that is stored in the matrix
    using value_type = DataType;

    friend class MatrixFactory;

    friend class BaseMatrix3D<Matrix3D<DataType>,
                              DataType,
                              true>;

    // Copy constructor (We do shallow copy)
    Matrix3D(const Matrix3D<DataType>& matrix)
    {
        this->shallow_copy(matrix);
    }

    // Constructor from rows, columns and initial value
    Matrix3D(uintptr_t pages = 0, uintptr_t rows = 0, uintptr_t columns = 0, const DataType& initial_value = static_cast<DataType>(0))
    {
        this->resize_(pages, rows, columns, initial_value);
    }

    // Constructor to memory map a matrix from file
    Matrix3D(const std::string& file_to_load_matrix_from)
    {
        this->load_matrix(file_to_load_matrix_from);
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
        return const_EigenMapType(&(*this)(0,0),this->rows(),this->columns());
    }

    EigenMapType to_eigen_map()
    {
        return EigenMapType(&(*this)(0,0),this->rows(),this->columns());
    }



    // Carry out a shallow copy of another mapped matrix
    std::error_code shallow_copy(const Matrix<DataType>& matrix_to_copy)
    {
        filename_of_memory_mapped_file_ = matrix_to_copy.get_filename_of_memory_mapped_file();

        std::error_code mapping_error;
        mapped_file_.map(filename_of_memory_mapped_file_, mapping_error);

        return mapping_error;
    }



    // Carry out a deep copy of another mapped matrix
    std::error_code deep_copy(const Matrix<DataType>& matrix_to_copy)
    {
        std::error_code error;

        error = this->resize_(matrix_to_copy.pages(), matrix_to_copy.rows(), matrix_to_copy.columns());

        for(int64_t i = 0; i < this->pages(); ++i)
        {
            for(int64_t j = 0; j < this->rows(); ++j)
            {
                for(int64_t k = 0; k < this->columns(); ++k)
                {
                    (*this)(i,j,k) = matrix_to_copy(i,j,k);
                }
            }
        }

        return error;
    }



    // Return a deep copy of this matrix
    Matrix3D<DataType> get_a_deep_copy()const
    {
        Matrix3D<DataType> copy_of_this_matrix;
        copy_of_this_matrix.deep_copy(*this);
        return copy_of_this_matrix;
    }



    bool is_valid()const
    {
        return mapped_file_.is_open();
    }



    uintptr_t pages()const
    {
        return this->get_header()->pages;
    }

    uintptr_t rows()const
    {
        return this->get_header()->rows;
    }

    uintptr_t columns()const
    {
        return this->get_header()->columns;
    }



    uintptr_t capacity()const
    {
        return (get_mapped_file_size() - sizeof(Matrix3DHeader) + sizeof(Matrix3DFooter)) / sizeof(DataType);
    }



    std::size_t get_mapped_file_size()const
    {
        return mapped_file_.size();
    }



    void initialize(const DataType& initial_value)
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



    std::error_code create_matrix(uintptr_t pages,
                                  uintptr_t rows,
                                  uintptr_t columns,
                                  const DataType& initial_value = static_cast<DataType>(0),
                                  const std::string& matrix_filename = "");

    std::error_code load_matrix(const std::string& file_to_load_matrix_from);



    std::mutex& get_lock()
    {
        return this->get_header()->lock;
    }



    const std::string& get_filename_of_memory_mapped_file()const
    {
        return filename_of_memory_mapped_file_;
    }



private: // Private functions

    std::error_code resize_(uintptr_t pages,
                            uintptr_t rows,
                            uintptr_t columns,
                            const DataType& initial_value = static_cast<DataType>(0))
    {
        return this->create_matrix(pages, rows, columns, initial_value);
    }

    DataType const_at_(int64_t page, int64_t row, int64_t column)const
    {
        return reinterpret_cast<const DataType*>(mapped_file_.cbegin() + sizeof(Matrix3DHeader))[page*rows()*columns() + row*columns() + column];
    }

    DataType& non_const_at_(int64_t page, int64_t row, int64_t column)
    {
        return reinterpret_cast<DataType*>(mapped_file_.begin() + sizeof(Matrix3DHeader))[page*rows()*columns() + row*columns() + column];
    }

    // Functions used to get the header and footer
    // of the mapped matrix from the mapped file
    
    const Matrix3DHeader* get_header()const
    {
        return reinterpret_cast<const Matrix3DHeader*>(mapped_file_.cbegin());
    }

    Matrix3DHeader* get_header()
    {
        return reinterpret_cast<Matrix3DHeader*>(mapped_file_.begin());
    }
    
    const Matrix3DFooter* get_footer()const
    {
        return reinterpret_cast<const Matrix3DFooter*>(mapped_file_.cbegin() + sizeof(Matrix3DHeader) + this->size()*sizeof(DataType));
    }

    Matrix3DFooter* get_footer()
    {
        return reinterpret_cast<Matrix3DFooter*>(mapped_file_.begin() + sizeof(Matrix3DHeader) + this->size()*sizeof(DataType));
    }



private: // Private variables
    
    // The mapped file used as the memory
    // for this memory mapped matrix and
    // its filename

    mio::shared_mmap_sink mapped_file_;
    std::string filename_of_memory_mapped_file_;
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
// Function used to create a matrix
//-------------------------------------------------------------------
template<typename DataType>

inline std::error_code Matrix3D<DataType>::create_matrix(uintptr_t pages,
                                                         uintptr_t rows,
                                                         uintptr_t columns,
                                                         const DataType& initial_value,
                                                         const std::string& matrix_filename)
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
                this->get_header()->pages = pages;
                this->get_header()->rows = rows;
                this->get_header()->columns = columns;
                
                this->initialize(initial_value);
            }

            return mapping_error;
        }
    }

    mapped_file_.unmap();

    filename_of_memory_mapped_file_ = matrix_filename;

    // If the user specified a filename, we attempt
    // to create the matrix using that filename, otherwise
    // we create the matrix using a unique name in the temp
    // directory
    if(filename_of_memory_mapped_file_.empty())
    {
        // The user did not specify a filename, so
        // we get a temporary filename
        filename_of_memory_mapped_file_ = std::tmpnam(nullptr);
    }

    // Calculate the necessary size of the file
    // so that it can hold the matrix
    uintptr_t size_of_file = sizeof(Matrix3DHeader) + sizeof(Matrix3DFooter) + pages*rows*columns*sizeof(DataType);

    // Create the file
    std::fstream file_to_create(filename_of_memory_mapped_file_,std::fstream::in | std::fstream::out | std::fstream::trunc);

    if(!file_to_create.is_open())
    {
        mapping_error.assign(1,std::iostream_category());
        return mapping_error;
    }
    
    file_to_create.close();

    // Resize the file to the correct size
    fs::resize_file(filename_of_memory_mapped_file_, size_of_file, mapping_error);

    if(mapping_error)
        return mapping_error;

    // Finally we memory map the entire file
    mapped_file_.map(filename_of_memory_mapped_file_, mapping_error);

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
// Function used to load a matrix from file
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
    mapped_file_.map(filename_of_memory_mapped_file_, mapping_error);

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
