//-------------------------------------------------------------------
/**
 * @file image_matrix.hpp
 * @brief Provides the ImageMatrix class to handle images with pixel-level manipulation.
 *
 * This file contains the ImageMatrix class which is a wrapper around dlib's image processing
 * capabilities. It allows for the loading, manipulation, and querying of images in a matrix
 * format. The class is designed to be flexible with various pixel types including standard
 * color formats and grayscale. It relies on dlib's image processing tools and thus requires
 * pixel types that are compatible with dlib.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_IMAGE_MATRIX_HPP_
#define INCLUDE_IMAGE_MATRIX_HPP_



//-------------------------------------------------------------------
#include <dlib/matrix.h>
#include <dlib/image_io.h>
#include <dlib/image_processing.h>
#include <dlib/pixel.h>

#include "base_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
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
 * @brief Trait to check if a type is a valid pixel type for use with dlib.
 *
 * This trait ensures that only pixel types supported by dlib are used with ImageMatrix.
 * It includes specializations for dlib's pixel types and common grayscale formats.
 */
//-------------------------------------------------------------------
template<typename T>
struct is_valid_dlib_pixel_type : std::false_type {};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Specialize for dlib's provided pixel types
template<> struct is_valid_dlib_pixel_type<dlib::rgb_pixel> : std::true_type {};
template<> struct is_valid_dlib_pixel_type<dlib::bgr_pixel> : std::true_type {};
template<> struct is_valid_dlib_pixel_type<dlib::rgb_alpha_pixel> : std::true_type {};
template<> struct is_valid_dlib_pixel_type<dlib::bgr_alpha_pixel> : std::true_type {};
template<> struct is_valid_dlib_pixel_type<dlib::hsi_pixel> : std::true_type {};
//template<> struct is_valid_dlib_pixel_type<dlib::hsv_pixel> : std::true_type {};
template<> struct is_valid_dlib_pixel_type<dlib::lab_pixel> : std::true_type {};

// Specialize for common grayscale types
template<> struct is_valid_dlib_pixel_type<unsigned char> : std::true_type {};  // 8-bit grayscale
template<> struct is_valid_dlib_pixel_type<unsigned short> : std::true_type {}; // 16-bit grayscale
template<> struct is_valid_dlib_pixel_type<unsigned int> : std::true_type {};   // 32-bit grayscale
template<> struct is_valid_dlib_pixel_type<float> : std::true_type {};          // Floating-point grayscale
template<> struct is_valid_dlib_pixel_type<double> : std::true_type {};         // Double precision floating-point grayscale
template<> struct is_valid_dlib_pixel_type<std::complex<float>> : std::true_type {}; // Complex number for FFT, etc.
template<> struct is_valid_dlib_pixel_type<std::complex<double>> : std::true_type {}; // Complex number for FFT, etc.

// Specialize for signed grayscale types if necessary
template<> struct is_valid_dlib_pixel_type<signed char> : std::true_type {};    
template<> struct is_valid_dlib_pixel_type<short> : std::true_type {};          
template<> struct is_valid_dlib_pixel_type<int> : std::true_type {};            
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename PixelType>

inline PixelType get_default_pixel_value()
{
    if constexpr (std::is_same_v<PixelType, dlib::rgb_pixel>)
    {
        return dlib::rgb_pixel(0, 0, 0); // Black pixel for RGB
    }
    else if constexpr (std::is_same_v<PixelType, dlib::bgr_pixel>)
    {
        return dlib::bgr_pixel(0, 0, 0); // Black pixel for BGR
    }
    else if constexpr (std::is_same_v<PixelType, dlib::rgb_alpha_pixel>)
    {
        return dlib::rgb_alpha_pixel(0, 0, 0, 0); // Transparent black pixel for RGBA
    }
    else if constexpr (std::is_same_v<PixelType, dlib::bgr_alpha_pixel>)
    {
        return dlib::bgr_alpha_pixel(0, 0, 0, 0); // Transparent black pixel for BGRA
    }
    else if constexpr (std::is_same_v<PixelType, dlib::hsi_pixel>)
    {
        return dlib::hsi_pixel(0, 0, 0); // Black pixel in HSI color space
    }
    else if constexpr (std::is_same_v<PixelType, dlib::lab_pixel>)
    {
        return dlib::lab_pixel(0, 0, 0); // Black pixel in LAB color space
    }
    else if constexpr (std::is_integral_v<PixelType> || std::is_floating_point_v<PixelType>)
    {
        return PixelType(0); // Zero for grayscale (int, float, double)
    }
    else if constexpr (std::is_same_v<PixelType, std::complex<float>> || std::is_same_v<PixelType, std::complex<double>>)
    {
        return PixelType(0, 0); // Zero for complex numbers
    }
    else
    {
        // Fallback for any non-specialized types, might not be valid for all cases
        return PixelType{};
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief A class that represents an image matrix, serving as an interface to dlib's image handling.
 * 
 * Allowed pixel types (from dlib):
 * - dlib::rgb_pixel
 * - dlib::rgb_alpha_pixel
 * - dlib::bgr_pixel
 * - unsigned char (for grayscale)
 * - uint16 (for 16-bit grayscale)
 * - float (for floating-point grayscale)
 * - double (for floating-point grayscale)
 * - dlib::hsi_pixel
 * - dlib::lab_pixel
 * - dlib::rgb_pixel_32
 * - dlib::hsi_pixel_32
 *
 * @tparam PixelType The pixel type of the image. Must be one of the allowed types.
 */
//-------------------------------------------------------------------
template<typename PixelType = dlib::rgb_pixel>
class ImageMatrix : public BaseMatrix<ImageMatrix<PixelType>,true>
{
public:

    // Type of value that is stored in the matrix
    using value_type = PixelType;

    friend class MatrixFactory;

    friend BaseMatrix<ImageMatrix<PixelType>,true>;

    /**
     * @brief Static assertion to ensure the pixel type is valid.
     *
     * This static assertion checks at compile time if the PixelType used with
     * ImageMatrix is one of the supported types defined in is_valid_dlib_pixel_type.
     * If an invalid type is used, the compilation will fail, providing an error
     * message indicating the misuse.
     */
    static_assert(is_valid_dlib_pixel_type<PixelType>::value, "Invalid pixel type for ImageMatrix. Please use a valid dlib pixel type.");
    
    /**
     * @brief Default constructor. Initializes an image matrix with given rows and columns.
     * @param rows Number of rows in the matrix. Default is 0.
     * @param columns Number of columns in the matrix. Default is 0.
     * @param initial_value The initial value to fill the matrix. Default is 0.
     */
    explicit ImageMatrix(uintptr_t rows = 0, uintptr_t columns = 0, const PixelType& initial_value = get_default_pixel_value<PixelType>())
    {
        image_data_.set_size(rows, columns);
        image_data_ = initial_value;
    }

    /**
     * @brief Constructs an ImageMatrix by loading an image from the given file path.
     * 
     * @param filename The path to the image file.
     */
    explicit ImageMatrix(const std::string& filename)
    {
        dlib::load_image(image_data_, filename);
    }

    /**
     * @brief Returns the number of rows in the image.
     * 
     * @return int64_t The number of rows.
     */
    uintptr_t rows() const
    {
        return image_data_.nr();
    }

    /**
     * @brief Returns the number of columns in the image.
     * 
     * @return int64_t The number of columns.
     */
    uintptr_t columns() const
    {
        return image_data_.nc();
    }

    // Functions used to handle row and column header names
    std::string get_row_header(int64_t row_index) const { return this->headers_.get_row_header(row_index); }
    std::string get_column_header(int64_t column_index) const { return this->headers_.get_column_header(column_index); }
    void set_row_header(int64_t row_index, const std::string& row_header) const { this->headers_.set_row_header(row_index, row_header); }
    void set_column_header(int64_t column_index, const std::string& column_header) const { this->headers_.set_column_header(column_index, column_header); }



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
     * @brief Gets the pixel at the specified row and column.
     * 
     * @param row The row index.
     * @param column The column index.
     * @return PixelType The pixel at the specified location.
     */
    PixelType const_at_(int64_t row, int64_t column) const
    {
        return image_data_(row, column);
    }

    /**
     * @brief Gets a reference to the pixel at the specified row and column.
     * 
     * @param row The row index.
     * @param column The column index.
     * @return PixelType The pixel at the specified location.
     */
    PixelType& non_const_at_(int64_t row, int64_t column)
    {
        return image_data_(row, column);
    }



private: // Private variables

    dlib::matrix<PixelType> image_data_;  ///< The underlying dlib matrix storing the image data.
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename DataType>

struct is_type_a_matrix< ImageMatrix<DataType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
}  // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_IMAGE_MATRIX_HPP_