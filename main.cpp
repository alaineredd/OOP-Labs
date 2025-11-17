#include <iostream>

#include "rectangle.hpp"
#include "rhombus.hpp"
#include "trapezoid.hpp"

int main() {
    figure::Rhombus<double> s;
    std::cout << "Enter the rhombus points:" << std::endl;
    std::cin >> s;

    figure::Rectangle<double> r;
    std::cout << "Enter the rectangle points:" << std::endl;
    std::cin >> r;

    figure::Trapezoid<double> t;
    std::cout << "Enter the trapezoid points:" << std::endl;
    std::cin >> t;

    std::cout << "Rhombus: " << s << std::endl;
    std::cout << "Rectangle: " << r << std::endl;
    std::cout << "Trapezoid: " << t << std::endl;
    return 0;
}