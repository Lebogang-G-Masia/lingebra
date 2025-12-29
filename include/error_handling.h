#ifndef LINGEBRA_ERROR_HANDLING
#define LINGEBRA_ERROR_HANDLING

#include <exception>

class AdditionException : public std::exception {
    public:
        AdditionException() {}
        // override the what() function
        const char* what() const noexcept override {
            return "Excepetion Caught: Cannot add matrices of different dimensions.";
        }
};



#endif // LINGEBRA_ERROR_HANDLING
