#ifndef LINGEBRA_VECTOR_H
#define LINGEBRA_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <ostream>

namespace Lingebra {
    class Vector {
        private:
            std::size_t length;
            double* data;
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

            // Friends
            friend std::ostream& operator<<(std::ostream&, const Vector&);
    };
    std::ostream& operator<<(std::ostream&, const Vector&);
}

#endif // LINGEBRA_VECTOR_H
