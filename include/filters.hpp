//-------------------------------------------------------------------
/**
 * @file filters.hpp
 * @brief Provides filtering functionality for matrices.
 *
 * This file is part of the LazyMatrix library and includes functions to create 
 * various filter kernels (like Gaussian and Laplacian) and to apply these filters
 * to matrices. It is particularly useful in image processing and computer vision 
 * applications.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_FILTERS_HPP_
#define INCLUDE_FILTERS_HPP_



//-------------------------------------------------------------------
#include <time.h>
#include <cstdint>

#include "numerical_constants.hpp"
#include "border_functor_view.hpp"
#include "matrix_factory.hpp"
#include "shared_references.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Creates a Gaussian kernel of a specified size.
 * 
 * This function generates a Gaussian kernel, which is useful for smoothing images. 
 * The Gaussian kernel is a square matrix with dimensions based on the specified kernel size.
 * 
 * @param kernel_size The size of the kernel (should be odd).
 * @param sigma The standard deviation of the Gaussian distribution.
 * @return Matrix<double> A Gaussian kernel matrix.
 */
//-------------------------------------------------------------------
inline auto create_gaussian_kernel(uintptr_t kernel_size, double sigma)
{
    uintptr_t actual_kernel_size = (kernel_size / 2) * 2 + 1;
    uintptr_t half_kernel_size = (kernel_size / 2);

    auto kernel = MatrixFactory::create_simple_matrix<double>(actual_kernel_size, actual_kernel_size, 0);

    if(sigma <= 0)
        sigma = 0.3*((actual_kernel_size - 1)*0.5 - 1) + 0.8;

    double r = 0.0;
    double s = 2.0 * sigma * sigma;

    double sum = 0.0;

    for(int row = -half_kernel_size; row <= half_kernel_size; row++)
    {
        for(int column = -half_kernel_size; column <= half_kernel_size; column++)
        {
            r = std::sqrt(row * row + column * column);

            kernel(row + half_kernel_size, column + half_kernel_size) = (std::exp(-(r * r) / s)) / (LazyMatrix::PI * s);

            sum += kernel(row + half_kernel_size, column + half_kernel_size);
        }
    }

    for(int i = 0; i < kernel.rows(); ++i)
        for(int j = 0; j < kernel.columns(); ++j)
            kernel(i,j) /= sum;

    return kernel;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Creates a Laplacian kernel.
 * 
 * This function generates a Laplacian kernel, used for edge detection in images. 
 * The Laplacian kernel is a 3x3 matrix with specific values to highlight areas of 
 * rapid intensity change.
 * 
 * @tparam DataType The data type of the matrix elements.
 * @return Matrix<DataType> A Laplacian kernel matrix.
 */
//-------------------------------------------------------------------
template<typename DataType>

inline auto create_laplacian_kernel()
{
    auto laplacian_kernel = MatrixFactory::create_simple_matrix<DataType>(3,3,0);

    laplacian_kernel(0,1) = DataType(1);
    laplacian_kernel(1,0) = DataType(1);
    laplacian_kernel(1,1) = DataType(-4);
    laplacian_kernel(1,2) = DataType(1);
    laplacian_kernel(2,1) = DataType(1);

    return laplacian_kernel;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Applies a filter kernel to a source matrix.
 * @tparam ReferenceType1 The type of the source matrix.
 * @tparam ReferenceType2 The type of the filter kernel.
 * @param source_matrix Shared reference to the source matrix.
 * @param filter_kernel Shared reference to the filter kernel.
 * @return A SharedMatrixRef to the SimpleMatrix (the filtered matrix).
 */
//-------------------------------------------------------------------
template<typename ReferenceType1, typename ReferenceType2,
            std::enable_if_t<is_matrix_reference<ReferenceType1>{}>* = nullptr,
            std::enable_if_t<is_matrix_reference<ReferenceType2>{}>* = nullptr>

inline auto filter(const ReferenceType1& source_matrix,
                   ReferenceType2 filter_kernel)
{
    using value_type = typename ReferenceType1::value_type;

    auto source_matrix_with_border = repeated_border(source_matrix);

    int64_t half_kernel_size = filter_kernel.rows() / 2;

    int64_t rows = source_matrix.rows();
    int64_t columns = source_matrix.columns();

    auto filtered_output = MatrixFactory::create_simple_matrix<value_type>(rows, columns, 0);

    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < columns; ++j)
        {
            for(int ki = 0; ki < filter_kernel.rows(); ++ki)
            {
                for(int kj = 0; kj < filter_kernel.columns(); ++kj)
                {
                    filtered_output(i,j) += filter_kernel(ki,kj) * source_matrix_with_border(i + ki - half_kernel_size, j + kj - half_kernel_size);
                }
            }
        }
    }

    return filtered_output;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_FILTERS_HPP_
