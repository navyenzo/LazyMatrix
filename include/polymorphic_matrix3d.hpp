//-------------------------------------------------------------------
/**
 * @file polymorphic_matrix3d.hpp
 * @brief Provides polymorphic wrappers for various 3d-matrix-like data structures in LazyMatrix.
 *
 * The polymorphic_matrix3d.hpp file defines classes and functions crucial for creating a polymorphic
 * interface for 3d-matrix-like data structures. It allows for uniform handling of different matrix
 * types in a polymorphic context. This file includes the definition of the `PolymorphicMatrix3D` base
 * class, which offers a generic 3d-matrix interface, and the `PolymorphicMatrixWrapper3D` class, which
 * wraps around specific 3d-matrix types and exposes them through the `PolymorphicMatrix3D` interface.
 * These tools are essential for applications where polymorphic behavior with 3d-matrices is required.
 * 
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_POLYMORPHIC_MATRIX3D_HPP_
#define INCLUDE_POLYMORPHIC_MATRIX3D_HPP_



//-------------------------------------------------------------------
#include <memory>
#include <type_traits>
#include "base_matrix3d.hpp"
#include "shared_references.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class PolymorphicMatrix3D
 * @brief Base class providing a polymorphic interface for 3d-matrix-like data.
 *
 * This class defines a generic interface for 3d-matrices, allowing for 
 * operations like accessing elements and querying dimensions. It's
 * templated to handle different data types within the 3d-matrix.
 */
//-------------------------------------------------------------------
template<typename DataType>

class PolymorphicMatrix3D : public BaseMatrix3D<PolymorphicMatrix3D<DataType>,
                                                DataType,
                                                true>
{
public:

    friend class BaseMatrix3D<PolymorphicMatrix3D<DataType>,
                              DataType,
                              true>;

    PolymorphicMatrix3D() = default;
    virtual ~PolymorphicMatrix3D() = default;

    virtual uintptr_t pages() const = 0;
    virtual uintptr_t rows() const = 0;
    virtual uintptr_t columns() const = 0;
    virtual uintptr_t size() const = 0;

    virtual std::error_code resize(uintptr_t pages, uintptr_t rows, uintptr_t columns) = 0;

private:

    // Accessors for matrix elements.
    virtual DataType const_at_(int64_t page, int64_t row, int64_t column)const = 0;
    virtual DataType& non_const_at_(int64_t page, int64_t row, int64_t column) = 0;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Compile time function to check if the type is a matrix expression type.
 */
//-------------------------------------------------------------------
template<typename DataType>

struct is_matrix3d_reference< PolymorphicMatrix3D<DataType> > : std::true_type {};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class PolymorphicMatrixWrapper3D
 * @brief Wrapper class that provides a polymorphic interface to a given 3d-matrix type.
 *
 * This class wraps around a specific 3d-matrix type and exposes it through
 * the PolymorphicMatrix3D interface. It handles both const and non-const
 * 3d-matrix types, allowing for uniform treatment in a polymorphic context.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix3d_reference<ReferenceType>{}>* = nullptr>

class PolymorphicMatrixWrapper3D : public PolymorphicMatrix3D<typename ReferenceType::value_type>
{
public:

    // Type of value that is stored in the expression
    using value_type = typename ReferenceType::value_type;
    using DataType = typename ReferenceType::value_type;
    
    explicit PolymorphicMatrixWrapper3D(ReferenceType& matrix) : matrix_(matrix) {}

    uintptr_t pages() const override { return matrix_.pages(); }
    uintptr_t rows() const override { return matrix_.rows(); }
    uintptr_t columns() const override { return matrix_.columns(); }
    uintptr_t size() const override { return matrix_.size(); }

    std::error_code resize(uintptr_t pages, uintptr_t rows, uintptr_t columns) override { return matrix_.resize(pages, rows, columns); }

private: // Private functions

    // Accessors for matrix elements.
    DataType const_at_(int64_t page, int64_t row, int64_t column)const override { return matrix_.at(page, row, column); }
    DataType& non_const_at_(int64_t page, int64_t row, int64_t column) override { return matrix_.at(page, row, column); }

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

struct is_matrix3d_reference< PolymorphicMatrixWrapper3D<ReferenceType> > : std::true_type {};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Aliases for the polymorphic matrix and wrapper
//-------------------------------------------------------------------
template<typename DataType>
using Data3D = PolymorphicMatrix3D<DataType>;

template<typename ReferenceType>
using SpecializedData3D = PolymorphicMatrixWrapper3D<ReferenceType>;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Wraps a matrix in a PolymorphicMatrixWrapper3D and returns a shared pointer to PolymorphicMatrix3D.
 * 
 * @tparam ReferenceType Type of the 3d-matrix to wrap.
 * @param matrix The matrix to be wrapped.
 * @return std::shared_ptr<PolymorphicMatrix<value_type>> Shared pointer to the base polymorphic type of the wrapped matrix.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix3d_reference<ReferenceType>{}>* = nullptr>
         
inline auto wrap_matrix3d(ReferenceType matrix)
{
    using value_type = typename ReferenceType::value_type;
    
    std::shared_ptr<PolymorphicMatrix3D<value_type>> wrapped_matrix3d = std::make_shared<PolymorphicMatrixWrapper3D<ReferenceType>>(matrix);

    return wrapped_matrix3d;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_POLYMORPHIC_MATRIX3D_HPP_
