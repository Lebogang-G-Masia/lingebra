#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>

class vector {
    private:
        size_t length;
        double* data;
    public:
        // constructor incase for an empty vector
        vector(size_t size) : length(size) {
            data = new double[length]();
        }

        // constructor for a non-empty vector
        template<std::size_t N>
        vector(double (&input)[N]) {
            length = N;
            data = new double[length];
            std::copy(std::begin(input), std::end(input), data);
        }

        ~vector() {
            delete[] data;
        }

        vector(const vector& other) : length(other.length) {
            data = new double[length];
            for (size_t i { 0 }; i < length; i++) data[i] = other.data[i];
        }

        vector& operator=(const vector& other) {
            if (this == &other) return *this;

            delete[] data;
            length = other.length;
            data = new double[length];
            for (size_t i { 0 }; i < length; i++) data[i] = other.data[i];
            return *this;
        }

        double& operator[](int index) {
            return data[index];
        }

        const double& operator[](int index) const {
            return data[index];
        }

        friend std::ostream& operator<<(std::ostream&, const vector&);
};

std::ostream& operator<<(std::ostream&, const vector&);

#endif // VECTOR_H
