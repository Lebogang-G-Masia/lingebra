#ifndef LINGEBRA_ERROR_HANDLING
#define LINGEBRA_ERROR_HANDLING

#include <exception>

namespace Lingebra {
    class AdditionException : public std::exception {
        public:
            AdditionException() {}
            // override the what() function
            const char* what() const noexcept override {
                return "AdditionException: Cannot add matrices of different dimensions.";
            }
    };
    
    class SubtractionException : public std::exception {
        public:
            SubtractionException() {}
            const char* what() const noexcept override {
                return "SubtractionException: Cannot subtract matrices of different dimensions.";
            }
    };

    class MultiplicationException : public std::exception {
        public:
            MultiplicationException() {}
            const char* what() const noexcept override {
                return "MultiplicationException: Invalid input dimensions for matrix multiplication.";      
            }
    };

    class MatrixNotSquareException: public std::exception {
        public:
            MatrixNotSquareException() {}
            const char* what() const noexcept override {
                return "MatrixNotSquareException: The matrix is not square";
            }
    };

    class MatrixNotInvertibleException: public std::exception {
        public:
            MatrixNotInvertibleException() {}
            const char* what() const noexcept override {
                return "MatrixNotInvertibleExption: The matrix is not invertible";
            }
    };
}





#endif // LINGEBRA_ERROR_HANDLING
