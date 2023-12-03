# LazyMatrix

## Overview
**LazyMatrix** is a sophisticated, header-only C++ library designed for intuitive and efficient handling of 2D and 3D matrix data. It leverages lazy evaluation for performance optimization and integrates seamlessly with the Eigen and dlib libraries for advanced matrix operations and neural network functionalities.

## Key Features
- **Header-Only**: Easy integration with just a single include.
- **Lazy Evaluation**: Optimized for performance, carrying out computations only when needed.
- **2D and 3D Matrix Support**: Unified interface for both 2D and 3D matrices.
- **Memory-Mapped File Storage**: `Matrix<Type>` and `Matrix3D<Type>` use memory-mapped files for efficient data sharing across threads and processes.
- **Eigen and dlib Integration**: For efficient matrix math and neural network support.
- **User-Friendly Syntax**: Designed for clarity and ease of use.

## Active Development
LazyMatrix is under active development. We regularly add new features and enhancements to make the library more powerful and easy to use.

## Getting Started
Clone the repository and include `lazy_matrix.hpp` in your project:
```bash
git clone https://github.com/navyenzo/LazyMatrix.git
```

Then update your cmake file adding LazyMatrix's include directory:
```cmake
# Add the LazyMatrix include directory
include_directories(path_to_LazyMatrix/include)
```

Alternatively, you can use FetchContent in CMake to include LazyMatrix (this in my opinion is a better choice):
```cmake
###############################################################
# LazyMatrix library
###############################################################

# Declare LazyMatrix dependency with a specific commit hash
FetchContent_Declare(
    LazyMatrix
    GIT_REPOSITORY https://github.com/navyenzo/LazyMatrix.git
    GIT_TAG main
)

# Make LazyMatrix content available for use
FetchContent_MakeAvailable(LazyMatrix)
###############################################################
```

Finally, in your c++:
```cpp
#include <lazy_matrix.hpp>
```

## Example Usage
Using LazyMatrix library is very simple, you just need to include one header file and that's it: ```#include <lazy_matrix.hpp>```

Here is a simple main.cpp file showing some examples on how to use some of the library's features, showcasing how we can do some advanced matrix manipulations/operations while keeiping code legible and easy to understand:
```cpp
#include <iostream>
#include <lazy_matrix.hpp>

int main()
{
    // Basic Matrix Operations
    auto m1 = LazyMatrix::MappedMatrix<double>(3, 3, 8.23); // 3x3 matrix with all elements as 8.23
    auto m2 = LazyMatrix::MappedMatrix<double>(3, 3, 3.34); // 3x3 matrix with all elements as 3.34

    std::cout << "m1 =\n" << m1 << "\n";
    std::cout << "m2 =\n" << m2 << "\n";
    std::cout << "m1 + m2 =\n" << m1 + m2 << "\n";
    std::cout << "m1 .* m2 (element-wise multiplication) =\n" << LazyMatrix::elem_by_elem_multiply(m1, m2) << "\n";
    std::cout << "sqrt(m1) =\n" << LazyMatrix::sqrt(m1) << "\n";

    // Interval Arithmetic
    LazyMatrix::Interval<double> I1(1, 2), I2(4, 5);
    std::cout << "Interval I1 = " << I1 << "\n";
    std::cout << "Interval I2 = " << I2 << "\n";
    std::cout << "Interval I1 + I2 = " << I1 + I2 << "\n";
    std::cout << "Interval I1 * I2 = " << I1 * I2 << "\n";

    // Augmenting Matrices
    auto augmentedByRows = LazyMatrix::augment_by_rows_view(m1, m2);
    std::cout << "Augmented Matrix by Rows:\n" << augmentedByRows << "\n";
    auto augmentedByColumns = LazyMatrix::augment_by_columns_view(m1, m2);
    std::cout << "Augmented Matrix by Columns:\n" << augmentedByColumns << "\n";

    // Creating a Region Of Interest (ROI) View
    auto roiMatrix = LazyMatrix::roi_view(m1, 1, 1, 2, 2);
    std::cout << "Region Of Interest Matrix:\n" << roiMatrix << "\n";

    // Using Matrix Generators
    LazyMatrix::Iota<int> iotaMatrix(3, 3);
    std::cout << "Iota Matrix:\n" << iotaMatrix << "\n";
    LazyMatrix::RandomMatrix<int> randomIntMatrix(3, 3, 0, 10);
    std::cout << "Random Integer Matrix:\n" << randomIntMatrix << "\n";

    // Employing Border Views
    auto repeatedBorderMatrix = LazyMatrix::repeated_border_view(m1);
    std::cout << "Repeated Border View at (-1, -1): " << repeatedBorderMatrix.at_(-1, -1) << "\n";
    auto constantBorderMatrix = LazyMatrix::constant_border_view(m1, 0.0);
    std::cout << "Constant Border View at (4, 4): " << constantBorderMatrix.at_(4, 4) << "\n";

    // CSV Matrix Handling
    LazyMatrix::CSVMatrix<double> csvMatrix;
    csvMatrix.load("data.csv", true, true);
    std::cout << "CSV Matrix:\n" << csvMatrix << "\n";

    // Handling Strings and Conversions
    std::string z = "+.e5.4";
    double convertedZ;
    int endOfConversion = LazyMatrix::from_string(convertedZ, z.data(), 0, z.size(), '.');
    std::cout << "Converted Value: " << convertedZ << "\n";

    // TODO: Need to put more examples showcasing more advanced matrix operations

    return 0;
}
```

## License
LazyMatrix is available under the MIT License. This permissive license allows for wide use and modification as long as the original license and copyright notice are included.

## Contributing
We welcome contributions from the community! Whether it's adding new features, fixing bugs, or improving documentation, your help is valuable. Please see [CONTRIBUTING.md](CONTRIBUTING.md) for more details on how to get involved.

## Stay Updated
For the latest updates, enhancements, and features, keep an eye on our Githut repository

## About the Author

LazyMatrix is developed and maintained by Vincenzo Barbato. For more information about my professional background and other projects, please visit my [LinkedIn profile](https://www.linkedin.com/in/vincenzobarbato/).