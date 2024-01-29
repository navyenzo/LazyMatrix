//-------------------------------------------------------------------
/**
 * @file custom_eigen_wrapper.hpp
 * @brief Provides a wrapper for integrating LazyMatrix references with Eigen.
 *
 * This file contains the CustomEigenWrapper class, which wraps around LazyMatrix
 * references to provide compatibility with Eigen's matrix operations.
 * 
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_EIGEN_WRAPPER_HPP_
#define INCLUDE_EIGEN_WRAPPER_HPP_



//-------------------------------------------------------------------
#include <Eigen/Eigen>
#include "shared_references.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class CustomEigenWrapper
 * @brief A wrapper class to integrate LazyMatrix references with Eigen.
 *
 * CustomEigenWrapper provides an interface to adapt LazyMatrix matrix references
 * (both const and non-const) for use with Eigen's operations and algorithms.
 *
 * @tparam ReferenceType Type of the LazyMatrix reference (SharedReferenceType or ConstSharedReferenceType).
 */
//-------------------------------------------------------------------
template<typename ReferenceType>

class CustomEigenWrapper : public Eigen::MatrixBase<CustomEigenWrapper<ReferenceType>>
{
public:

    // Define necessary Eigen types
    using Scalar = typename ReferenceType::value_type;
    using Index = Eigen::Index;
    static constexpr int RowsAtCompileTime = Eigen::Dynamic;
    static constexpr int ColsAtCompileTime = Eigen::Dynamic;
    static constexpr int MaxRowsAtCompileTime = Eigen::Dynamic;
    static constexpr int MaxColsAtCompileTime = Eigen::Dynamic;

    using Nested = CustomEigenWrapper<ReferenceType>&;
    using NestedExpression = CustomEigenWrapper<ReferenceType>;

    /**
     * @brief Constructs a wrapper around a LazyMatrix reference.
     * @param ref The LazyMatrix reference to wrap.
     */
    CustomEigenWrapper(ReferenceType ref)
    : matrix_(ref)
    {
    }

    /**
     * @brief Provides constant access to an element of the matrix.
     * @param row The row index.
     * @param col The column index.
     * @return A constant reference to the matrix element.
     */
    const Scalar coeff(Index row, Index col) const
    {
        return matrix_(row, col);
    }

    /**
     * @brief Provides non-constant access to an element of the matrix.
     *        Enabled only if the reference type allows non-const access.
     * @param row The row index.
     * @param col The column index.
     * @return A reference to the matrix element.
     */
    //template <typename T = ReferenceType, typename = std::enable_if_t<has_non_const_access<T>::value>>
    Scalar& coeffRef(Index row, Index col)
    {
        if constexpr (has_non_const_access<ReferenceType>::value)
        {
            return matrix_(row, col);
        }
        else
        {
            return DummyValueHolder<Scalar>::zero;
        }
    }

    // Size Information
    Index rows() const { return matrix_.rows(); }
    Index cols() const { return matrix_.columns(); }
    Index size() const { return matrix_.size(); }

    // Assignment Operators (if applicable)
    CustomEigenWrapper& operator=(const CustomEigenWrapper& other)
    {
        if (this != &other)
        {
            matrix_ = other.matrix_;
        }

        return *this;
    }

    const ReferenceType& get_matrix() const { return matrix_; }

    // Resize (only if non-const access is allowed and if resizing is applicable)
    // template <typename T = ReferenceType, typename = std::enable_if_t<has_non_const_access<T>::value>>
    // void resize(Index rows, Index cols)
    // {
    //     matrix_.resize(rows, cols);
    // }



private:

    ReferenceType matrix_; ///< The underlying LazyMatrix reference.
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Wraps a given LazyMatrix reference with a CustomEigenWrapper for compatibility with Eigen.
 * @tparam ReferenceType Type of the LazyMatrix reference (SharedReferenceType or ConstSharedReferenceType).
 *        The type must be a valid matrix reference type.
 * @param ref The matrix reference to be wrapped.
 * @return An instance of CustomEigenWrapper wrapping the given matrix reference.
 */
//-------------------------------------------------------------------
template<typename ReferenceType,
         std::enable_if_t<is_matrix_reference<ReferenceType>::value>* = nullptr>

inline auto wrap_with_eigen(ReferenceType ref)
{
    return CustomEigenWrapper<ReferenceType>(ref);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Specialize Eigen traits for CustomEigenWrapper
//-------------------------------------------------------------------
namespace Eigen {
namespace internal {



template<typename ReferenceType>         
struct traits<LazyMatrix::CustomEigenWrapper<ReferenceType>> : public Eigen::internal::traits<Eigen::Matrix<typename ReferenceType::value_type, Eigen::Dynamic, Eigen::Dynamic>>
{
    using Scalar = typename ReferenceType::value_type;
    using Index = Index;
};



// Evaluator for CustomEigenWrapper
template<typename ReferenceType>
struct evaluator<LazyMatrix::CustomEigenWrapper<ReferenceType>> : public evaluator_base<LazyMatrix::CustomEigenWrapper<ReferenceType>>
{
    using XprType = LazyMatrix::CustomEigenWrapper<ReferenceType>;

    explicit evaluator(XprType xpr)
    : expr_(xpr)
    {
    }

    // Access coefficient at a given row and column
    typename XprType::Scalar coeff(Index row, Index col) const
    {
        return expr_.coeff(row, col);
    }

    // Access coefficient at a given index (for vectors or linear access)
    typename XprType::Scalar coeff(Index index) const
    {
        // Assuming linear access; modify if your matrix uses a different storage order
        return expr_.coeff(index / expr_.cols(), index % expr_.cols());
    }

    // Access coefficient by row and column for non-const access (if applicable)
    typename XprType::Scalar& coeffRef(Index row, Index col)
    {
        return expr_.coeffRef(row, col);
    }

    // Get the number of rows
    Index rows() const { return expr_.rows(); }

    // Get the number of columns
    Index cols() const { return expr_.cols(); }

    // Get the total size (for vectors or linear access)
    Index size() const { return expr_.size(); }

private:

    XprType expr_;
};



} // namespace internal
} // namespace Eigen
//-------------------------------------------------------------------



#endif // INCLUDE_EIGEN_WRAPPER_HPP_