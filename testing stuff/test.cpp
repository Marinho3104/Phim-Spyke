
#include <iostream>

#define VALUE 100

int main() {

    std::cout << VALUE << std::endl;

    #include "test_1.cpp"

    std::cout << VALUE << std::endl;

}