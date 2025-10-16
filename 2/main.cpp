#include <iostream>

#include "seven.h"

int main() {
    std::string str;
    std::cin >> str;
    try {
        Seven::Seven s(str);
        Seven::Seven s1({'1'});
        Seven::Seven expect({'3'});
        Seven::Seven result(Seven::Seven::ADD(s, s1));
        std::cout << expect.EQ(result);
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}