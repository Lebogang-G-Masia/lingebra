#ifndef LINGEBRA_MATRIX_H
#define LINGEBRA_MATRIX_H

#include "vector.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <functional>
#include <immintrin.h>

namespace Lingebra {

    template <typename F>
    class Matrix {
        private:
            std::size_t nrows;
            std::size_t ncols;
            Vector<F> data;
        public:
            static bool lu_decomposition(Matrix& mat, std::vector<std::size_t>& pivots, int& pivot_sign) {
                if (mat.shape()[0] != mat.shape()[1]) return false;
                
                std::size_t n = static_cast<std::size_t>(mat.shape()[0]);
                F* A = mat.data_ptr();
                
                pivot_sign = 1;
                for (std::size_t i = 0; i < n; i++) pivots[i] = i;

                for (std::size_t k = 0; k < n; k++) {
                    F max_val = 0.0;
                    std::size_t best_row = k;
                    
                    for (std::size_t i = k; i < n; i++) {
                        F val = std::abs(A[i * n + k]);
                        if (val > max_val) {
                            max_val = val;
                            best_row = i;
                        }
                    }

                    if (max_val < 1e-12) return false; 

                    if (best_row != k) {
                        for (std::size_t j = 0; j < n; j++) 
                            std::swap(A[k * n + j], A[best_row * n + j]);
                        
                        std::swap(pivots[k], pivots[best_row]);
                        pivot_sign = -pivot_sign;
                    }

                    for (std::size_t i = k + 1; i < n; i++) {
                        A[i * n + k] /= A[k * n + k];
                        
                        for (std::size_t j = k + 1; j < n; j++) {
                            A[i * n + j] -= A[i * n + k] * A[k * n + j];
                        }
                    }
                }
                return true;
            }


            Matrix() : nrows(0), ncols(0), data(0) {}
            
            Matrix(std::size_t r, std::size_t c, bool randomize=false) : nrows(r), ncols(c), data(r * c, randomize) {}

            Matrix(std::initializer_list<std::initializer_list<F>> input) {
                nrows = input.size();
                if (nrows == 0) { ncols = 0; return; } 

                ncols = input.begin()->size();
                for (auto& row_list: input)
                    if (row_list.size() != ncols)
                        throw std::invalid_argument("Columns should be the same length");

                data = Vector<F>(nrows*ncols);
                std::size_t i = 0;
                for (auto& row_list: input)
                    for (F val: row_list)
                        data[i++] = val;
            }
            
            Matrix(std::vector<F> input) : nrows(input.size()), ncols(1) {
                for (std::size_t i = 0; i < input.size(); i++) {
                    data[i] = input[i];
                }
            }

            ~Matrix() {}

            Matrix(const Matrix& other) : 
                nrows(other.nrows),
                ncols(other.ncols),
                data(other.data) 
        {}

            Matrix(Matrix&& other) noexcept :
                nrows(other.nrows),
                ncols(other.ncols),
                data(std::move(other.data)) {
                    other.nrows = 0;
                    other.ncols = 0;
                }

            Matrix operator=(const Matrix& other) {
                if (this == &other) return *this;
                if (other.nrows == nrows && other.ncols == ncols) {
                    Matrix tmp(other);
                    std::swap(*this, tmp);
                }
                return *this;
            }

            Matrix operator=(Matrix&& other) noexcept {
                if (this == &other) return *this;
                data = std::move(other.data);
                nrows = other.nrows;
                ncols = other.ncols;
                other.nrows = 0;
                other.ncols = 0;
                return *this;
            }
            
            const Vector<std::size_t> shape() const noexcept {
                return Vector{static_cast<std::size_t>(nrows), static_cast<std::size_t>(ncols)};
            }
            
            F* data_ptr() { return data.data_ptr(); }
            const F* data_ptr() const { return data.data_ptr(); }

            F& operator()(std::size_t i, std::size_t j) { return data[i * ncols + j]; }
            const F& operator()(std::size_t i, std::size_t j) const { return data[i * ncols + j]; }

            Matrix& operator+=(const Matrix& other) {
                if (nrows != other.nrows || ncols != other.ncols)
                    throw std::invalid_argument("Input dimensions should be the same size.");
                
                std::size_t total = nrows * ncols;
                F* a_ptr = data.data_ptr();
                const F* b_ptr = other.data.data_ptr();

                if constexpr (std::is_same_v<F, float>) {
                    std::size_t i = 0;
                    for (; i + 7 < total; i += 8) {
                        __m256 a = _mm256_load_ps(a_ptr + i);
                        __m256 b = _mm256_load_ps(b_ptr + i);
                        __m256 sum = _mm256_add_ps(a, b);
                        _mm256_store_ps(a_ptr + i, sum);
                    }
                    for (; i < total; i++) a_ptr[i] += b_ptr[i];
                } else if constexpr (std::is_same_v<F, double>) {
                    std::size_t i = 0;
                    for (; i + 3 < total; i += 4) {
                        __m256d a = _mm256_load_pd(a_ptr + i);
                        __m256d b = _mm256_load_pd(b_ptr + i);
                        __m256d sum = _mm256_add_pd(a, b);
                        _mm256_store_pd(a_ptr + i, sum);
                    }
                    for (; i < total; i++) a_ptr[i] += b_ptr[i];
                } else {
                    for (std::size_t i = 0; i < total; i++) a_ptr[i] += b_ptr[i];
                }
                return *this;
            }

            Matrix& operator-=(const Matrix& other) {
                if (nrows != other.nrows || ncols != other.ncols)
                    throw std::invalid_argument("Input dimensions should be the same size.");
                
                std::size_t total = nrows * ncols;
                F* a_ptr = data.data_ptr();
                const F* b_ptr = other.data.data_ptr();

                if constexpr (std::is_same_v<F, float>) {
                    std::size_t i = 0;
                    for (; i + 7 < total; i += 8) {
                        __m256 a = _mm256_load_ps(a_ptr + i);
                        __m256 b = _mm256_load_ps(b_ptr + i);
                        __m256 diff = _mm256_sub_ps(a, b);
                        _mm256_store_ps(a_ptr + i, diff);
                    }
                    for (; i < total; i++) a_ptr[i] -= b_ptr[i];
                } else if constexpr (std::is_same_v<F, double>) {
                    std::size_t i = 0;
                    for (; i + 3 < total; i += 4) {
                        __m256d a = _mm256_load_pd(a_ptr + i);
                        __m256d b = _mm256_load_pd(b_ptr + i);
                        __m256d diff = _mm256_sub_pd(a, b);
                        _mm256_store_pd(a_ptr + i, diff);
                    }
                    for (; i < total; i++) a_ptr[i] -= b_ptr[i];
                } else {
                    for (std::size_t i = 0; i < total; i++) a_ptr[i] -= b_ptr[i];
                }
                return *this;
            }

            Matrix& operator*=(const double scalar) {
                std::size_t total = nrows * ncols;
                F* a_ptr = data.data_ptr();

                if constexpr (std::is_same_v<F, float>) {
                    float s = static_cast<float>(scalar);
                    __m256 s_vec = _mm256_set1_ps(s);
                    std::size_t i = 0;
                    for (; i + 7 < total; i += 8) {
                        __m256 a = _mm256_load_ps(a_ptr + i);
                        __m256 prod = _mm256_mul_ps(a, s_vec);
                        _mm256_store_ps(a_ptr + i, prod);
                    }
                    for (; i < total; i++) a_ptr[i] *= s;
                } else if constexpr (std::is_same_v<F, double>) {
                    __m256d s_vec = _mm256_set1_pd(scalar);
                    std::size_t i = 0;
                    for (; i + 3 < total; i += 4) {
                        __m256d a = _mm256_load_pd(a_ptr + i);
                        __m256d prod = _mm256_mul_pd(a, s_vec);
                        _mm256_store_pd(a_ptr + i, prod);
                    }
                    for (; i < total; i++) a_ptr[i] *= scalar;
                } else {
                    for (std::size_t i = 0; i < total; i++) a_ptr[i] *= scalar;
                }
                return *this;
            }

            Matrix operator+(const Matrix& other) { Matrix r = *this; r += other; return r; }
            Matrix operator-(const Matrix& other) { Matrix r = *this; r -= other; return r; }
            Matrix operator*(const double scalar) { Matrix r = *this; r *= scalar; return r; }

            Matrix matmul(const Matrix& other) const {
                if (ncols != other.nrows)
                    throw std::invalid_argument("Dimension mismatch: Matrix A cols must equal Matrix B rows.");

                Matrix result(nrows, other.ncols);
                const F* A_ptr = data.data_ptr();
                const F* B_ptr = other.data.data_ptr();
                F* C_ptr = result.data.data_ptr();

                const std::size_t M = nrows;
                const std::size_t K = ncols;
                const std::size_t N = other.ncols;

                if constexpr (std::is_same_v<F, float>) {
                    for (std::size_t i = 0; i < M; i++) {
                        const std::size_t row_A = i * K;
                        const std::size_t row_C = i * N;
                        for (std::size_t k = 0; k < K; k++) {
                            const float r_val = A_ptr[row_A + k];
                            if (r_val == 0.0f) continue;
                            __m256 a_vec = _mm256_set1_ps(r_val);
                            const std::size_t row_B = k * N;
                            std::size_t j = 0;
                            for (; j + 7 < N; j += 8) {
                                __m256 b_vec = _mm256_load_ps(B_ptr + row_B + j);
                                __m256 c_vec = _mm256_load_ps(C_ptr + row_C + j);
                                c_vec = _mm256_fmadd_ps(a_vec, b_vec, c_vec);
                                _mm256_store_ps(C_ptr + row_C + j, c_vec);
                            }
                            for (; j < N; j++) {
                                C_ptr[row_C + j] += r_val * B_ptr[row_B + j];
                            }
                        }
                    }
                } else {
                    for (std::size_t i = 0; i < M; i++) {
                        const std::size_t row_A = i * K;
                        const std::size_t row_C = i * N;
                        for (std::size_t k = 0; k < K; k++) {
                            const F r_val = A_ptr[row_A + k];
                            if (r_val == 0.0) continue; 
                            const std::size_t row_B = k * N;
                            for (std::size_t j = 0; j < N; j++) {
                                C_ptr[row_C + j] += r_val * B_ptr[row_B + j];
                            }
                        }
                    }
                }
                return result;
            }

            static Matrix matmul(const Matrix& A, const Matrix& B) {
                return A.matmul(B);
            }

            Matrix element_wise(const Matrix& other) const {
                if (nrows != other.nrows || ncols != other.ncols)
                    throw std::invalid_argument("Dimension mismatch: Matrices should have the same dimsions.");

                Matrix result(nrows, ncols);
                std::size_t dims = nrows * ncols;
                for (std::size_t i = 0; i < dims; i++) {
                    result.data[i] = data[i] * other.data[i];
                }
                return result;
            }

            F sum() const {
                F total = 0.0;
                std::size_t dims = nrows * ncols;
                for (std::size_t i = 0; i < dims; i++)
                    total += data[i];
                return total;
            }

            Matrix map(std::function<F(F)> func) const {
                Matrix result(nrows, ncols);
                std::size_t dims = nrows * ncols;
                for (std::size_t i = 0; i < dims; i++)
                    result.data[i] = func(data[i]);
                return result;
            }

            Matrix sum_rows() const {
                Matrix result(1, ncols);
                for (std::size_t i = 0; i < nrows; i++)
                    for (std::size_t j = 0; j < ncols; j++)
                        result.data[j] += data[i * ncols + j];
                return result;
            }

            double determinant() const {
                if (nrows != ncols) throw std::invalid_argument("Matrix must be square");

                Matrix lu = *this;
                std::vector<std::size_t> pivots(nrows);
                int sign = 1;

                if (!lu_decomposition(lu, pivots, sign)) return 0.0; 

                double det = static_cast<double>(sign);
                const F* ptr = lu.data.data_ptr();
                
                for (std::size_t i = 0; i < nrows; i++) {
                    det *= ptr[i * ncols + i];
                }
                return det;
            }

            Matrix inverse() const {
                if (nrows != ncols) throw std::invalid_argument("Matrix must be square");

                Matrix lu = *this;
                std::vector<std::size_t> pivots(nrows);
                int sign;

                if (!lu_decomposition(lu, pivots, sign)) 
                    throw std::runtime_error("Matrix is singular (det=0) and cannot be inverted.");

                Matrix inv(nrows, ncols);
                const F* lu_ptr = lu.data.data_ptr();
                F* inv_ptr = inv.data.data_ptr();
                std::size_t n = nrows;

                std::vector<F> b(n);
                
                for (std::size_t j = 0; j < n; j++) {
                    for (std::size_t i = 0; i < n; i++) {
                        b[i] = (pivots[i] == j) ? 1.0 : 0.0;
                    }

                    for (std::size_t i = 0; i < n; i++) {
                        for (std::size_t k = 0; k < i; k++) {
                            b[i] -= lu_ptr[i * n + k] * b[k];
                        }
                    }

                    for (long i = n - 1; i >= 0; i--) {
                        for (std::size_t k = i + 1; k < n; k++) {
                            b[i] -= lu_ptr[i * n + k] * b[k];
                        }
                        b[i] /= lu_ptr[i * n + i];
                    }

                    for (std::size_t i = 0; i < n; i++) {
                        inv_ptr[i * n + j] = b[i];
                    }
                }
                return inv;
            }

            Matrix transpose() const {
                Matrix T(ncols, nrows);
                for (std::size_t i = 0; i < nrows; i++)
                    for (std::size_t j = 0; j < ncols; j++)
                        T.data[j * nrows + i] = data[i * ncols + j];
                return T;
            }

            Matrix cofactors() const {
                double det = determinant();
                if (std::abs(det) < 1e-12) throw std::runtime_error("Singular matrix has no cofactors defined in this context.");
                
                Matrix inv = inverse();
                return inv.transpose() * det;
            }

            friend std::ostream& operator<<(std::ostream& out, const Matrix m) {
                const double tolerance = 1e-10;
                out << "Matrix(\n";
                for (std::size_t i { 0 }; i < m.nrows; i++) {
                    out << "  [";
                    for (std::size_t j { 0 }; j < m.ncols; j++) {
                        F val = m.data[i * m.ncols + j];
                        if (std::abs(val) < tolerance) val = 0.0;
                        out << val; 
                        if (j + 1 < m.ncols) out << ", ";
                    }
                    out << "]";
                    if (i + 1 < m.nrows) out << ",\n";
                }
                out << "\n)";
                return out;
            }
    };
}


#endif // LINGEBRA_MATRIX_H
