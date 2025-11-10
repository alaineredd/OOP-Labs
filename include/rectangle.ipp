#include <cmath>

#include "rectangle.hpp"

namespace figure {

template<Scalar T>
Rectangle<T>::Rectangle() 
    : vertices{
        std::make_unique<Point<T>>(0, 0),
        std::make_unique<Point<T>>(1, 0),
        std::make_unique<Point<T>>(1, 1),
        std::make_unique<Point<T>>(0, 1)
    } {}

template<Scalar T>
Rectangle<T>::Rectangle(const Point<T>& p1, const Point<T>& p2, const Point<T>& p3, const Point<T>& p4)
    : vertices{
        std::make_unique<Point<T>>(p1),
        std::make_unique<Point<T>>(p2),
        std::make_unique<Point<T>>(p3),
        std::make_unique<Point<T>>(p4)
    } {}

template<Scalar T>
Rectangle<T>::Rectangle(const Rectangle& other) {
    for (int i = 0; i < FOUR_VERTICES; ++i) {
        vertices[i] = std::make_unique<Point<T>>(*other.vertices[i]);
    }
}

template<Scalar T>
Rectangle<T>::Rectangle(Rectangle&& other) noexcept {
    for (int i = 0; i < FOUR_VERTICES; ++i) {
        vertices[i] = std::move(other.vertices[i]);
    }
}

template<Scalar T>
Rectangle<T>& Rectangle<T>::operator=(const Rectangle& other) {
    if (this != &other) {
        for (int i = 0; i < FOUR_VERTICES; ++i) {
            vertices[i] = std::make_unique<Point<T>>(*other.vertices[i]);
        }
    }
    return *this;
}

template<Scalar T>
Rectangle<T>& Rectangle<T>::operator=(Rectangle&& other) noexcept {
    if (this != &other) {
        for (int i = 0; i < FOUR_VERTICES; ++i) {
            vertices[i] = std::move(other.vertices[i]);
        }
    }
    return *this;
}

template<Scalar T>
bool Rectangle<T>::operator==(const Figure<T>& other) const {
    const Rectangle* rect = dynamic_cast<const Rectangle*>(&other);
    if (!rect) return false;

    for (int i = 0; i < FOUR_VERTICES; ++i) {
        if (!(*vertices[i] == *rect->vertices[i])) {
            return false;
        }
    }
    return true;
}

template<Scalar T>
Point<T> Rectangle<T>::Center() const {
    T centerX = 0, centerY = 0;
    for (int i = 0; i < FOUR_VERTICES; ++i) {
        centerX += vertices[i]->x;
        centerY += vertices[i]->y;
    }
    return Point<T>(centerX / FOUR_VERTICES, centerY / FOUR_VERTICES);
}

template<Scalar T>
double Rectangle<T>::Area() const {
    double width = std::sqrt(std::pow(vertices[1]->x - vertices[0]->x, 2) + 
                            std::pow(vertices[1]->y - vertices[0]->y, 2));
    double height = std::sqrt(std::pow(vertices[2]->x - vertices[1]->x, 2) + 
                             std::pow(vertices[2]->y - vertices[1]->y, 2));
    return width * height;
}

template<Scalar T>
void Rectangle<T>::Print(std::ostream& os) const {
    os << "rectangle: ";
    for (int i = 0; i < FOUR_VERTICES; ++i) {
        os << *vertices[i];
        if (i < 3) os << " ";
    }
}

template<Scalar T>
void Rectangle<T>::Read(std::istream& is) {
    for (int i = 0; i < FOUR_VERTICES; ++i) {
        T x, y;
        is >> x >> y;
        vertices[i] = std::make_unique<Point<T>>(x, y);
    }
}

template<Scalar T>
std::shared_ptr<Figure<T>> Rectangle<T>::clone() const {
    return std::make_shared<Rectangle<T>>(*this);
}

} // namespace figure
