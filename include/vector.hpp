#ifndef LINGEBRA_VECTOR_H
#define LINGEBRA_VECTOR_H

#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>

namespace Lingebra {
    template<typename T>
    class Vector {
        private:
            std::size_t length;
            T* data;

            T* allocate_aligned(std::size_t count) {
                if (count == 0) return nullptr;
                std::size_t bytes = count * sizeof(T);

                std::size_t padded_bytes = (bytes + 31) & ~31;

                void* ptr = std::aligned_alloc(32, padded_bytes);
                if (!ptr) throw std::bad_alloc();

                return static_cast<T*>(ptr);
            }

            void free_aligned(T* ptr) {
                std::free(ptr);
            }
        
        public:
            // Constructors
            Vector() : length(0), data(nullptr) {}

            Vector(std::size_t size) : length(size) {
                data = allocate_aligned(size);
                std::fill(data, data+length, 0.0);
            }

            Vector(std::initializer_list<T> input) : length(input.size()) {
                data = allocate_aligned(input.size());
                std::copy(input.begin(), input.end(), data);
            }

            // rule of 5 stuff

            // 1.
            ~Vector() {
                free_aligned(data);
            }
            // 2.
            Vector(const Vector& other) : length(other.length) {
                data = allocate_aligned(other.length);
                std::copy(other.data, other.data + other.length, data);
            }
            // 3.
            Vector(Vector&& other) noexcept : length(other.length) {
                data = other.data;
                other.data = nullptr;
                other.length = 0;
            }
            // 4.
            Vector& operator=(const Vector& other) {
                if (this == &other) return *this;
                if (length == other.length) {
                    std::copy(other.data, other.data + other.length, data);
                    return *this;
                }
                T* temp = allocate_aligned(other.length);
                std::copy(other.data, other.data + other.length, temp);
                free_aligned(data);
                data = temp;
                length = other.length;
                return *this;
            }
            // 5.
            Vector& operator=(Vector&& other) noexcept {
                if (this == &other) return *this;
                free_aligned(data);
                data = other.data;
                length = other.length;
                other.data = nullptr;
                other.length = 0;
                
                return *this;
            }

            // Accessor
            std::size_t size() const {
                return length;
            }

            // Overloading stuff
            T& operator[](std::size_t i) {
                return data[i];
            }

            const T& operator[](std::size_t i) const {
                return data[i];
            }

            const T& at(std::size_t i) const {
                if (i >= length) throw std::out_of_range("Index out of bounds");
                return data[i];
            }

            T* data_ptr() { return data; }
            const T* data_ptr() const { return data; }

            std::ostream& operator<<(std::ostream& out) {
                out << '[';
                for (std::size_t i { 0 }; i < length; i++) {
                    out << data[i];
                    if (i + 1 < length) out << ", ";
                }
                out << ']';

                return out;
            }
    };
}


#endif // LINGEBRA_VECTOR_H
