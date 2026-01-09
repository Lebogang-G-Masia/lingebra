#include "../include/matrix.h"
#include <cmath>
#include <iostream>


namespace Lingebra {
    Matrix::Matrix() : nrows(0), ncols(0), data(0) {}
    
    Matrix::Matrix(std::size_t r, std::size_t c) : nrows(r), ncols(c), data(r * c) {}

    
    Matrix::Matrix(std::initializer_list<std::initializer_list<double>> input) {
        nrows = input.size();

       if (nrows == 0) {
          ncols = 0;
         return; // Data is already 0 from the default constructor
       } 

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
        const double rows = static_cast<double>(nrows);
        const double cols = static_cast<double>(ncols);
        return Vector{rows, cols};
    }

    double& Matrix::operator()(std::size_t i, std::size_t j) { return data[i * ncols + j]; }
    const double& Matrix::operator()(std::size_t i, std::size_t j) const { return data[i * ncols + j]; }

    Matrix& Matrix::operator+=(const Matrix& other) {
        if (nrows != other.nrows || ncols != other.ncols)
            throw std::invalid_argument("Input dimensions should be the same size.");
        
        std::size_t total_size { nrows * ncols };
        for (std::size_t i { 0 }; i < total_size; i++) data[i] += other.data[i];
        return *this;
    }

    Matrix& Matrix::operator-=(const Matrix& other) {
        if (nrows != other.nrows || ncols != other.ncols)
            throw std::invalid_argument("Input dimensions should be the same size.");
        
        std::size_t total_size { nrows * ncols };
        for (std::size_t i { 0 }; i < total_size; i++) data[i] -= other.data[i];
        return *this;
    }

    Matrix& Matrix::operator*=(const double scalar) {
        std::size_t total_size { nrows * ncols };
        for (std::size_t i { 0 }; i < total_size; i++) data[i] *= scalar;
        return *this;
    }

    Matrix Matrix::operator+(const Matrix& other) {
        Matrix result = *this;
        result += other;
        return result;
    }

    Matrix Matrix::operator-(const Matrix& other) {
        Matrix result = *this;
        result -= other;
        return result;
    }

    Matrix Matrix::operator*(const double scalar) {
        Matrix result = *this;
        result *= scalar;
        return result;
    }

    // optimized matrix multiplication using IKJ iterations instead of IJK iterations
    Matrix Matrix::matmul(const Matrix& other) const {
        if (ncols != other.nrows)
            throw std::invalid_argument("Invalid matrix dimensions. Matrix A cols should = Matrix B rows.");
        Matrix result(nrows, other.ncols);

        const double* A_ptr = this->data.data_ptr();
        const double* B_ptr = other.data.data_ptr();
        double* C_ptr = result.data.data_ptr();

        // dimensions as variables for cleaner code
        const std::size_t M = nrows;
        const std::size_t K = ncols; // common dimension
        const std::size_t N = other.ncols;

        // the row-inner-column (IKJ iterations) accesses B_ptr and C_ptr sequentially.
        for (std::size_t i { 0 }; i < M; i++) {
            const std::size_t row_A { i * K };
            const std::size_t row_C { i * N };
            for (std::size_t k { 0 }; k < K; k++) {
                const double r_val = A_ptr[row_A + k];
                std::size_t row_B { k * N };
                for (std::size_t j { 0 }; j < N; j++) {
                    C_ptr[row_C + j] += r_val * B_ptr[row_B + j];
                } 
            }
        }
        return result;
    } 

    std::ostream& operator<<(std::ostream& out, const Matrix& m) {
        const double tolerance = 1e-10;
        out << "Matrix(";
        for (std::size_t i { 0 }; i < m.nrows; i++) {
            if (i != 0) out << "       ";
            out << '[';
            for (std::size_t j { 0 }; j < m.ncols; j++) {
                double val = m.data[i * m.ncols + j];
                if (std::abs(val) < tolerance) out << 0;
                else out << val; 
                if (j + 1 < m.ncols) out << ", ";
            }
            out << ']';
            if (i + 1 < m.nrows) out << ", " << std::endl;
        }
        out << ')' << std::endl;
        return out;
    }
}
