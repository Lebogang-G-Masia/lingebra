# Lingebra

A lightweight, header-only C++ linear algebra library for matrix and vector manipulations. **Lingebra** provides an intuitive API for standard mathematical operations while ensuring efficient memory management through a strict implementation of the **Rule of Five**.

---

## Features

* **Header-Only**: Easy to integrate into any project.
* **Memory Managed**: Full support for Copy/Move semantics to prevent leaks and optimize performance.
* **Intuitive Syntax**: Operator overloading for matrix addition, subtraction, and scalar multiplication.
* **Advanced Operations**: 
    * Matrix Multiplication (`matmul`)
    * Recursive Determinant calculation
    * Cofactor Matrix generation
    * Transposition
    * Matrix Inversion (using Adjugate method)
* **Exception Safety**: Robust error handling for arithmetic and dimension mismatches (e.g., non-square matrices for inversion).
* **Clean Output**: Overloaded `<<` operator with a built-in tolerance for floating-point precision.

---

## Usage

### 1. Initialization
You can initialize matrices by dimensions or via nested initializer lists.

```cpp
#include "matrix.h"

using namespace Lingebra;

// Initialize a 3x3 matrix
matrix A = {
    {1.0, 2.0, 3.0},
    {0.0, 1.0, 4.0},
    {5.0, 6.0, 0.0}
};

// Access elements
double val = A[0][2]; // 3.0
```

### 2. Basic arithmetic

The library supports standard matrix algebra operations.

```cpp
matrix B = A * 2.0;       // Scalar multiplication
matrix C = A + B;         // Addition
matrix D = A.matmul(B);   // Matrix Multiplication
```

### 3. Linear algebra functions

Perform complex operations with static methods.

```cpp
try {
    double det = matrix::determinant(A);
    matrix inv = matrix::inverse(A);
    matrix trans = matrix::transpose(A);

    std::cout << "Inverse of A:\n" << inv;
} catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
}
```

## API reference

### Constructors & Memory
| Method | Description |
| :--- | :--- |
| **Constructor** | Creates a matrix of specific dimensions or from nested lists. |
| **Rule of Five** | Includes Copy/Move constructors and Assignment operators. |
| **Destructor** | Safely deallocates the underlying vector array. |

### Core Matrix Methods
| Method | Description |
| :--- | :--- |
| **shape()** | Returns a vector with dimensions `[rows, cols]`. |
| **matmul(other)** | Performs dot product multiplication between two matrices. |
| **transpose(mat)** | Static method that returns the transposed version of a matrix. |
| **determinant(mat)** | Static method that returns the scalar determinant. |
| **inverse(mat)** | Static method that returns the inverse matrix. |

---

## Requirements

* **C++ Standard**: C++11 or higher.
* **Internal Dependencies**: Requires `vector.h` and `error_handling.h`.

---

## License
This project is open-source. Feel free to use and modify for your own linear algebra needs.
