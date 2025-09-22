#include "gcd.h"
#include <iostream>

int main() {
    int64_t first;
    int64_t second;
    std::cout << "Введите два числа для поиска НОД\n";
    if(!(std::cin >> first >> second)) {
        std::cerr << "Input Error";
        return 1;
    }

    int64_t gdivisor = gcd::gcd(first, second);

    std::cout << gdivisor;
    return 0;
}