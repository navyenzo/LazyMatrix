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

## License
LazyMatrix is available under the MIT License. This permissive license allows for wide use and modification as long as the original license and copyright notice are included.

## Contributing
We welcome contributions from the community! Whether it's adding new features, fixing bugs, or improving documentation, your help is valuable. Please see our contributing guidelines for more information on how to contribute.

## Stay Updated
For the latest updates, enhancements, and features, keep an eye on our Githut repository