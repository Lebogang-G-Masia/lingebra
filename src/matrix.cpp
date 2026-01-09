#include "../include/matrix.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

namespace Lingebra {

    static bool lu_decomposition(Matrix& mat, std::vector<std::size_t>& pivots, int& pivot_sign) {
        if (mat.shape()[0] != mat.shape()[1]) return false;
        
        std::size_t n = static_cast<std::size_t>(mat.shape()[0]);
        double* A = mat.data_ptr();
        
        pivot_sign = 1;
        for (std::size_t i = 0; i < n; i++) pivots[i] = i;

        for (std::size_t k = 0; k < n; k++) {
            double max_val = 0.0;
            std::size_t best_row = k;
            
            for (std::size_t i = k; i < n; i++) {
                double val = std::abs(A[i * n + k]);
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


    Matrix::Matrix() : nrows(0), ncols(0), data(0) {}
    
    Matrix::Matrix(std::size_t r, std::size_t c) : nrows(r), ncols(c), data(r * c) {}

    Matrix::Matrix(std::initializer_list<std::initializer_list<double>> input) {
        nrows = input.size();
        if (nrows == 0) { ncols = 0; return; } 

        ncols = input.begin()->size();
        for (auto& row_list: input)
            if (row_list.size() != ncols)
                throw std::invalid_argument("Columns should be the same length");

        data = Vector(nrows*ncols);
        std::size_t i = 0;
        for (auto& row_list: input)
            for (double val: row_list)
                data[i++] = val;
    }
    
    const Vector Matrix::shape() const noexcept {
        return Vector{static_cast<double>(nrows), static_cast<double>(ncols)};
    }
    
    double* Matrix::data_ptr() { return data.data_ptr(); }
    const double* Matrix::data_ptr() const { return data.data_ptr(); }

    double& Matrix::operator()(std::size_t i, std::size_t j) { return data[i * ncols + j]; }
    const double& Matrix::operator()(std::size_t i, std::size_t j) const { return data[i * ncols + j]; }

    Matrix& Matrix::operator+=(const Matrix& other) {
        if (nrows != other.nrows || ncols != other.ncols)
            throw std::invalid_argument("Input dimensions should be the same size.");
        std::size_t total = nrows * ncols;
        for (std::size_t i = 0; i < total; i++) data[i] += other.data[i];
        return *this;
    }

    Matrix& Matrix::operator-=(const Matrix& other) {
        if (nrows != other.nrows || ncols != other.ncols)
            throw std::invalid_argument("Input dimensions should be the same size.");
        std::size_t total = nrows * ncols;
        for (std::size_t i = 0; i < total; i++) data[i] -= other.data[i];
        return *this;
    }

    Matrix& Matrix::operator*=(const double scalar) {
        std::size_t total = nrows * ncols;
        for (std::size_t i = 0; i < total; i++) data[i] *= scalar;
        return *this;
    }

    Matrix Matrix::operator+(const Matrix& other) { Matrix r = *this; r += other; return r; }
    Matrix Matrix::operator-(const Matrix& other) { Matrix r = *this; r -= other; return r; }
    Matrix Matrix::operator*(const double scalar) { Matrix r = *this; r *= scalar; return r; }

    Matrix Matrix::matmul(const Matrix& other) const {
        if (ncols != other.nrows)
            throw std::invalid_argument("Dimension mismatch: Matrix A cols must equal Matrix B rows.");

        Matrix result(nrows, other.ncols);
        const double* A_ptr = data.data_ptr();
        const double* B_ptr = other.data.data_ptr();
        double* C_ptr = result.data.data_ptr();

        const std::size_t M = nrows;
        const std::size_t K = ncols;
        const std::size_t N = other.ncols;

        for (std::size_t i = 0; i < M; i++) {
            const std::size_t row_A = i * K;
            const std::size_t row_C = i * N;
            for (std::size_t k = 0; k < K; k++) {
                const double r_val = A_ptr[row_A + k];
                if (r_val == 0.0) continue; 
                const std::size_t row_B = k * N;
                for (std::size_t j = 0; j < N; j++) {
                    C_ptr[row_C + j] += r_val * B_ptr[row_B + j];
                }
            }
        }
        return result;
    }

    double Matrix::determinant() const {
        if (nrows != ncols) throw std::invalid_argument("Matrix must be square");

        Matrix lu = *this;
        std::vector<std::size_t> pivots(nrows);
        int sign = 1;

        if (!lu_decomposition(lu, pivots, sign)) return 0.0; 

        double det = static_cast<double>(sign);
        const double* ptr = lu.data.data_ptr();
        
        for (std::size_t i = 0; i < nrows; i++) {
            det *= ptr[i * ncols + i];
        }
        return det;
    }

    Matrix Matrix::inverse() const {
        if (nrows != ncols) throw std::invalid_argument("Matrix must be square");

        Matrix lu = *this;
        std::vector<std::size_t> pivots(nrows);
        int sign;

        if (!lu_decomposition(lu, pivots, sign)) 
            throw std::runtime_error("Matrix is singular (det=0) and cannot be inverted.");

        Matrix inv(nrows, ncols);
        const double* lu_ptr = lu.data.data_ptr();
        double* inv_ptr = inv.data.data_ptr();
        std::size_t n = nrows;

        std::vector<double> b(n);
        
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

    Matrix Matrix::transpose() const {
        Matrix T(ncols, nrows);
        for (std::size_t i = 0; i < nrows; i++)
            for (std::size_t j = 0; j < ncols; j++)
                T.data[j * nrows + i] = data[i * ncols + j];
        return T;
    }

    Matrix Matrix::cofactors() const {
        double det = determinant();
        if (std::abs(det) < 1e-12) throw std::runtime_error("Singular matrix has no cofactors defined in this context.");
        
        Matrix inv = inverse();
        return inv.transpose() * det;
    }

    std::ostream& operator<<(std::ostream& out, const Matrix& m) {
        const double tolerance = 1e-10;
        out << "Matrix(\n";
        for (std::size_t i { 0 }; i < m.nrows; i++) {
            out << "  [";
            for (std::size_t j { 0 }; j < m.ncols; j++) {
                double val = m.data[i * m.ncols + j];
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
}
