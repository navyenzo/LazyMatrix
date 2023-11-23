#ifndef INCLUDE_FILTERS_HPP_
#define INCLUDE_FILTERS_HPP_



//-------------------------------------------------------------------
#include <time.h>
#include <cstdint>

#include "numerical_constants.hpp"
#include "border_functor.hpp"
#include "matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Creates a Gaussian Kernel of a user specified size
//-------------------------------------------------------------------
inline auto create_gaussian_kernel(int64_t kernel_size, double sigma)
{
    int64_t actual_kernel_size = (kernel_size / 2) * 2 + 1;
    int64_t half_kernel_size = (kernel_size / 2);

    auto kernel = LazyMatrix::Matrix<double>(actual_kernel_size, actual_kernel_size, 0);

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
// The Laplacian kernel
//-------------------------------------------------------------------
template<typename DataType>

inline auto create_laplacian_kernel()
{
    auto laplacian_kernel = LazyMatrix::Matrix<DataType>(3,3);

    laplacian_kernel(0,1) = DataType(1);
    laplacian_kernel(1,0) = DataType(1);
    laplacian_kernel(1,1) = DataType(-4);
    laplacian_kernel(1,2) = DataType(1);
    laplacian_kernel(2,1) = DataType(1);

    return laplacian_kernel;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to filter an image using a provided filter
//-------------------------------------------------------------------
template<typename MatrixType1,
         typename MatrixType2,
         std::enable_if_t<is_type_a_matrix<MatrixType1>{}>* = nullptr,
         std::enable_if_t<is_type_a_matrix<MatrixType2>{}>* = nullptr>

inline auto filter(const MatrixType1& source_matrix, const MatrixType2& filter_kernel)
{
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType1>()(0,0))>::type;

    auto source_matrix_with_border = repeated_border(source_matrix);

    int64_t half_kernel_size = filter_kernel.rows() / 2;

    int64_t rows = source_matrix.rows();
    int64_t columns = source_matrix.columns();

    LazyMatrix::Matrix<value_type> filtered_output(rows, columns, 0);

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
