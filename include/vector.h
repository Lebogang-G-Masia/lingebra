#ifndef LINGEBRA_VECTOR_H
#define LINGEBRA_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <ostream>

namespace Lingebra {
    class Vector {
        private:
            // member variables
            std::size_t length;
            double* data;
            // memory management stuff
            static double* allocate_aligned(std::size_t count);
            static void free_aligned(double* ptr);
        public:
            // Constructors
            Vector();

            Vector(std::size_t);

            Vector(std::initializer_list<double>);

            // rule of 5 stuff
            
            // 1.
            ~Vector();
            // 2.
            Vector(const Vector&);
            // 3.
            Vector(Vector&&) noexcept;
            // 4.
            Vector& operator=(const Vector&);
            // 5.
            Vector& operator=(Vector&&) noexcept;

            // Accessors
            std::size_t size() const;

            // Overloading stuff
            double& operator[](std::size_t);        
            const double& operator[](std::size_t) const;
            
            const double& at(std::size_t) const;

            // an easy easy way to retrieve data
            double* data_ptr() { return data; }
            const double* data_ptr() const { return data; }

            // Friends
            friend std::ostream& operator<<(std::ostream&, const Vector&);
    };
    std::ostream& operator<<(std::ostream&, const Vector&);
}

#endif // LINGEBRA_VECTOR_H
