//-------------------------------------------------------------------
/**
 * @file matrix_factory.hpp
 * @brief Defines functions used to build matrix storage and return by smart pointers.
 *
 * The matrix_factory.hpp file defines functions that create 2d or 3d matrix storage
 * and return a shared pointer to the matrix storage
 * 
 * @author Vincenzo Barbato
 * 
 * Additional Information:
 * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
 * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_MATRIX_FACTORY_HPP_
#define INCLUDE_MATRIX_FACTORY_HPP_



//-------------------------------------------------------------------
#include <memory>
#include "matrix.hpp"
#include "matrix3d.hpp"
#include "simple_matrix.hpp"
#include "simple_matrix3d.hpp"
#include "csv_matrix.hpp"
#include "image_matrix.hpp"
#include "shared_references.hpp"
#include "database_matrix.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace LazyMatrix
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Factory class used to create a SharedMatrixRef reference of
 *        different types of matrices.
 * 
 * This class defines factory functions used to create a SharedMatrixRef
 * reference of different matrix objects.
 */
//-------------------------------------------------------------------
class MatrixFactory
{
public:

    /**
     * @brief Create a Matrix object
     * 
     * @tparam DataType 
     * @tparam Args 
     * @param args 
     * @return SharedMatrixRef<Matrix<DataType>> 
     */
    template<typename DataType, typename... Args>
    static SharedMatrixRef<Matrix<DataType>> create_matrix(Args&&... args)
    {
        auto matrix_ptr = std::make_shared<Matrix<DataType>>(std::forward<Args>(args)...);
        return SharedMatrixRef<Matrix<DataType>>(matrix_ptr);
    }

    /**
     * @brief Create a CSVMatrix object
     * 
     * @tparam DataType 
     * @tparam Args 
     * @param args 
     * @return SharedMatrixRef<CSVMatrix<DataType>> 
     */
    template<typename DataType, typename... Args>
    static SharedMatrixRef<CSVMatrix<DataType>> create_csv_matrix(Args&&... args)
    {
        auto matrix_ptr = std::make_shared<CSVMatrix<DataType>>(std::forward<Args>(args)...);
        return SharedMatrixRef<CSVMatrix<DataType>>(matrix_ptr);
    }

    /**
     * @brief Create an ImageMatrix object
     * 
     * @tparam DataType 
     * @tparam Args 
     * @param args 
     * @return SharedMatrixRef<ImageMatrix<DataType>> 
     */
    template<typename DataType, typename... Args>
    static SharedMatrixRef<ImageMatrix<DataType>> create_image_matrix(Args&&... args)
    {
        auto matrix_ptr = std::make_shared<ImageMatrix<DataType>>(std::forward<Args>(args)...);
        return SharedMatrixRef<ImageMatrix<DataType>>(matrix_ptr);
    }

    /**
     * @brief Create a SimpleMatrix object
     * 
     * @tparam DataType 
     * @tparam Args 
     * @param args 
     * @return SharedMatrixRef<SimpleMatrix<DataType>> 
     */
    template<typename DataType, typename... Args>
    static SharedMatrixRef<SimpleMatrix<DataType>> create_simple_matrix(Args&&... args)
    {
        auto matrix_ptr = std::make_shared<SimpleMatrix<DataType>>(std::forward<Args>(args)...);
        return SharedMatrixRef<SimpleMatrix<DataType>>(matrix_ptr);
    }

    /**
     * @brief Create a DatabaseMatrix object
     * 
     * @tparam Args 
     * @param args 
     * @return SharedMatrixRef<DatabaseMatrix> 
     */
    template<typename... Args>
    static ConstSharedMatrixRef<DatabaseMatrix> create_database_matrix(Args&&... args)
    {
        auto matrix_ptr = std::make_shared<DatabaseMatrix>(std::forward<Args>(args)...);
        return ConstSharedMatrixRef<DatabaseMatrix>(matrix_ptr);
    }

    /**
     * @brief Create a Matrix3D object
     * 
     * @tparam DataType 
     * @tparam Args 
     * @param args 
     * @return SharedMatrixRef<Matrix<DataType>> 
     */
    template<typename DataType, typename... Args>
    static SharedMatrix3DRef<Matrix3D<DataType>> create_matrix3d(Args&&... args)
    {
        auto matrix3d_ptr = std::make_shared<Matrix3D<DataType>>(std::forward<Args>(args)...);
        return SharedMatrix3DRef<Matrix3D<DataType>>(matrix3d_ptr);
    }

    /**
     * @brief Create a SimpleMatrix3D object
     * 
     * @tparam DataType 
     * @tparam Args 
     * @param args 
     * @return SharedMatrixRef<Matrix<DataType>> 
     */
    template<typename DataType, typename... Args>
    static SharedMatrix3DRef<SimpleMatrix3D<DataType>> create_simple_matrix3d(Args&&... args)
    {
        auto matrix3d_ptr = std::make_shared<SimpleMatrix3D<DataType>>(std::forward<Args>(args)...);
        return SharedMatrix3DRef<SimpleMatrix3D<DataType>>(matrix3d_ptr);
    }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyMatrix
//-------------------------------------------------------------------



#endif  // INCLUDE_MATRIX_FACTORY_HPP_
