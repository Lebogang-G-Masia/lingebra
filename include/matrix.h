#ifndef LINGEBRA_MATRIX_H
#define LINGEBRA_MATRIX_H

#include <initializer_list>
#include <algorithm>
#include <iostream>
#include <cstddef>
#include <string>
#include <cstdlib>
#include <cmath>
#include "vector.h"
#include "error_handling.h"

namespace Lingebra {
    class matrix {
        private:
            std::size_t nrows;
            std::size_t ncols;
            vector* data;
            bool is_square;
        public:
            matrix() : nrows(0), ncols(0), data(nullptr), is_square(false) {}

            matrix(std::size_t rows, std::size_t cols) : nrows(rows), ncols(cols) {
                is_square = nrows == ncols ? true : false;
                data = new vector[rows]();
                for (std::size_t i { 0 }; i < rows; i++) data[i] = vector(cols);
            }

            matrix(std::initializer_list<vector> input) : nrows(input.size()), ncols(input.begin()->size()) {
                is_square = nrows == ncols ? true : false;
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
                is_square = nrows == ncols ? true : false;
                data = new vector[nrows]();
                std::copy(other.data, other.data + nrows, data);
            }

            // 3.
            matrix(matrix&& other) noexcept : nrows(other.nrows), ncols(other.ncols) {
                is_square = nrows == ncols ? true : false;
                data = other.data; 
                other.data = nullptr;
                other.nrows = 0;
                other.ncols = 0;
            }

            // 4.
            matrix& operator=(const matrix& other) {
                is_square = nrows == ncols ? true : false;
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
                is_square = nrows == ncols ? true : false;
                other.data = nullptr;
                other.nrows = 0;
                other.ncols = 0;


                return *this;
            }

            const vector shape() const noexcept {
                return vector({static_cast<double>(nrows), static_cast<double>(ncols)});
            }

            // TODO: Write the following two functions in a one function

            const matrix operator+(const matrix& other) const {
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


            const matrix operator-(const matrix& other) const {
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

            const matrix operator*(const double& scalar) const {
                matrix prod(nrows, ncols);
                for (std::size_t i { 0 }; i < nrows; i++)
                    for (std::size_t j { 0 }; j < ncols; j++)
                        prod[i][j] = data[i][j] * scalar;

                return prod;
            }

            const matrix matmul(const matrix& other) const {
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

            // TODO: Remove the code repetition in the determinant and cofactor code
            
            static const matrix cofactors(const matrix& mat) const {
                matrix C(mat.nrows, mat.ncols);

                std::size_t row { 0 };
                std::size_t col { 0 };

                for (std::size_t i { 0 }; i < mat.nrows; i++) {
                    for (std::size_t j { 0 }; j < mat.ncols; j++) {
                        row = 0;
                        matrix m(mat.nrows-1, mat.ncols-1);
                        for (std::size_t r { 0 }; r < mat.nrows; r++) {
                            col = 0;
                            for (std::size_t c { 0 }; c < mat.ncols; c++) {
                                if (r == i || c == j) continue;
                                m[row][col] = mat[r][c];
                                if (col == mat.ncols - 2) row += 1;
                                else col += 1;
                            }
                        }
                        double s = std::pow(-1, i+j)*determinant(m);
                        C[i][j] = s; 
                    }
                }
                return C;
            }

            static const double determinant(const matrix& mat, const std::size_t& i=0) const {
                double det { 0.0 };
                if (mat.nrows == 2) det = (mat[0][0]*mat[1][1]) - (mat[0][1]*mat[1][0]);
                else {
                    std::size_t row { 0 };
                    std::size_t col { 0 };
                    for (std::size_t j { 0 }; j < mat.ncols; j++) {
                        row = 0;
                        matrix m(mat.nrows-1, mat.ncols-1);
                        for (int r = 0; r < mat.nrows; r++) {
                            col = 0;
                            for (int c = 0; c < mat.ncols; c++) {
                                if (r == i || c == j) continue;
                                m[row][col] = mat[r][c];
                                if (col == mat.ncols - 2) row += 1;
                                else col += 1;
                            }
                        }
                        det += std::pow(-1, i+j)*mat[i][j]*determinant(m);
                        //std::cout << det << std::endl;
                    }
                }
                return det;
            }

            static const matrix transpose(const matrix& mat) const {
                matrix T(mat.nrows, mat.ncols);
                for (std::size_t i { 0 }; i < mat.nrows; i++)
                    for (std::size_t j { 0 }; j < mat.ncols; j++)
                        T[i][j] = mat[j][i];
                return T;
            }

            static const matrix inverse(const matrix& mat) const {
                matrix inv(mat.nrows, mat.ncols);
                
                try {
                    if (mat.nrows != mat.ncols) throw MatrixNotSquareException();
                    double det = determinant(mat);
                    std::cout << det << std::endl;
                    if (det == 0) throw MatrixNotInvertibleException();
                    matrix C = cofactors(mat);

                    inv = transpose(C)*(1/det);
                }

                catch (MatrixNotSquareException& ex) {
                    std::cerr << ex.what() << std::endl;
                    exit(EXIT_FAILURE);
                }

                catch (MatrixNotInvertibleException& ex) {
                    std::cerr << ex.what() << std::endl;
                    exit(EXIT_FAILURE);
                }

                return inv;
            }

            vector& operator[](std::size_t i) { return data[i]; };
            const vector& operator[](std::size_t i) const { return data[i]; }

            friend std::ostream& operator<<(std::ostream&, const matrix&);
    };

    inline std::ostream& operator<<(std::ostream& out, const matrix& mat) {
        const double tolerance = 1e-12;
    
        out << "Matrix(";
        for (std::size_t i { 0 }; i < mat.nrows; i++) {
            if (i != 0) out << "       ";
        
            out << "[";
            for (std::size_t j { 0 }; j < mat.ncols; j++) {
                double val = mat.data[i][j];
            
                if (std::abs(val) < tolerance) {
                    out << 0;
                } else {
                    out << val;
                }
            
                if (j + 1 < mat.ncols) out << ", ";
            }
            out << "]";

            if (i + 1 < mat.nrows) out << ", " << std::endl;
        }
        out << ")" << std::endl;
        return out;
    };
}

#endif // LINGEBRA_MATRIX_H
