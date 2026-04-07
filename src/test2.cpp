#include <iostream>
#include <cmath>
#include "../include/matrix.hpp"

using namespace Lingebra;

int main() {
    Matrix<float> m1 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    std::cout << m1 << std::endl;
    std::cout << "Determinant: " << m1.determinant() << std::endl;
    
    Matrix<float> m2 = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    Matrix<float> m3 = m1.matmul(m2);
    std::cout << "m1.matmul(identity):" << std::endl;
    std::cout << m3 << std::endl;

    return 0;
}
