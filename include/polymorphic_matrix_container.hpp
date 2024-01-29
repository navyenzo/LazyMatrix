//-------------------------------------------------------------------
/**
 * @file polymorphic_matrix_container.hpp
 * @brief This file contains the MatrixContainer class for storing and
 *        managing a collection of polymorphic 2D matrix references, augmented
 *        as a 3D matrix.
 *        - The MatrixContainer class provides functionalities to store,
 *          access, and manage a dynamic array of 2D matrix references wrapped
 *          as a single 3D matrix using SharedMatrixRef. It enables handling
 *          matrices of different types and sizes in a unified structure.
 * 
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef POLYMORPHIC_MATRIX_VECTOR_HPP
#define POLYMORPHIC_MATRIX_VECTOR_HPP



//-------------------------------------------------------------------
#include <vector>
#include "polymorphic_matrix.hpp"
#include "base_matrix3d.hpp"
#include "matrix_factory.hpp"
//-------------------------------------------------------------------



namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class MatrixContainer
 * @brief Container class for managing a collection of 2D polymorphic matrix references augmented as a 3D matrix.
 *
 * MatrixContainer holds SharedMatrixRef objects, allowing dynamic storage and access of polymorphic 2D matrices.
 * The class provides mechanisms to add, remove, and access matrices. It maintains the maximum rows and columns
 * across all matrices to present a unified 3D matrix interface.
 */
//-------------------------------------------------------------------
template<typename DataType>
class MatrixContainer : public BaseMatrix3D<MatrixContainer<DataType>, false>
{
public:

    using value_type = DataType;
    using MatrixBaseRef = ConstSharedMatrixRef<PolymorphicMatrix<DataType>>;

    friend class BaseMatrix3D<MatrixContainer<DataType>, false>;

    /**
     * @brief Adds a new matrix to the container and pads it to maintain consistent dimensions.
     * @param matrix Reference to the matrix to be added.
     */
    template<typename ReferenceType,
                std::enable_if_t<is_matrix_reference<ReferenceType>{}>* = nullptr>
    void add_matrix(const ReferenceType& matrix)
    {
        MatrixBaseRef wrapped_matrix = wrap_matrix(matrix);
        matrices_.push_back(wrapped_matrix);
        
        max_rows_ = std::max(max_rows_, matrix.rows());
        max_columns_ = std::max(max_columns_, matrix.columns());
    }

    /**
     * @brief Removes a matrix from the container by its index.
     * @param index Index of the matrix to be removed.
     */
    void remove_matrix(size_t index)
    {
        auto& matrix = matrices_[index];
        bool recalculate = matrix.rows() == max_rows_ || matrix.columns() == max_columns_;

        matrices_.erase(matrices_.begin() + index);

        if (recalculate)
        {
            recalculate_dimensions();
        }
    }

    /**
     * @brief Accesses a matrix by its index.
     * @param index Index of the matrix to be accessed.
     * @return Reference to the matrix at the specified index.
     */
    MatrixBaseRef& operator[](size_t index)
    {
        return matrices_[index];
    }

    /**
     * @brief Accesses a matrix by its index (const version).
     * @param index Index of the matrix to be accessed.
     * @return Const reference to the matrix at the specified index.
     */
    const MatrixBaseRef& operator[](size_t index) const
    {
        return matrices_[index];
    }
    
    /**
     * @brief Gets the total number of matrices in the container.
     * @return Number of matrices.
     */
    size_t size() const
    {
        return matrices_.size();
    }

    // BaseMatrix3D interface implementation
    uintptr_t pages() const
    {
        return matrices_.size();
    }

    uintptr_t rows() const
    {
        return max_rows_;
    }

    uintptr_t columns() const
    {
        return max_columns_;
    }



private: // Private functions

    /**
     * @brief Accesses the element at the specified position.
     * @param page Page index
     * @param row Row index.
     * @param column Column index.
     * @return A copy of the value of the element at the specified position. This
     *         function indexes the vector of matrices as if it were a single 3d matrix.
     */
    DataType const_at_(int64_t page, int64_t row, int64_t column) const
    {
        if (page < 0 || page >= static_cast<int64_t>(matrices_.size()) ||
            row >= matrices_[page].rows() ||
            column >= matrices_[page].columns())
        {
            return DummyValueHolder<DataType>::zero;
        }
        else
        {
            return matrices_[page](row, column);
        }
    }

    /**
     * @brief Accesses the element at the specified position.
     * @param row Row index.
     * @param column Column index.
     * @return A reference to the element at the specified position. This function
     *         indexes the vector of matrices as if it were a single 3d matrix.
     */
    // DataType& non_const_at_(int64_t page, int64_t row, int64_t column)
    // {
    //     if (page < 0 || page >= static_cast<int64_t>(matrices_.size()) ||
    //         row >= matrices_[page].rows() ||
    //         column >= matrices_[page].columns())
    //     {
    //         return DummyValueHolder<DataType>::zero;
    //     }
    //     else
    //     {
    //         return matrices_[page](row, column);
    //     }
    // }

    /**
     * @brief Recalculates the maximum rows and columns across all matrices.
     */
    void recalculate_dimensions()
    {
        max_rows_ = 0;
        max_columns_ = 0;

        for (const auto& matrix : matrices_)
        {
            max_rows_ = std::max(max_rows_, matrix.rows());
            max_columns_ = std::max(max_columns_, matrix.columns());
        }
    }



private: // Private variables

    std::vector<MatrixBaseRef> matrices_;   ///< Vector holding the stored matrices.
    uintptr_t max_rows_ = 0;                ///< Maximum number of rows across all matrices.
    uintptr_t max_columns_ = 0;             ///< Maximum number of columns across all matrices.
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Compile time functions to check if the type is an expression type
//-------------------------------------------------------------------
template<typename DataType>

struct is_type_a_matrix3d< MatrixContainer<DataType> > : std::true_type
{
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif // POLYMORPHIC_MATRIX_VECTOR_HPP
