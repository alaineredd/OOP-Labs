#include <cmath>
#include <algorithm>

#include "trapezoid.h"

namespace figure {

Trapezoid::Trapezoid() : vertices{{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}} {}

Trapezoid::Trapezoid(const Point& p1, const Point& p2, const Point& p3, const Point& p4) : vertices{p1, p2, p3, p4} {}

Trapezoid::~Trapezoid() = default;

Trapezoid& Trapezoid::operator=(const Trapezoid& other) {
    if (this != &other) {
        for (int i = 0; i < TETRA_VERTICE; ++i) {
            vertices[i] = other.vertices[i];
        }
    }
    return *this;
}

Trapezoid& Trapezoid::operator=(Trapezoid&& other) {
    if (this != &other) {
        for (int i = 0; i < TETRA_VERTICE; ++i) {
            vertices[i] = std::move(other.vertices[i]);
            other.vertices[i] = {0.0, 0.0};
        }
    }
    return *this;
}

Point Trapezoid::Center() const {
    Point center = {0.0, 0.0};
    for (int i = 0; i < TETRA_VERTICE; ++i) {
        center.x += vertices[i].x;
        center.y += vertices[i].y;
    }
    center.x /= TETRA_VERTICE;
    center.y /= TETRA_VERTICE;
    return center;
}

double Trapezoid::Area() const {
    double base1 = std::sqrt(std::pow(vertices[1].x - vertices[0].x, 2) + std::pow(vertices[1].y - vertices[0].y, 2));
    double base2 = std::sqrt(std::pow(vertices[3].x - vertices[2].x, 2) + std::pow(vertices[3].y - vertices[2].y, 2));
    double height = std::abs(vertices[0].y - vertices[2].y);
    return 0.5 * (base1 + base2) * height;
}

Trapezoid::operator double() {
    return Area();
}

void Trapezoid::Print(std::ostream& out) const {
    out << "trapezoid\n";
    for (int i = 0; i < TETRA_VERTICE; ++i) {
        out << vertices[i];
        if (i < TETRA_VERTICE - 1) {
            out << ", ";
        }
    }
}

Point Trapezoid::GetVertex(int index) const {
    if (index < 0 || index >= TETRA_VERTICE) {
        throw std::out_of_range("vertex index out of range");
    }
    return vertices[index];
}

bool Trapezoid::IsValid() const {
    for (int i = 0; i < TETRA_VERTICE; ++i) {
        for (int j = i + 1; j < TETRA_VERTICE; ++j) {
            if (vertices[i] == vertices[j]) {
                return false;
            }
        }
    }
    
    double slope1 = (vertices[1].y - vertices[0].y) / (vertices[1].x - vertices[0].x);
    double slope2 = (vertices[3].y - vertices[2].y) / (vertices[3].x - vertices[2].x);
    
    return std::abs(slope1 - slope2) < EPSILON;
}

bool operator==(const Trapezoid& first, const Trapezoid& second) {
    for (int i = 0; i < TETRA_VERTICE; ++i) {
        if (std::abs(first.vertices[i].x - second.vertices[i].x) > EPSILON ||
            std::abs(first.vertices[i].y - second.vertices[i].y) > EPSILON) {
            return false;
        }
    }
    return true;
}

bool operator!=(const Trapezoid& first, const Trapezoid& second) {
    return !(first == second);
}

std::istream &operator>>(std::istream &in, Trapezoid& trapezoid) {
    for (int i = 0; i < TETRA_VERTICE; ++i) {
        in >> trapezoid.vertices[i];
    }
    return in;
}

std::ostream &operator<<(std::ostream &out, const Trapezoid& trapezoid) {
    trapezoid.Print(out);
    return out;
}

} // namespace figure