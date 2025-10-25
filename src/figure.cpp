#include <iostream>

#include "../include/figure.h"

namespace figure{

Point::Point() : x(0), y(0) {};

Point::Point(double cx, double cy) : x(cx), y(cy) {};

std::ostream &operator<<(std::ostream &out, const Figure &figure) {
    figure.Print(out);
    return out;
}

} // namespace figure