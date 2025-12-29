#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>
#include <algorithm>
#include <iostream>
#include <cstddef>
#include "vector.h"

namespace Lingebra {
    class matrix {
        private:
            std::size_t nrows;
            std::size_t ncols;
            vector* data;
        public:
            matrix() : nrows(0), ncols(0), data(nullptr) {}

            matrix(std::size_t rows, std::size_t cols) : nrows(rows), ncols(cols) {
                data = new vector[rows]();
                for (std::size_t i { 0 }; i < rows; i++) data[i] = vector(cols);
            }

            matrix(std::initializer_list<vector> input) : nrows(input.size()), ncols(input.begin()->size()) {
                data = new vector[input.size()];
                std::copy(input.begin(), input.end(), data);
            }

            // rule of 5 stuff, again...

            // 1.
            ~matrix() {
                delete[] data;
            }

            // 2.
            matrix(const matrix& other) : nrows(other.nrows), ncols(other.ncols) {
                data = new vector[nrows];
                std::copy(other.data, other.data + nrows, data);
            }

            // 3.
            matrix(matrix&& other) noexcept : nrows(other.nrows), ncols(other.ncols) {
                data = other.data; 
                other.data = nullptr;
                other.nrows = 0;
                other.ncols = 0;
            }

            // 4.
            matrix& operator=(const matrix& other) {
                if (this == &other) return *this;
                vector* temp = new vector[other.nrows];
                std::copy(other.data, other.data + other.nrows, temp);
                delete[] data;

                data = temp;
                nrows = other.nrows;
                ncols = other.ncols;

                return *this;
            }

            // 5.
            matrix& operator=(matrix&& other) noexcept {
                if (this == &other) return *this;
                delete[] data;
                data = other.data;
                nrows = other.nrows;
                ncols = other.ncols;

                other.data = nullptr;
                other.nrows = 0;
                other.ncols = 0;

                return *this;
            }

            vector& operator[](std::size_t i) { return data[i]; };

            const vector& operator[](std::size_t i) const { return data[i]; }

            friend std::ostream& operator<<(std::ostream&, const matrix&);
    };

    inline std::ostream& operator<<(std::ostream& out, const matrix& mat) {
        out << "Matrix(";
        for (std::size_t i { 0 }; i < mat.nrows; i++) {
            if (i == 0) out << mat.data[i];
            else out << "       " << mat.data[i];
            if (i + 1 < mat.nrows) out << ", " << std::endl;
        }
        out << ")";
        return out;
    }
}

#endif // MATRIX_H
