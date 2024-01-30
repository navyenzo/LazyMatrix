    //-------------------------------------------------------------------
    /**
     * @file test_eigen_matrix_interface.cpp
     * @brief Test file for the CustomEigenWrapper class using Catch2.
     *
     * This file tests the custom eigen wrapper interface that makes
     * LazyMatrix matrix expressions look like eigen matrix expressions so
     * they can be used in Eigen algorithms.
     * 
     * @author Vincenzo Barbato
     * 
     * Additional Information:
     * - GitHub Project: [LazyMatrix](https://github.com/navyenzo/LazyMatrix.git)
     * - LinkedIn: [Vincenzo Barbato](https://www.linkedin.com/in/vincenzobarbato/)
     */
    //-------------------------------------------------------------------



    //-------------------------------------------------------------------
    #include <catch2/catch_all.hpp>
    #include <lazy_matrix.hpp>
    //-------------------------------------------------------------------



    //-------------------------------------------------------------------
    TEST_CASE("CustomEigenWrapper Basic Functionality", "[eigen_wrapper]")
    {
        // Test initialization and basic element access
        SECTION("Initialization and Element Access")
        {
            // Create a LazyMatrix matrix (adjust as per your actual matrix creation)
            auto matrix = LazyMatrix::MatrixFactory::create_simple_matrix<double>(3, 3, 1.0);  // 3x3 matrix with all elements as 1.0

            // Wrap the matrix reference in CustomEigenWrapper
            auto eigen_wrapped_matrix = LazyMatrix::eigen_wrap(matrix);

            eigen_wrapped_matrix(0,1) = -10.05;

            // Check basic element access
            REQUIRE(eigen_wrapped_matrix(0, 0) == 1.0);
            REQUIRE(eigen_wrapped_matrix(1, 1) == 1.0);
            REQUIRE(eigen_wrapped_matrix(2, 2) == 1.0);
        }

        // TODO: Need to implement the part where CustomEigenWrapper can be
        //       used interchangeably in eigen algorithms

        // // Section for Testing Addition and Subtraction on Wrapped Matrices
        // SECTION("Addition and Subtraction on Wrapped Matrices")
        // {
        //     // Create and wrap matrices
        //     auto matrix1 = LazyMatrix::MatrixFactory::create_simple_matrix<double>(3, 3, 1.0);
        //     auto matrix2 = LazyMatrix::MatrixFactory::create_simple_matrix<double>(3, 3, 2.0);
        //     auto wrapped1 = LazyMatrix::eigen_wrap(matrix1);
        //     auto wrapped2 = LazyMatrix::eigen_wrap(matrix2);

        //     // Perform and test operations on the custom wrappers
        //     // to see if eigen recognizes them as eigen matrices
        //     Eigen::MatrixXd resultAdd = wrapped1 + wrapped2;
        //     Eigen::MatrixXd resultSub = wrapped1 - wrapped2;

        //     // Check results
        //     for (int i = 0; i < resultAdd.rows(); ++i)
        //     {
        //         for (int j = 0; j < resultAdd.cols(); ++j)
        //         {
        //             REQUIRE(resultAdd(i, j) == 3.0);
        //             REQUIRE(resultSub(i, j) == -1.0);
        //         }
        //     }
        // }

        // Section for Testing Wrapping the Result of Addition and Subtraction
        SECTION("Wrapping Result of Addition and Subtraction")
        {
            // Create matrices
            auto matrix1 = LazyMatrix::MatrixFactory::create_simple_matrix<double>(3, 3, 1.0);
            auto matrix2 = LazyMatrix::MatrixFactory::create_simple_matrix<double>(3, 3, 2.0);

            // Perform operations and then wrap the results
            auto resultAdd = LazyMatrix::eigen_wrap(matrix1 + matrix2);
            auto resultSub = LazyMatrix::eigen_wrap(matrix1 - matrix2);

            // Check results
            for (int i = 0; i < resultAdd.rows(); ++i)
            {
                for (int j = 0; j < resultAdd.cols(); ++j)
                {
                    REQUIRE(3.0 == resultAdd(i, j));
                    REQUIRE(-1.0 == resultSub(i, j));
                }
            }
        }

        // Section for Testing Norm Calculation on Wrapped Matrices
        SECTION("Norm Calculation on Wrapped Matrices")
        {
            // Create and wrap a matrix
            auto matrix = LazyMatrix::MatrixFactory::create_simple_matrix<double>(3,3,1.0);
            auto wrapped = LazyMatrix::eigen_wrap(matrix);

            // Calculate the norm of the wrapped matrix
            double norm = wrapped.norm();

            // Check the result
            REQUIRE(norm == Catch::Approx(std::sqrt(9.0))); // Since all elements are 1.0, the Frobenius norm should be sqrt(9)
        }
    }
    //-------------------------------------------------------------------