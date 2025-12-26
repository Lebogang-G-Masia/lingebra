#include <iostream>
#include "../include/vector.h"

int main() {
    double data[] {1, 2, 3, 4, 5 };
    vector v1(data);
    v1[0] = 5.5;
    std::cout << v1 << std::endl;
    return 0;
}
