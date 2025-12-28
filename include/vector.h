#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <utility>
#include <ostream>
#include <stdexcept>

namespace Lingebra {
    class vector {
        private:
            std::size_t length;
            double* data;
        public:
            vector() : length(0), data(nullptr) {}

            explicit vector(std::size_t size) : length(size) {
                data = new double[size]();
            }

            vector(std::initializer_list<double> input) : length(input.size()) {
                data = new double[input.size()];
                std::copy(input.begin(), input.end(), data);
            }

            // rule of 5 stuff

            // 1.
            ~vector() { delete[] data; }

            // 2.
            vector(const vector& other) : length(other.length) {
                data = new double[other.length];
                std::copy(other.data, other.data + length, data);
            }

            // 3.
            vector(vector&& other) noexcept : length(other.length) {
                data = other.data;
                other.data = nullptr;
                other.length = 0;
            }

            // 4.
            vector& operator=(const vector& other) {
                if (this == &other) return *this;
                double* temp = new double[other.length];
                std::copy(other.data, other.data + other.length, temp);
                delete[] data;
                data = temp;
                length = other.length;
                return *this;
            }

            // 5.
            vector& operator=(vector&& other) noexcept {
                if (this == &other) return *this;
                delete[] data;
                data = other.data;
                length = other.length;

                other.data = nullptr;
                other.length = 0;
                return *this;
            }

            std::size_t size() const { return length; }

            double& operator[](std::size_t i) { return data[i]; }
            const double& operator[](std::size_t i) const { return data[i]; }

            friend std::ostream& operator<<(std::ostream&, const vector&);
    };

    inline std::ostream& operator<<(std::ostream& out, const vector& v) {
        out << "Vector([";
        for (std::size_t i { 0 }; i < v.length; i++) {
            out << v.data[i];
            if (i + 1 < v.length) out << ", ";
        }
        out << "])";

        return out;
    }
}

#endif // VECTOR_H
