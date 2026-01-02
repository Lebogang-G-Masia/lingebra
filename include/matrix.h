#ifndef LINGEBRA_MATRIX_H
#define LINGEBRA_MATRIX_H

#include <initializer_list>
#include <algorithm>
#include <iostream>
#include <cstddef>
#include <string>
#include <cstdlib>
#include "vector.h"
#include "error_handling.h"

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

            const vector shape() const noexcept {
                return vector({static_cast<double>(nrows), static_cast<double>(ncols)});
            }

            // TODO: Write the following two functions in a one function

            matrix operator+(matrix other) {
                matrix sum(nrows, ncols);

                try {
                    if (nrows != other.nrows || ncols != other.ncols) {
                        throw AdditionException();
                    }

                    for (std::size_t i { 0 }; i < nrows; i++) {
                        for (std::size_t j { 0 }; j < ncols; j++) {
                            sum[i][j] = data[i][j] + other.data[i][j];
                        }
                    }
                }

                catch (AdditionException& ex) {
                    std::cerr << ex.what() << std::endl;
                    exit(EXIT_FAILURE);
                }
                return sum;
            }


            matrix operator-(matrix other) {
                matrix diff(nrows, ncols);

                try {
                    if (nrows != other.nrows || ncols != ncols) {
                        throw SubtractionException();
                    }

                    for (std::size_t i { 0 }; i < nrows; i++) {
                        for (std::size_t j { 0 }; j < ncols; j++) {
                            diff[i][j] = data[i][j] - other.data[i][j];
                        }
                    }
                }

                catch (SubtractionException& ex) {
                    std::cerr << ex.what() << std::endl;
                    exit(EXIT_FAILURE);
                }
                return diff;
            }

            matrix operator*(int scalar) {
                matrix prod(nrows, ncols);
                for (std::size_t i { 0 }; i < nrows; i++)
                    for (std::size_t j { 0 }; j < ncols; j++)
                        prod[i][j] = data[i][j] * scalar;

                return prod;
            }

            matrix matmul(matrix other) {
                matrix product(nrows, other.ncols);
                try {
                    if (ncols != other.nrows) throw MultiplicationException();    
                    for (std::size_t r { 0 }; r < nrows; r++) {
                        for (std::size_t c { 0 }; c < other.ncols; c++) {
                            double sum { 0 };
                            for (std::size_t i { 0 }; i < ncols; i++) {
                                sum += data[r][i]*other.data[i][c];
                            }
                            product[r][c] = sum;
                        }
                    }
                }

                catch (MultiplicationException& ex) {
                    std::cerr << ex.what() << std::endl;
                    exit(EXIT_FAILURE);
                }

                return product;
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
        out << ")" << std::endl;
        return out;
    };
}

#endif // LINGEBRA_MATRIX_H
