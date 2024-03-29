//-------------------------------------------------------------------
/**
 * @file lazy_matrix.hpp
 * @brief Main include file for the LazyMatrix library.
 *
 * The LazyMatrix library offers a comprehensive suite for advanced matrix operations, 
 * including support for lazy evaluation, memory-mapped matrices, and various 
 * transformations. This header file is the central include point for the entire library, 
 * facilitating easy integration into projects. Including this file provides access to all 
 * functionalities of the LazyMatrix library.
 *
 * Usage: Simply include 'lazy_matrix.hpp' in your project to utilize the library.
 *
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_LAZY_MATRIX_HPP_
#define INCLUDE_LAZY_MATRIX_HPP_



//-------------------------------------------------------------------
// File and directory manipulation utilities
#include "files.hpp"

// Robust mutex for thread synchronization
#include "robust_mutex.hpp"

// Constants for numerical computations
#include "numerical_constants.hpp"

// Utilities for converting strings to numbers and expressions
#include "convert_numbers.hpp"

// Circular iterator for cyclic traversing
#include "circular_iterator.hpp"

// Page, Row and Column Header naming
#include "row_and_column_headers.hpp"
#include "page_and_row_and_column_headers.hpp"

// Base matrix class for 2D matrices
#include "base_matrix.hpp"

// Base matrix class for 3D matrices
#include "base_matrix3d.hpp"

// Shared Reference to 2d and 3d Matrix expressions
#include "shared_references.hpp"

// Custom eigen matrix wrappers for LazyMatrix matrix expressions
#include "eigen_wrappers.hpp"

// Wrapper for 2D matrices with simple data storage allowing modification
#include "simple_data_2d_matrix_wrapper.hpp"

// Wrapper for 3D matrices with simple
#include "simple_data_3d_matrix_wrapper.hpp"

// Polymorphic matrix for dynamic type handling
#include "polymorphic_matrix.hpp"

// Polymorphic 3d matrix for dynamic type handling
#include "polymorphic_matrix3d.hpp"

// 2D matrix storage using std::vector for storage
#include "simple_matrix.hpp"

// 3D matrix storage using std::vector for storage
#include "simple_matrix3d.hpp"

// 2D matrix with memory-mapped file storage
#include "matrix.hpp"

// 3D matrix with memory-mapped file storage
#include "matrix3d.hpp"

// Matrix representation of CSV files
#include "csv_matrix.hpp"

// Interval data structure for numerical ranges
#include "interval.hpp"

// Matrix representation of images
#include "image_matrix.hpp"

// Transpose operation view without modifying original data
#include "transpose_view.hpp"

// Reverse operation view without modifying original data
#include "reverse_view.hpp"

// View for row augmentation allowing modification
#include "augment_rows_view.hpp"

// View for column augmentation allowing modification
#include "augment_columns_view.hpp"

// View applying border conditions allowing modification
#include "border_functor_view.hpp"

// View for padding a matrix expression
#include "padding_view.hpp"

// View for ROI allowing modification
#include "roi_view.hpp"

// View for selector operation allowing modification
#include "selector_view.hpp"

// Sorted view of matrix expressions
// - Sort rows or columns using a user specified row or column
#include "sorting_view.hpp"

// View for shuffle operation allowing modification
#include "shuffle_view.hpp"

// Moving average calculation for matrices
#include "moving_average.hpp"

// Generators for creating specific types of matrices
#include "matrix_generators.hpp"

// Matrix multiplication operations
#include "matrix_multiplication.hpp"

// Differential operation for matrices
#include "diff.hpp"

// Filters for processing matrix data
#include "filters.hpp"

// Fast Fourier Transform (FFT) operations
#include "fft.hpp"

// Binary expression templates for element-wise operations
#include "element_by_element_binary_expression.hpp"

// Unary expression templates for element-wise operations
#include "element_by_element_unary_expression.hpp"

// Database matrix views
#include "database_matrix.hpp"

// Downsampling algorithms
#include "downsampling.hpp"

// Factory functions to create 2d and 3d Matrix storage
#include "matrix_factory.hpp"

// Matrix container holding 2d matrices and looks like a 3d matrix
#include "polymorphic_matrix_container.hpp"

// Output operators for matrix serialization
#include "output_operators.hpp"

// Strassen algorithm for matrix multiplication
#include "strassen_matrix_multiplication.hpp"

// Python bindings for matrices
#include "python_bindings.hpp"
//-------------------------------------------------------------------



#endif  // INCLUDE_LAZY_MATRIX_HPP_
