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
 * @link https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_POLYMORPHIC_MATRIX3D_HPP_
#define INCLUDE_POLYMORPHIC_MATRIX3D_HPP_



//-------------------------------------------------------------------
#include <memory>
#include <type_traits>
#include "base_matrix3d.hpp"
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

class PolymorphicMatrix3D : public BaseMatrix3D< PolymorphicMatrix3D<DataType> >
{
public:

    PolymorphicMatrix3D() = default;
    virtual ~PolymorphicMatrix3D() = default;

    virtual uintptr_t pages() const = 0;
    virtual uintptr_t rows() const = 0;
    virtual uintptr_t columns() const = 0;
    virtual uintptr_t size() const = 0;
    


    virtual const DataType& at_(int64_t page, int64_t row, int64_t column) const = 0;
    virtual DataType& at_(int64_t page, int64_t row, int64_t column) = 0;
};
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
template<typename MatrixType>

class PolymorphicMatrixWrapper3D : public PolymorphicMatrix3D<typename std::remove_const<typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0,0))>::type>::type>
{
public:

    // Type of value that is stored in the expression
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0,0))>::type>::type;
    


    explicit PolymorphicMatrixWrapper3D(MatrixType& matrix) : matrix_(matrix) {}

    
    
    uintptr_t pages() const override { return matrix_.pages(); }
    uintptr_t rows() const override { return matrix_.rows(); }
    uintptr_t columns() const override { return matrix_.columns(); }
    uintptr_t size() const override { return matrix_.size(); }
    


    const value_type& at_(int64_t page, int64_t row, int64_t column) const override
    {
        return matrix_(page, row, column);
    }
    
    value_type& at_(int64_t page, int64_t row, int64_t column) override 
    {
        if constexpr (std::is_const_v<MatrixType>)
        {
            return zero; // Return default value for const matrix types
        }
        else
        {
            return matrix_(page, row, column); // Non-const matrices allow modification
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

struct is_type_a_matrix3d< PolymorphicMatrixWrapper3D<MatrixType> > : std::true_type {};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Aliases for the polymorphic matrix and wrapper
//-------------------------------------------------------------------
template<typename DataType>
using Data3D = PolymorphicMatrix3D<DataType>;

template<typename MatrixType>
using SpecializedData3D = PolymorphicMatrixWrapper3D<MatrixType>;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Wraps a matrix in a PolymorphicMatrixWrapper3D and returns a shared pointer to PolymorphicMatrix3D.
 * 
 * @tparam MatrixType Type of the 3d-matrix to wrap.
 * @param matrix The matrix to be wrapped.
 * @return std::shared_ptr<PolymorphicMatrix<value_type>> Shared pointer to the base polymorphic type of the wrapped matrix.
 */
//-------------------------------------------------------------------
template<typename MatrixType,
         std::enable_if_t<is_type_a_matrix3d<MatrixType>{}>* = nullptr>
         
inline auto wrap_matrix3d(MatrixType& matrix)
{
    using value_type = typename std::remove_const<typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0,0))>::type>::type;

    std::shared_ptr<PolymorphicMatrix3D<value_type>> wrapped_matrix3d = std::make_shared<PolymorphicMatrixWrapper3D<MatrixType>>(matrix);

    return wrapped_matrix3d;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_POLYMORPHIC_MATRIX3D_HPP_
