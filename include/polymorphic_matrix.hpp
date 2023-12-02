#ifndef INCLUDE_POLYMORPHIC_MATRIX_HPP_
#define INCLUDE_POLYMORPHIC_MATRIX_HPP_



//-------------------------------------------------------------------
#include <memory>
#include <type_traits>
#include "base_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @file polymorphic_matrix.hpp
 * @brief Defines polymorphic wrappers for matrix-like data structures in the LazyMatrix library.
 * @namespace LazyMatrix
 *
 * This file contains classes and functions that provide a polymorphic interface
 * for working with various matrix-like data structures, allowing them to be 
 * stored and manipulated in a generic manner. It's particularly useful for 
 * applications requiring polymorphic behavior with matrices.
 */
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class PolymorphicMatrix
 * @brief Base class providing a polymorphic interface for matrix-like data.
 *
 * This class defines a generic interface for matrices, allowing for 
 * operations like accessing elements and querying dimensions. It's
 * templated to handle different data types within the matrix.
 */
//-------------------------------------------------------------------
template<typename DataType>

class PolymorphicMatrix : public BaseMatrix< PolymorphicMatrix<DataType> >
{
public:

    PolymorphicMatrix() = default;
    virtual ~PolymorphicMatrix() = default;

    virtual uintptr_t rows() const = 0;
    virtual uintptr_t columns() const = 0;
    virtual uintptr_t size() const = 0;
    


    virtual DataType at_(int64_t row, int64_t column) const = 0;
    virtual DataType& at_(int64_t row, int64_t column) = 0;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class PolymorphicMatrixWrapper
 * @brief Wrapper class that provides a polymorphic interface to a given matrix type.
 *
 * This class wraps around a specific matrix type and exposes it through
 * the PolymorphicMatrix interface. It handles both const and non-const
 * matrix types, allowing for uniform treatment in a polymorphic context.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

class PolymorphicMatrixWrapper : public PolymorphicMatrix<typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type>
{
public:

    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;

    explicit PolymorphicMatrixWrapper(MatrixType& matrix) : matrix_(matrix) {}

    uintptr_t rows() const override { return matrix_.rows(); }
    uintptr_t columns() const override { return matrix_.columns(); }
    uintptr_t size() const override { return matrix_.size(); }
    


    value_type at_(int64_t row, int64_t column) const override { return matrix_(row, column); }
    
    value_type& at_(int64_t row, int64_t column) override 
    {
        if constexpr (std::is_const_v<MatrixType>)
        {
            return zero; // Return default value for const matrix types
        }
        else
        {
            return matrix_(row, column); // Non-const matrices allow modification
        }
    }



private:

    MatrixType& matrix_;

    // Inline static member for zero value for wrapping
    // matrix objects that don't allow changing entries
    inline static value_type zero{};
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Compile time function to check if the type is a matrix expression type.
 */
//-------------------------------------------------------------------
template<typename MatrixType>

struct is_type_a_matrix< PolymorphicMatrixWrapper<MatrixType> > : std::true_type {};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Wraps a matrix in a PolymorphicMatrixWrapper and returns a shared pointer to PolymorphicMatrix.
 * 
 * @tparam MatrixType Type of the matrix to wrap.
 * @param matrix The matrix to be wrapped.
 * @return std::shared_ptr<PolymorphicMatrix<value_type>> Shared pointer to the base polymorphic type of the wrapped matrix.
 */
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix<MatrixType>{}>* = nullptr>
inline auto wrap_matrix(MatrixType& matrix)
{
    using value_type = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;

    std::shared_ptr<PolymorphicMatrix<value_type>> wrapped_matrix = std::make_shared<PolymorphicMatrixWrapper<MatrixType>>(matrix);

    return wrapped_matrix;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_POLYMORPHIC_MATRIX_HPP_
