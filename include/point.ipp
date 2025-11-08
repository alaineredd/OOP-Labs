#include "point.hpp"

namespace figure {

template<Scalar T>
Point<T>::Point(T x, T y) : x(x), y(y) {}

template<Scalar T>
bool Point<T>::operator==(const Point& other) const {
    if constexpr (std::floating_point<T>) {
        return std::abs(x - other.x) < 1e-9 && std::abs(y - other.y) < 1e-9;
    } else {
        return x == other.x && y == other.y;
    }
}

template<Scalar T>
std::ostream& operator<<(std::ostream& os, const Point<T>& point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}

template<Scalar T>
std::istream& operator>>(std::istream& is, Point<T>& point) {
    is >> point.x >> point.y;
    return is;
}

} // namespace figure