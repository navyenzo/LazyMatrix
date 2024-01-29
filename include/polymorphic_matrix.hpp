//-------------------------------------------------------------------
/**
 * @file polymorphic_matrix.hpp
 * @brief Provides polymorphic wrappers for various matrix-like data structures in LazyMatrix.
 *
 * The polymorphic_matrix.hpp file defines classes and functions crucial for creating a polymorphic
 * interface for matrix-like data structures. It allows for uniform handling of different matrix
 * types in a polymorphic context. This file includes the definition of the `PolymorphicMatrix` base
 * class, which offers a generic matrix interface, and the `PolymorphicMatrixWrapper` class, which
 * wraps around specific matrix types and exposes them through the `PolymorphicMatrix` interface.
 * These tools are essential for applications where polymorphic behavior with matrices is required.
 * 
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_POLYMORPHIC_MATRIX_HPP_
#define INCLUDE_POLYMORPHIC_MATRIX_HPP_



//-------------------------------------------------------------------
#include <memory>
#include <type_traits>
#include "numerical_constants.hpp"
#include "base_matrix.hpp"
#include "shared_references.hpp"
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

class PolymorphicMatrix : public BaseMatrix<PolymorphicMatrix<DataType>,true>
{
public:

    using value_type = DataType;

    friend class BaseMatrix<PolymorphicMatrix<DataType>,true>;

    PolymorphicMatrix() = default;
    virtual ~PolymorphicMatrix() = default;

    virtual uintptr_t rows() const = 0;
    virtual uintptr_t columns() const = 0;
    virtual uintptr_t size() const = 0;
    virtual std::error_code resize(uintptr_t rows, uintptr_t columns) = 0;

private:

    virtual DataType const_at_(int64_t row, int64_t column) const = 0;
    virtual DataType& non_const_at_(int64_t row, int64_t column) = 0;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Compile time function to check if the type is a matrix expression type.
 */
//-------------------------------------------------------------------
template<typename DataType>
struct is_type_a_matrix< PolymorphicMatrix<DataType> > : std::true_type {};
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
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>

class PolymorphicMatrixWrapper : public PolymorphicMatrix<typename ReferenceType::value_type>
{
public:

    // Type of value that is stored in the expression
    using value_type = typename ReferenceType::value_type;

    explicit PolymorphicMatrixWrapper(ReferenceType matrix) : matrix_(matrix)
    {
    }

    uintptr_t rows() const override { return matrix_.rows(); }
    uintptr_t columns() const override { return matrix_.columns(); }
    uintptr_t size() const override { return matrix_.size(); }

    std::error_code resize(uintptr_t rows, uintptr_t columns) override
    {
        if constexpr (has_non_const_access<ReferenceType>::value)
        {
            return matrix_.resize(rows, columns);
        }
        else
        {
            return std::error_code();
        }
    }



private: // Private functions

    value_type const_at_(int64_t row, int64_t column)const override
    {
        return matrix_.circ_at(row, column);
    }

    value_type& non_const_at_(int64_t row, int64_t column) override
    {
        if constexpr (has_non_const_access<ReferenceType>::value)
        {
            return matrix_.circ_at(row,column);
        }
        else
        {
            // Since we can't return a reference to a local,
            // we return a reference to a dummy value
            return DummyValueHolder<value_type>::zero;
        }
    }
    

private: // Private variables

    ReferenceType matrix_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Compile time function to check if the type is a matrix expression type.
 */
//-------------------------------------------------------------------
template<typename ReferenceType>
struct is_type_a_matrix< PolymorphicMatrixWrapper<ReferenceType> > : std::true_type {};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Aliases for the polymorphic matrix and wrapper
//-------------------------------------------------------------------
template<typename DataType>
using Data = PolymorphicMatrix<DataType>;



template<typename ReferenceType,std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>
using SpecializedData = PolymorphicMatrixWrapper<ReferenceType>;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Wraps a matrix in a PolymorphicMatrixWrapper and returns a Shared
 *        Reference to the base PolymorphicMatrix class so that a user can
 *        store multiple different types of 2d-matrices in a single container
 * 
 * @tparam ReferenceType Type of the matrix to wrap.
 * @param matrix The matrix to be wrapped.
 * @return SharedMatrixRef wrapping the input matrix.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>
         
inline auto wrap_matrix(ReferenceType matrix)
{
    using value_type = typename ReferenceType::value_type;

    std::shared_ptr<PolymorphicMatrix<value_type>> wrapped_matrix_ptr = std::make_shared<PolymorphicMatrixWrapper<ReferenceType>>(matrix);

    return SharedMatrixRef<PolymorphicMatrix<value_type>>(wrapped_matrix_ptr);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_POLYMORPHIC_MATRIX_HPP_
