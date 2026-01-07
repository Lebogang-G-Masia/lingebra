#include <iostream>
#include <algorithm>
#include <stdexcept>

#include "../include/vector.h"

namespace Lingebra {
    // Constructors
    Vector::Vector() : length(0), data(nullptr) {}

    Vector::Vector(std::size_t size) : length(size) {
        data = new double[size];
    }

    Vector::Vector(std::initializer_list<double> input) : length(input.size()) {
        data = new double[input.size()];
        std::copy(input.begin(), input.end(), data);
    }

    // rule of 5 stuff

    // 1.
    Vector::~Vector() {
        delete[] data;
    }
    // 2.
    Vector::Vector(const Vector& other) : length(other.length) {
        data = new double[other.length];
        std::copy(other.data, other.data + other.length, data);
    }
    // 3.
    Vector::Vector(Vector&& other) noexcept : length(other.length) {
        data = other.data;
        other.data = nullptr;
        other.length = 0;
    }
    // 4.
    Vector& Vector::operator=(const Vector& other) {
        if (this == &other) return *this;
        if (length == other.length) {
            std::copy(other.data, other.data + other.length, data);
            return *this;
        }
        double* temp = new double[other.length];
        std::copy(other.data, other.data + other.length, temp);
        delete[] data;
        data = temp;
        length = other.length;
        return *this;
    }
    // 5.
    Vector& Vector::operator=(Vector&& other) noexcept {
        if (this == &other) return *this;
        delete[] data;
        data = other.data;
        length = other.length;
        other.data = nullptr;
        other.length = 0;
        
        return *this;
    }

    // Accessor
    std::size_t Vector::size() const {
        return length;
    }

    // Overloading stuff
    double& Vector::operator[](std::size_t i) {
        return data[i];
    }

    const double& Vector::operator[](std::size_t i) const {
        return data[i];
    }

    const double& Vector::at(std::size_t i) const {
        if (i >= length) throw std::out_of_range("Index out of bounds");
        return data[i];
    }

    std::ostream& operator<<(std::ostream& out, const Vector& v) {
        out << '[';
        for (std::size_t i { 0 }; i < v.length; i++) {
            out << v.data[i];
            if (i + 1 < v.length) out << ", ";
        }
        out << ']';

        return out;
    }
}
