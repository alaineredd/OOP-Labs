#include <cmath>

#include "rhombus.hpp"

namespace figure {

template<Scalar T>
Rhombus<T>::Rhombus() 
    : vertices{
        std::make_unique<Point<T>>(0, 0),
        std::make_unique<Point<T>>(1, 0),
        std::make_unique<Point<T>>(1, 1),
        std::make_unique<Point<T>>(0, 1)
    } {}

template<Scalar T>
Rhombus<T>::Rhombus(const Point<T>& p1, const Point<T>& p2, const Point<T>& p3, const Point<T>& p4)
    : vertices{
        std::make_unique<Point<T>>(p1),
        std::make_unique<Point<T>>(p2),
        std::make_unique<Point<T>>(p3),
        std::make_unique<Point<T>>(p4)
    } {}

template<Scalar T>
Rhombus<T>::Rhombus(const Rhombus& other) {
    for (int i = 0; i < FOUR_VERTICES; ++i) {
        vertices[i] = std::make_unique<Point<T>>(*other.vertices[i]);
    }
}

template<Scalar T>
Rhombus<T>::Rhombus(Rhombus&& other) noexcept {
    for (int i = 0; i < FOUR_VERTICES; ++i) {
        vertices[i] = std::move(other.vertices[i]);
    }
}

template<Scalar T>
Rhombus<T>& Rhombus<T>::operator=(const Rhombus& other) {
    if (this != &other) {
        for (int i = 0; i < FOUR_VERTICES; ++i) {
            vertices[i] = std::make_unique<Point<T>>(*other.vertices[i]);
        }
    }
    return *this;
}

template<Scalar T>
Rhombus<T>& Rhombus<T>::operator=(Rhombus&& other) noexcept {
    if (this != &other) {
        for (int i = 0; i < FOUR_VERTICES; ++i) {
            vertices[i] = std::move(other.vertices[i]);
        }
    }
    return *this;
}

template<Scalar T>
bool Rhombus<T>::operator==(const Figure<T>& other) const {
    const Rhombus* rect = dynamic_cast<const Rhombus*>(&other);
    if (!rect) return false;

    for (int i = 0; i < FOUR_VERTICES; ++i) {
        if (!(*vertices[i] == *rect->vertices[i])) {
            return false;
        }
    }
    return true;
}

template<Scalar T>
Point<T> Rhombus<T>::Center() const {
    T centerX = 0, centerY = 0;
    for (int i = 0; i < FOUR_VERTICES; ++i) {
        centerX += vertices[i]->x;
        centerY += vertices[i]->y;
    }
    return Point<T>(centerX / FOUR_VERTICES, centerY / FOUR_VERTICES);
}

template<Scalar T>
double Rhombus<T>::Area() const {
    double d1 = std::sqrt(std::pow(vertices[2]->x - vertices[0]->x, 2) + std::pow(vertices[2]->y - vertices[0]->y, 2));
    double d2 = std::sqrt(std::pow(vertices[3]->x - vertices[1]->x, 2) + std::pow(vertices[3]->y - vertices[1]->y, 2));
    return 0.5 * d1 * d2;
}

template<Scalar T>
void Rhombus<T>::Print(std::ostream& os) const {
    os << "Rhombus: ";
    for (int i = 0; i < FOUR_VERTICES; ++i) {
        os << *vertices[i];
        if (i < 3) os << " ";
    }
}

template<Scalar T>
void Rhombus<T>::Read(std::istream& is) {
    for (int i = 0; i < FOUR_VERTICES; ++i) {
        T x, y;
        is >> x >> y;
        vertices[i] = std::make_unique<Point<T>>(x, y);
    }
}

template<Scalar T>
std::shared_ptr<Figure<T>> Rhombus<T>::clone() const {
    return std::make_shared<Rhombus<T>>(*this);
}

} // namespace figure
