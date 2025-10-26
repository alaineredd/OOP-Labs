#include <iostream>

#include "figure.h"

namespace figure{

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << "," << p.y << ")";
}

std::istream& operator>>(std::istream& is, Point& p) {
    is >> p.x >> p.y;
    return is;
}

bool operator==(const Point& p1, const Point& p2) {
    return std::abs(p1.x - p2.x) < EPSILON && std::abs(p1.y - p2.y) < EPSILON;
}

bool operator!=(const Point& p1, const Point& p2) {
    return !(p1==p2);
}

Point::Point() : x(0), y(0) {};

Point::Point(double cx, double cy) : x(cx), y(cy) {};

std::ostream &operator<<(std::ostream &out, const Figure &figure) {
    figure.Print(out);
    return out;
}

} // namespace figure