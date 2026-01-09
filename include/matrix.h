#ifndef LINGEBRA_MATRIX_H
#define LINGEBRA_MATRIX_H

#include "vector.h"

namespace Lingebra {
    class Matrix {
        private:
            std::size_t nrows;
            std::size_t ncols;
            Vector data;
        public:
            Matrix();
            Matrix(std::size_t, std::size_t);
            Matrix(std::initializer_list<std::initializer_list<double>>);

            const Vector shape() const noexcept;

            Matrix& operator+=(const Matrix&);
            Matrix& operator-=(const Matrix&);
            Matrix& operator*=(const double);

            Matrix operator+(const Matrix&);
            Matrix operator-(const Matrix&);
            Matrix operator*(const double);

            // Matrix operations
            Matrix matmul(const Matrix&) const;
            Matrix cofactors() const;
            double determinant() const;
            Matrix transpose() const;
            Matrix inverse() const;

            double& operator()(std::size_t i, std::size_t j);
            const double& operator()(std::size_t i, std::size_t j) const;

            friend std::ostream& operator<<(std::ostream&, const Matrix&);
    };
    std::ostream& operator<<(std::ostream&, const Matrix&);
}

#endif // LINGEBRA_MATRIX_H
