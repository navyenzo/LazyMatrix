//-------------------------------------------------------------------
/**
 * @file fft.hpp
 * @brief Provides Fast Fourier Transform (FFT) functionalities for matrices.
 *
 * This file is part of the LazyMatrix library. It includes functions to perform 
 * FFT and related transformations on matrices. The implementation leverages 
 * Eigen library's FFT capabilities to provide efficient and accurate 
 * frequency domain transformations.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_FFT_HPP_
#define INCLUDE_FFT_HPP_



//-------------------------------------------------------------------
#include <complex>
#include "Eigen/Eigen"
#include "Eigen/Dense"
#include "unsupported/Eigen/FFT"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyMatrix
//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Computes the moving FFT of a matrix.
 * 
 * This function applies a moving window FFT to a matrix of data, typically used 
 * for analyzing time series or signal data. The FFT is computed for each window, 
 * and the results are stored in a matrix. Overlapping windows can be specified 
 * for more continuous analysis.
 * 
 * @tparam Scalar The data type of the matrix elements (typically double or float).
 * @param data The input matrix of data.
 * @param result The output matrix where FFT results will be stored.
 * @param window_duration_sec The duration of the time window in seconds.
 * @param sampling_period_sec The sampling period or frequency of the data in seconds.
 * @param num_overlap_steps The number of overlap steps between successive windows.
 */
//-------------------------------------------------------------------
template<typename Scalar>

inline void fft_moving_window(const Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>& data, // Input matrix of data
                              Eigen::Matrix<std::complex<Scalar>, Eigen::Dynamic, Eigen::Dynamic>& result, // Output matrix of FFT results
                              const double window_duration_sec, // Duration of the time window in seconds
                              const double sampling_period_sec, // Sampling period or frequency of the data in seconds
                              const int num_overlap_steps) // Number of overlap steps between successive windows
{
    // Create an FFT object for computing the FFT
    Eigen::FFT<typename Eigen::Matrix<Scalar, Eigen::Dynamic, 1>::Index> fft;

    // Compute the size of the time window and the number of steps for the moving FFT
    const int window_size = static_cast<int>(window_duration_sec / sampling_period_sec);
    const int num_channels = data.rows();
    const int num_samples = data.cols();
    const int num_steps = (num_samples - window_size) / (window_size - num_overlap_steps) + 1;

    // Resize the output matrix to hold the FFT results
    result.resize(num_channels, window_size / 2 + 1);

    // Loop over each channel in the input data
    for (int i = 0; i < num_channels; ++i)
    {
        // Loop over each time window in the data
        for (int j = 0; j < num_steps; ++j)
        {
            // Extract the current window of data from the input matrix
            Eigen::Matrix<Scalar, Eigen::Dynamic, 1> window = data.block(i, j * (window_size - num_overlap_steps), 1, window_size).transpose();

            // Compute the FFT of the window
            auto fft_result = fft.fwd(window);

            // Copy the non-redundant half of the FFT output to the output matrix
            for (int k = 0; k < window_size / 2 + 1; ++k)
            {
                result(i, k) = fft_result(k);
            }
        }
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_FFT_HPP_
