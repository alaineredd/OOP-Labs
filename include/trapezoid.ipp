#include <cmath>

#include "trapezoid.hpp"

namespace figure {

template<Scalar T>
Trapezoid<T>::Trapezoid() 
    : vertices{
        std::make_unique<Point<T>>(0, 0),
        std::make_unique<Point<T>>(1, 0),
        std::make_unique<Point<T>>(1, 1),
        std::make_unique<Point<T>>(0, 1)
    } {}

template<Scalar T>
Trapezoid<T>::Trapezoid(const Point<T>& p1, const Point<T>& p2, const Point<T>& p3, const Point<T>& p4)
    : vertices{
        std::make_unique<Point<T>>(p1),
        std::make_unique<Point<T>>(p2),
        std::make_unique<Point<T>>(p3),
        std::make_unique<Point<T>>(p4)
    } {}

template<Scalar T>
Trapezoid<T>::Trapezoid(const Trapezoid& other) {
    for (int i = 0; i < FOUR_VERTICES; ++i) {
        vertices[i] = std::make_unique<Point<T>>(*other.vertices[i]);
    }
}

template<Scalar T>
Trapezoid<T>::Trapezoid(Trapezoid&& other) noexcept {
    for (int i = 0; i < FOUR_VERTICES; ++i) {
        vertices[i] = std::move(other.vertices[i]);
    }
}

template<Scalar T>
Trapezoid<T>& Trapezoid<T>::operator=(const Trapezoid& other) {
    if (this != &other) {
        for (int i = 0; i < FOUR_VERTICES; ++i) {
            vertices[i] = std::make_unique<Point<T>>(*other.vertices[i]);
        }
    }
    return *this;
}

template<Scalar T>
Trapezoid<T>& Trapezoid<T>::operator=(Trapezoid&& other) noexcept {
    if (this != &other) {
        for (int i = 0; i < FOUR_VERTICES; ++i) {
            vertices[i] = std::move(other.vertices[i]);
        }
    }
    return *this;
}

template<Scalar T>
bool Trapezoid<T>::operator==(const Figure<T>& other) const {
    const Trapezoid* rect = dynamic_cast<const Trapezoid*>(&other);
    if (!rect) return false;

    for (int i = 0; i < FOUR_VERTICES; ++i) {
        if (!(*vertices[i] == *rect->vertices[i])) {
            return false;
        }
    }
    return true;
}

template<Scalar T>
Point<T> Trapezoid<T>::Center() const {
    T centerX = 0, centerY = 0;
    for (int i = 0; i < FOUR_VERTICES; ++i) {
        centerX += vertices[i]->x;
        centerY += vertices[i]->y;
    }
    return Point<T>(centerX / FOUR_VERTICES, centerY / FOUR_VERTICES);
}

template<Scalar T>
double Trapezoid<T>::Area() const {
    double base1 = std::sqrt(std::pow(vertices[1]->x - vertices[0]->x, 2) + std::pow(vertices[1]->y - vertices[0]->y, 2));
    double base2 = std::sqrt(std::pow(vertices[3]->x - vertices[2]->x, 2) + std::pow(vertices[3]->y - vertices[2]->y, 2));
    double height = std::abs(vertices[0]->y - vertices[2]->y);
    return 0.5 * (base1 + base2) * height;
}

template<Scalar T>
void Trapezoid<T>::Print(std::ostream& os) const {
    os << "Trapezoid: ";
    for (int i = 0; i < FOUR_VERTICES; ++i) {
        os << *vertices[i];
        if (i < 3) os << " ";
    }
}

template<Scalar T>
void Trapezoid<T>::Read(std::istream& is) {
    for (int i = 0; i < FOUR_VERTICES; ++i) {
        T x, y;
        is >> x >> y;
        vertices[i] = std::make_unique<Point<T>>(x, y);
    }
}

template<Scalar T>
std::shared_ptr<Figure<T>> Trapezoid<T>::clone() const {
    return std::make_shared<Trapezoid<T>>(*this);
}

} // namespace figure
