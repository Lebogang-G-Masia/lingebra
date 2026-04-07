#include <iostream>
#include <chrono>
#include <vector>
#include "../include/matrix.hpp"

using namespace Lingebra;

void benchmark_matmul(size_t N) {
    Matrix<float> A(N, N, true);
    Matrix<float> B(N, N, true);

    auto start = std::chrono::high_resolution_clock::now();
    Matrix<float> C = A.matmul(B);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start;
    std::cout << "Matmul (" << N << "x" << N << "): " << diff.count() << " s" << std::endl;
}

void benchmark_add(size_t N) {
    Matrix<float> A(N, N, true);
    Matrix<float> B(N, N, true);

    auto start = std::chrono::high_resolution_clock::now();
    A += B;
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start;
    std::cout << "Addition (" << N << "x" << N << "): " << diff.count() << " s" << std::endl;
}

int main() {
    std::cout << "Starting benchmarks..." << std::endl;
    benchmark_add(2048);
    benchmark_matmul(512);
    benchmark_matmul(1024);
    return 0;
}
