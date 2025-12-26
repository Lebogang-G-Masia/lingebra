#include <iostream>
#include "../include/vector.h"


// overload the << operator to be able to print out the vector in a neat way.
std::ostream& operator<<(std::ostream& out, const vector& v) {
    out << "[ ";
    for (int i { 0 }; i < v.length; i++) out << v.data[i] << ' ';
    out << ']';
    return out;
}
