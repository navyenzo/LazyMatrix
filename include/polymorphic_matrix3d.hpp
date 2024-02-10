//-------------------------------------------------------------------
/**
 * @file polymorphic_matrix3d.hpp
 * @brief Provides polymorphic wrappers for various 3d-matrix-like data structures in LazyMatrix.
 *
 * The polymorphic_matrix3d.hpp file defines classes and functions crucial for creating a polymorphic
 * interface for 3d-matrix-like data structures. It allows for uniform handling of different 3d matrix
 * types in a polymorphic context. This file includes the definition of the `PolymorphicMatrix3D` base
 * class, which offers a generic 3d matrix interface, and the `PolymorphicMatrix3DWrapper` class, which
 * wraps around specific 3d matrix types and exposes them through the `PolymorphicMatrix3D` interface.
 * These tools are essential for applications where polymorphic behavior with 3d matrices is required.
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
#include "numerical_constants.hpp"
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
 * This class defines a generic interface for 3d matrices, allowing for 
 * operations like accessing elements and querying dimensions. It's
 * templated to handle different data types within the matrix.
 */ 
//-------------------------------------------------------------------
template<typename DataType, bool HasNonConstAccess>

class PolymorphicMatrix3D : public BaseMatrix3D<PolymorphicMatrix3D<DataType,HasNonConstAccess>,HasNonConstAccess>
{
public:

    using value_type = DataType;

    friend class BaseMatrix3D<PolymorphicMatrix3D<DataType,HasNonConstAccess>,HasNonConstAccess>;

    PolymorphicMatrix3D() = default;
    virtual ~PolymorphicMatrix3D() = default;

    virtual uintptr_t pages() const = 0;
    virtual uintptr_t rows() const = 0;
    virtual uintptr_t columns() const = 0;
    virtual uintptr_t size() const = 0;
    virtual std::error_code resize(uintptr_t pages, uintptr_t rows, uintptr_t columns) = 0;

private:

    virtual DataType const_at_(int64_t page, int64_t row, int64_t column) const = 0;
    virtual DataType& non_const_at_(int64_t page, int64_t row, int64_t column) = 0;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Template specialization of PolymorphicMatrix3D
//-------------------------------------------------------------------
template<typename DataType>

class PolymorphicMatrix3D<DataType, false> : public BaseMatrix3D<PolymorphicMatrix3D<DataType,false>,false>
{
public:

    using value_type = DataType;

    friend class BaseMatrix3D<PolymorphicMatrix3D<DataType,false>,false>;

    PolymorphicMatrix3D() = default;
    virtual ~PolymorphicMatrix3D() = default;

    virtual uintptr_t pages() const = 0;
    virtual uintptr_t rows() const = 0;
    virtual uintptr_t columns() const = 0;
    virtual uintptr_t size() const = 0;
    virtual std::error_code resize(uintptr_t pages, uintptr_t rows, uintptr_t columns) = 0;

private:

    virtual DataType const_at_(int64_t page, int64_t row, int64_t column) const = 0;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Compile time function to check if the type is a matrix expression type.
 */
//-------------------------------------------------------------------
template<typename DataType, bool HasNonConstAccess>
struct is_type_a_matrix3d< PolymorphicMatrix3D<DataType,HasNonConstAccess> > : std::true_type {};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class PolymorphicMatrixWrapper
 * @brief Wrapper class that provides a const polymorphic interface to a given matrix type.
 *
 * This class wraps around a specific matrix type and exposes it through
 * the PolymorphicMatrix interface. It handles both const and non-const
 * matrix types, allowing for uniform treatment in ConstPolymorphicMatrixa polymorphic context.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         bool HasNonConstAccess = has_non_const_access<ReferenceType>::value>

class PolymorphicMatrix3DWrapper : public PolymorphicMatrix3D<typename ReferenceType::value_type,
                                                              HasNonConstAccess>
{
public:

    // Type of value that is sterror:ored in the expression
    using value_type = typename ReferenceType::value_type;

    explicit PolymorphicMatrix3DWrapper(ReferenceType matrix) : matrix_(matrix)
    {
    }

    uintptr_t pages() const override { return matrix_.pages(); }
    uintptr_t rows() const override { return matrix_.rows(); }
    uintptr_t columns() const override { return matrix_.columns(); }
    uintptr_t size() const override { return matrix_.size(); }

    std::error_code resize(uintptr_t pages, uintptr_t rows, uintptr_t columns) override
    {
        if constexpr (has_non_const_access<ReferenceType>::value)
        {
            return matrix_.resize(pages, rows, columns);
        }
        else
        {
            return std::error_code();
        }
    }



private: // Private functionserror:

    value_type const_at_(int64_t page, int64_t row, int64_t column)const override
    {
        return matrix_(page, row, column);
    }

    value_type& non_const_at_(int64_t page, int64_t row, int64_t column) override
    {
        return matrix_(page, row, column);
    }
    

private: // Private variables

    ReferenceType matrix_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Template specialization of PolymorphicMatrix3DWrapper
//-------------------------------------------------------------------
template<typename ReferenceType>

class PolymorphicMatrix3DWrapper<ReferenceType, false> : public PolymorphicMatrix3D<typename ReferenceType::value_type, false>
{
public:

    // Type of value that is sterror:ored in the expression
    using value_type = typename ReferenceType::value_type;

    explicit PolymorphicMatrix3DWrapper(ReferenceType matrix) : matrix_(matrix)
    {
    }

    uintptr_t pages() const override { return matrix_.pages(); }
    uintptr_t rows() const override { return matrix_.rows(); }
    uintptr_t columns() const override { return matrix_.columns(); }
    uintptr_t size() const override { return matrix_.size(); }

    std::error_code resize(uintptr_t pages, uintptr_t rows, uintptr_t columns) override
    {
        if constexpr (has_non_const_access<ReferenceType>::value)
        {
            return matrix_.resize(pages, rows, columns);
        }
        else
        {
            return std::error_code();
        }
    }



private: // Private functionserror:

    value_type const_at_(int64_t page, int64_t row, int64_t column)const override
    {
        return matrix_(page, row, column);
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
template<typename ReferenceType, bool HasNonConstAccess>

struct is_type_a_matrix3d< PolymorphicMatrix3DWrapper<ReferenceType, HasNonConstAccess> > : std::true_type {};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Aliases for the polymorphic matrix and wrapper
//-------------------------------------------------------------------
template<typename DataType>
using Data3D = PolymorphicMatrix3D<DataType,true>;

template<typename DataType>
using ConstData3D = PolymorphicMatrix3D<DataType,false>;

template<typename ReferenceType>
using SpecializedData3D = PolymorphicMatrix3DWrapper<ReferenceType,true>;

template<typename ReferenceType>
using ConstSpecializedData3D = PolymorphicMatrix3DWrapper<ReferenceType,false>;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Wraps a matrix in a PolymorphicMatrixWrapper3D and returns a Shared
 *        Reference to the base PolymorphicMatrix3D class so that a user can
 *        store multiple different types of 3d-matrices in a single container
 * 
 * @tparam ReferenceType Type of the matrix to wrap.
 * @param matrix The matrix to be wrapped.
 * @return SharedMatrixRef wrapping the input matrix.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix3d_reference<ReferenceType>{}>* = nullptr>
         
inline auto wrap_matrix3d(ReferenceType matrix)
{
    using value_type = typename ReferenceType::value_type;

    if constexpr (has_non_const_access<ReferenceType>::value)
    {
        std::shared_ptr<Data3D<value_type>> wrapped_matrix = std::make_shared<SpecializedData3D<ReferenceType>>(matrix);
        return SharedMatrix3DRef<Data3D<value_type>>(wrapped_matrix);
    }
    else
    {
        std::shared_ptr<ConstData3D<value_type>> wrapped_matrix = std::make_shared<ConstSpecializedData3D<ReferenceType>>(matrix);
        return ConstSharedMatrix3DRef<ConstData3D<value_type>>(wrapped_matrix);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Wraps a matrix in a PolymorphicMatrixWrapper with const access only.
 * 
 * @tparam ReferenceType Type of the matrix to wrap.
 * @param matrix The matrix to be wrapped.
 * @return SharedMatrixRef wrapping the input matrix.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix3d_reference<ReferenceType>{}>* = nullptr>
         
inline auto wrap_matrix3d_const(ReferenceType matrix)
{
    using value_type = typename ReferenceType::value_type;
    
    std::shared_ptr<ConstData3D<value_type>> wrapped_matrix = std::make_shared<ConstSpecializedData3D<ReferenceType>>(matrix);
    return ConstSharedMatrix3DRef<ConstData3D<value_type>>(wrapped_matrix);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_POLYMORPHIC_MATRIX3D_HPP_
