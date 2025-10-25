#include <cmath>
#include <algorithm>

#include "../include/trapezoid.h"

namespace figure {

Trapezoid::Trapezoid() {
    vertices[0] = Point(0.0, 0.0);
    vertices[1] = Point(1.0, 0.0);
    vertices[2] = Point(1.0, 1.0);
    vertices[3] = Point(0.0, 1.0);
}

Trapezoid::Trapezoid(const Point& p1, const Point& p2, const Point& p3, const Point& p4) {
    vertices[0] = p1;
    vertices[1] = p2;
    vertices[2] = p3;
    vertices[3] = p4;
}

Trapezoid::Trapezoid(const Trapezoid& other) {
    for (int i = 0; i < TETRAVERTICES; ++i) {
        vertices[i] = other.vertices[i];
    }
}

Trapezoid::Trapezoid(Trapezoid&& other) {
    for (int i = 0; i < TETRAVERTICES; ++i) {
        vertices[i] = std::move(other.vertices[i]);
        other.vertices[i] = {0.0, 0.0};
    }
}

Trapezoid::~Trapezoid() = default;

Trapezoid& Trapezoid::operator=(const Trapezoid& other) {
    if (this != &other) {
        for (int i = 0; i < TETRAVERTICES; ++i) {
            vertices[i] = other.vertices[i];
        }
    }
    return *this;
}

Trapezoid& Trapezoid::operator=(Trapezoid&& other) {
    if (this != &other) {
        for (int i = 0; i < TETRAVERTICES; ++i) {
            vertices[i] = std::move(other.vertices[i]);
            other.vertices[i] = {0.0, 0.0};
        }
    }
    return *this;
}

Point Trapezoid::Center() const {
    Point center = {0.0, 0.0};
    for (int i = 0; i < TETRAVERTICES; ++i) {
        center.x += vertices[i].x;
        center.y += vertices[i].y;
    }
    center.x /= TETRAVERTICES;
    center.y /= TETRAVERTICES;
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
    for (int i = 0; i < TETRAVERTICES; ++i) {
        out << "(" << vertices[i].x << ", " << vertices[i].y << ")";
        if (i < TETRAVERTICES - 1) out << ", ";
    }
}

Point Trapezoid::GetVertex(int index) const {
    if (index < 0 || index >= TETRAVERTICES) {
        throw std::out_of_range("vertex index out of range");
    }
    return vertices[index];
}

bool Trapezoid::IsValid() const {
    for (int i = 0; i < TETRAVERTICES; ++i) {
        for (int j = i + 1; j < TETRAVERTICES; ++j) {
            if (vertices[i].x == vertices[j].x && vertices[i].y == vertices[j].y) {
                return false;
            }
        }
    }
    
    double slope1 = (vertices[1].y - vertices[0].y) / (vertices[1].x - vertices[0].x);
    double slope2 = (vertices[3].y - vertices[2].y) / (vertices[3].x - vertices[2].x);
    
    return std::abs(slope1 - slope2) < EPSILON;
}

bool operator==(const Trapezoid& first, const Trapezoid& second) {
    for (int i = 0; i < TETRAVERTICES; ++i) {
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
    for (int i = 0; i < TETRAVERTICES; ++i) {
        in >> trapezoid.vertices[i].x >> trapezoid.vertices[i].y;
    }
    return in;
}

std::ostream &operator<<(std::ostream &out, const Trapezoid& trapezoid) {
    out << "Trapezoid vertices: ";
    for (int i = 0; i < TETRAVERTICES; ++i) {
        out << "(" << trapezoid.vertices[i].x << ", " << trapezoid.vertices[i].y << ")";
        if (i < TETRAVERTICES - 1) out << " ";
    }
    return out;
}

} // namespace figure