# Lingebra

**Lingebra** is a high-performance, lightweight linear algebra library written in C++20. It features custom memory management, AVX-friendly data structures, and optimized algorithms for matrix operations.

## Key Features

* **High-Performance Matrix Multiplication**: Implements the **IKJ loop interchange algorithm** to maximize CPU cache locality, achieving $O(N^3)$ performance.
* **Optimized Memory Layout**: 
    * Uses a **flat 1D array** representation for matrices to ensure spatial locality.
    * Custom `Vector` container uses **32-byte memory alignment** (via `aligned_alloc`), enabling the compiler to auto-vectorize loops (AVX/SIMD).
* **Fast Inversion & Determinants**: Replaces naive $O(N!)$ Laplace expansion with **LU Decomposition** ( $O(N^3)$ ) for instant calculations on large matrices.
* **Modern C++ Design**:
    * Implements the **Rule of Five** for efficient resource management (Move Semantics).
    * RAII-compliant memory safety.
    * No external dependencies (Standard Library only).

## Build & Installation

Lingebra is a source-based library. To use it, simply include the headers and compile the source files with your project.

### Requirements
* C++ Compiler supporting C++17 or later (e.g., GCC, Clang, MSVC).
* Optimization flags (e.g., `-O3`) are highly recommended to enable vectorization.

### Compilation:

To compile a project using Lingebra:

```cpp
# Recommended: Enable O3 optimization for SIMD/AVX support
g++ main.cpp src/matrix.cpp src/vector.cpp -O3 -o my_app
```

## API Reference

### Class: `Lingebra::Matrix`

| Method / Operator | Signature | Description |
|------------------|-----------|-------------|
| Constructor | `Matrix(size_t r, size_t c)` | Initializes a matrix of dimensions **r × c** filled with zeros. |
| Constructor | `Matrix({ {v1, v2}, ... })` | Initializes a matrix from a nested initializer list (e.g., `{{1,2}, {3,4}}`). |
| Shape | `Vector shape()` | Returns a vector containing `{nrows, ncols}`. |
| Multiplication | `Matrix matmul(const Matrix& B)` | Performs matrix multiplication (**A × B**) using the optimized **IKJ algorithm**. |
| Determinant | `double determinant()` | Calculates the determinant using **LU Decomposition** *(O(N³))* |
| Inverse | `Matrix inverse()` | Calculates the inverse matrix using **LU Decomposition** and **Back Substitution**. |
| Transpose | `Matrix transpose()` | Returns a new matrix that is the transpose of the current one. |
| Cofactors | `Matrix cofactors()` | Returns the cofactor matrix (derived efficiently via **Inverse**). |
| Access | `double& operator()(i, j)` | Logical element access (row *i*, column *j*). |
| Raw Access | `double* data_ptr()` | Returns a raw pointer to the underlying flat 1D array (useful for optimizations). |
| Arithmetic | `+, -, +=, -=` | Standard element-wise matrix addition and subtraction. |
| Scaling | `*, *=` | Scalar multiplication (`Matrix * double`). |

---

## Performance

| Operation | Naive Implementation | Lingebra Implementation |
|----------|---------------------|-------------------------|
| Multiplication | O(N³) *(Cache Miss Heavy)* | O(N³) *(Cache Friendly / IKJ)* |
| Determinant | O(N!) *(Laplace Expansion)* | O(N³) *(LU Decomposition)* |
| Inverse | O(N!) *(Cofactors)* | O(N³) *(LU + Substitution)* |
| Memory Access | Pointer-to-Pointer (`double**`) | Contiguous Aligned Flat Array |
